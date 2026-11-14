/**
 *
 * Copyright (c) 2013-2018 Pascal Gauthier.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 *
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "GlobalEditor.h"
#include "ParamDialog.h"
#include "SysexComm.h"
#include "TuningShow.h"
#include "Dexed.h"
#include "math.h"
#include <fstream>
#include <memory>

#include "msfa/fm_op_kernel.h"

// Command+1-6, Command+G, Command+K, and Command+L all jump keyboard focus straight to a
// control inside some panel or dialog, skipping the panel itself, so VoiceOver/NVDA only
// ever announce that control's own name, never which panel it's in. Speaking the panel's
// name first fixes that, but only when focus is arriving from somewhere else; otherwise
// jumping to the same panel repeatedly, or jumping within a panel you're already in, would
// repeat its name on every press.
//
// The announcement and the actual focus move can't happen too close together: watching the
// real accessibility notifications confirmed both are posted correctly, in the right order,
// but VoiceOver cancels the announcement's speech the instant the focus-changed notification
// arrives, so only the control's name was ever heard, never the group. Deferring the focus
// move by a fraction of a second, instead of just to the next turn of the event loop, gives
// VoiceOver time to actually finish speaking the group name first.
static void announceGroupThenFocus(Component& group, std::function<void()> focusAction) {
    auto *current = Component::getCurrentlyFocusedComponent();
    if ( current == nullptr || ! group.isParentOf(current) ) {
        AccessibilityHandler::postAnnouncement(group.getTitle(), AccessibilityHandler::AnnouncementPriority::medium);
        Timer::callAfterDelay(1000, focusAction);
    } else {
        focusAction();
    }
}

// Which of the named panels, if any, a given component sits inside. Used to remember where
// focus was before the cartridge library opened, so leaving it again can announce that same
// panel, the same way jumping to it would.
static Component* findEnclosingNamedGroup(Component *focused, OperatorEditor *operators,
                                            GlobalEditor &global, MidiKeyboardComponent &midiKeyboard) {
    if ( focused == nullptr )
        return nullptr;

    for ( int i = 0; i < 6; i++ )
        if ( operators[i].isParentOf(focused) )
            return &operators[i];

    if ( global.isParentOf(focused) )
        return &global;

    if ( midiKeyboard.isParentOf(focused) )
        return &midiKeyboard;

    return nullptr;
}

//==============================================================================
DexedAudioProcessorEditor::DexedAudioProcessorEditor (DexedAudioProcessor* ownerFilter)
    : AudioProcessorEditor (ownerFilter),
      midiKeyboard (ownerFilter->keyboardState, MidiKeyboardComponent::horizontalKeyboard),
      cartManager(this)
{
    processor = ownerFilter;

    // We have to set size at startup because the keyboard doesnt show up before being added
    resetSize();
    setExplicitFocusOrder(1);


    frameComponent.setBounds(0,0, WINDOW_SIZE_X, (processor->showKeyboard ? WINDOW_SIZE_Y : WINDOW_SIZE_Y - 94));
    addAndMakeVisible(&frameComponent);
    lookAndFeel->setDefaultLookAndFeel(lookAndFeel);
    background = lookAndFeel->background;

    // OPERATORS
    frameComponent.addAndMakeVisible(&(operators[0]));
    operators[0].setBounds(2, 1, 287, 218);
    operators[0].bind(processor, 0);

    frameComponent.addAndMakeVisible(&(operators[1]));
    operators[1].setBounds(290, 1, 287, 218);
    operators[1].bind(processor, 1);

    frameComponent.addAndMakeVisible(&(operators[2]));
    operators[2].setBounds(578, 1, 287, 218);
    operators[2].bind(processor, 2);

    frameComponent.addAndMakeVisible(&(operators[3]));
    operators[3].setBounds(2, 219, 287, 218);
    operators[3].bind(processor, 3);

    frameComponent.addAndMakeVisible(&(operators[4]));
    operators[4].setBounds(290, 219, 287, 218);
    operators[4].bind(processor, 4);

    frameComponent.addAndMakeVisible(&(operators[5]));
    operators[5].setBounds(578, 219, 287, 218);
    operators[5].bind(processor, 5);

    // add the midi keyboard component..
    frameComponent.addAndMakeVisible (&midiKeyboard);

    // The DX7 is a badass on the bass, keep it that way
    midiKeyboard.setLowestVisibleKey(24);
    midiKeyboard.setBounds(4, 581, getWidth() - 8, 90);
    midiKeyboard.setTitle("Keyboard keys");
    midiKeyboard.setDescription("Shows which notes are playing. Play notes with a MIDI keyboard, the computer keyboard, or the mouse.");

    // Without this, VoiceOver's interact command reports the keyboard as an empty group,
    // because JUCE only exposes a component's children to that command if the component is
    // marked as a focus container. Marking it one lets interact reach the two scroll buttons
    // below instead of reporting nothing there.
    midiKeyboard.setFocusContainerType(Component::FocusContainerType::focusContainer);

    // JUCE's keyboard component adds two scroll buttons, one at each end, to shift the
    // visible octave range. They are its only two child components, always in this order,
    // and JUCE never gives them a name, so VoiceOver reports them as unlabeled. Name them
    // here instead of patching vendored JUCE code.
    if (auto *scrollDown = dynamic_cast<Button*>(midiKeyboard.getChildComponent(0))) {
        scrollDown->setTitle("Scroll Keyboard Down an Octave");
        scrollDown->setTooltip("Moves the visible range of the on-screen keyboard down by one octave.");
    }
    if (auto *scrollUp = dynamic_cast<Button*>(midiKeyboard.getChildComponent(1))) {
        scrollUp->setTitle("Scroll Keyboard Up an Octave");
        scrollUp->setTooltip("Moves the visible range of the on-screen keyboard up by one octave.");
    }

    frameComponent.addAndMakeVisible(&global);
    global.setBounds(2,436,864,144);
    global.bind(this);

    global.setMonoState(processor->isMonoMode());

    rebuildProgramCombobox();
    global.programs->addListener(this);

    frameComponent.addChildComponent(&cartManagerCover);
    cartManagerCover.addChildComponent(&cartManager);
    cartManager.setVisible(true);

    AffineTransform scale = AffineTransform::scale(processor->getZoomFactor());
    frameComponent.setTransform(scale);
    resetSize();
    addKeyListener(this);
    updateUI();
    startTimer(100);
}

DexedAudioProcessorEditor::~DexedAudioProcessorEditor() {
    stopTimer();
    processor->unbindUI();
    setLookAndFeel(nullptr);
}

//==============================================================================
void DexedAudioProcessorEditor::paint (Graphics& g) {
    g.setColour(background);
    g.fillRoundedRectangle(0.0f, 0.0f, (float) getWidth(), (float) getHeight(), 0);
}

void DexedAudioProcessorEditor::cartShow() {
    preCartridgeLibraryGroup = findEnclosingNamedGroup(Component::getCurrentlyFocusedComponent(),
                                                        operators, global, midiKeyboard);

    stopTimer();
    cartManager.resetActiveSysex();
    cartManagerCover.setBounds(0, 0, WINDOW_SIZE_X , WINDOW_SIZE_Y - 94);
    cartManager.setBounds(16, 16, cartManagerCover.getWidth() - 32, cartManagerCover.getHeight() - 32);
    cartManager.updateCartFilename();
    cartManagerCover.setVisible(true);

    // The cartridge manager isn't a real modal window, so it doesn't block keyboard or MIDI
    // input to the rest of the plugin, on purpose, to keep playing notes while browsing.
    // But that also means VoiceOver can walk straight out of it into the operators behind
    // it. Hiding the rest of the UI from accessibility while this is open stops that.
    setMainUIAccessible(false);

    cartManager.initialFocus();
}

void DexedAudioProcessorEditor::setMainUIAccessible(bool accessible) {
    for (int i = 0; i < 6; i++)
        operators[i].setAccessible(accessible);
    global.setAccessible(accessible);
    midiKeyboard.setAccessible(accessible);
}

std::unique_ptr<ComponentTraverser> DexedAudioProcessorEditor::createFocusTraverser() {
    return std::make_unique<FocusTraverser>();
}

void DexedAudioProcessorEditor::loadCart(File file) {
    Cartridge cart;

    int rc = cart.load(file);

    if ( rc < 0 ) {
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                          "Error",
                                          "Unable to open: " + file.getFullPathName());
        return;
    }

    if ( rc != 0 ) {
        rc = AlertWindow::showOkCancelBox(AlertWindow::QuestionIcon, "Unable to find DX7 sysex cartridge in file",
                                          "This sysex file is not for the DX7 or it is corrupted. "
                                          "Do you still want to load this file as random data ?");
        if ( rc == 0 )
            return;
    }

    processor->loadCartridge(cart);
    rebuildProgramCombobox();
    processor->setCurrentProgram(0);
    global.programs->setSelectedId(processor->getCurrentProgram()+1, dontSendNotification);
    processor->updateHostDisplay();

    processor->activeFileCartridge = file;
}

void DexedAudioProcessorEditor::saveCart() {
    File startFileName = processor->activeFileCartridge.exists() ? processor->activeFileCartridge : processor->dexedCartDir;

    FileChooser fc ("Export DX sysex...", processor->dexedCartDir, "*.syx;*.SYX", 1);
    if ( fc.browseForFileToSave(true) ) {
        if ( ! processor->currentCart.saveVoice(fc.getResults().getReference(0)) ) {
            AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                              "Error",
                                              "Unable to write: " + fc.getResults().getReference(0).getFullPathName());
        }
    }
}

void DexedAudioProcessorEditor::tuningShow() {
    auto te = new TuningShow();
    te->setTuning( processor->synthTuningState->getTuning() );

    DialogWindow::LaunchOptions options;
    options.content.setOwned(te);
    options.dialogTitle = "Current Tuning";
    options.dialogBackgroundColour = Colour(0xFF323E44);
    options.escapeKeyTriggersCloseButton = true;
    options.useNativeTitleBar = false;
    options.resizable = false;

    auto dialogwindow = options.launchAsync();
}

// I don't know why closeButtonPressed is not implemented in the standard DialogWindow version.
class DexedDialogWindow : public juce::DialogWindow {
    // DialogWindow's constructor registers Escape as a keyboard shortcut directly on its own
    // close button, as well as handling Escape itself (see escapeKeyPressed below). A single
    // Escape press ends up triggering both, which would call onCloseRequested twice: once to
    // start the announce-then-close sequence, and a second time immediately afterward with
    // nothing left to remember, closing right away and cutting the announcement off. This
    // guard makes a second close request while one is already underway a no-op.
    bool closeRequested = false;
public:
    // Set by whoever opens this dialog, so closing it (its own close button, or Escape, which
    // both already land here) can announce the panel the user is returning to before focus
    // actually settles there, the same way opening a panel does.
    std::function<void()> onCloseRequested;

    DexedDialogWindow(const juce::String& title, juce::Colour backgroundColour)
        : juce::DialogWindow(title, backgroundColour, true, false) {

    }
    void closeButtonPressed() override {
        if ( closeRequested )
            return;
        closeRequested = true;

        if ( onCloseRequested )
            onCloseRequested();
        else
            setVisible(false);
    }

    // DialogWindow's own escape-key handling hides the window directly instead of going
    // through closeButtonPressed(), so without this override Escape would skip
    // onCloseRequested entirely and silently hide the dialog with no announcement.
    bool escapeKeyPressed() override {
        closeButtonPressed();
        return true;
    }
};

// returnGroupOverride lets Command+P, which has to wait a second before this actually runs (see
// the keyPressed handler), capture which panel to return to right away instead of waiting until
// this runs to look at what's currently focused. In Logic Pro specifically, focus had already
// moved on by the time the delay elapsed, so capturing it here was landing on the wrong panel,
// Operator 1, instead of wherever the user actually was. The Function Settings button inside
// Global Parameters calls this directly with no delay, so it still captures the current focus
// itself, by passing nothing.
void DexedAudioProcessorEditor::parmShow(Component *returnGroupOverride) {
    int tp = processor->getEngineType();
    auto param = new ParamDialog();
    param->setColour(AlertWindow::backgroundColourId, Colour(0xFF323E44));
    param->setDialogValues(processor->controllers, processor->sysexComm, tp, processor->showKeyboard, processor->getZoomFactor());
    param->setIsStandardTuning(processor->synthTuningState->is_standard_tuning() );
    param->setTuningCallback([this](ParamDialog *p, ParamDialog::TuningAction which) {
                                switch(which)
                                {
                                case ParamDialog::LOAD_SCL:
                                    this->processor->applySCLTuning();
                                    break;
                                case ParamDialog::LOAD_KBM:
                                    this->processor->applyKBMMapping();
                                    break;
                                case ParamDialog::RESET_TUNING:
                                    this->processor->retuneToStandard();
                                    break;
                                case ParamDialog::SHOW_TUNING:
                                    // consider https://forum.juce.com/t/closing-a-modal-dialog-window/2961
                                    this->tuningShow();
                                    break;
                                }
                                p->setIsStandardTuning(this->processor->synthTuningState->is_standard_tuning() );
                            } );

    auto generalCallback = [this](ParamDialog *param)
                               {
                                   int tpo;
                                   float scale = this->processor->getZoomFactor();
                                   bool ret = param->getDialogValues(this->processor->controllers, this->processor->sysexComm, &tpo, &this->processor->showKeyboard, &scale);
                                   this->processor->setZoomFactor(scale);
                                   this->processor->setEngineType(tpo);
                                   this->processor->savePreference();

                                   AffineTransform scaleAffine = AffineTransform::scale(this->processor->getZoomFactor());
                                   frameComponent.setTransform(scaleAffine);
                                   resetSize();

                                   if ( ret == false ) {
                                       AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Midi Interface", "Error opening midi ports");
                                   }
                               };
    param->setGeneralCallback(generalCallback);

    preParamDialogGroup = returnGroupOverride != nullptr
        ? returnGroupOverride
        : findEnclosingNamedGroup(Component::getCurrentlyFocusedComponent(), operators, global, midiKeyboard);

    auto *dialog = new DexedDialogWindow("dexed Parameters", Colour(0xFF323E44));
    dialog->onCloseRequested = [this, dialog] {
        if ( auto *f = fopen("/tmp/dexed_au_diag.log", "a") ) {
            fprintf(f, "[DIAG] closing param dialog; preParamDialogGroup = %s\n",
                    preParamDialogGroup != nullptr ? preParamDialogGroup->getTitle().toRawUTF8() : "nullptr");
            fclose(f);
        }
        if ( preParamDialogGroup != nullptr ) {
            Component *returnGroup = preParamDialogGroup;
            preParamDialogGroup = nullptr;
            announceGroupThenFocus(*returnGroup, [dialog] { dialog->setVisible(false); });
        } else {
            dialog->setVisible(false);
        }
    };

    dexedParameterDialog.reset(dialog);
    dexedParameterDialog->setContentOwned(param, true);
    frameComponent.addAndMakeVisible(dexedParameterDialog.get());
    dexedParameterDialog->centreAroundComponent(&frameComponent, param->getWidth(), param->getHeight() + dexedParameterDialog->getTitleBarHeight());
    dexedParameterDialog->enterModalState(true,{});
}

void DexedAudioProcessorEditor::initProgram() {
    processor->resetToInitVoice();
}

void DexedAudioProcessorEditor::comboBoxChanged (ComboBox* comboBoxThatHasChanged) {
    processor->setCurrentProgram(global.programs->getSelectedId()-1);
    processor->updateHostDisplay();
}

void DexedAudioProcessorEditor::timerCallback() {
    // A top-level JUCE window wants keyboard focus itself, so when the standalone window
    // opens, focus lands on the window and never reaches any control. Tab then does nothing,
    // since the window has no siblings to move to, and the shortcuts in keyPressed never fire,
    // since key events go to the focused component and its parents, not its children. Focus
    // can only be given once the editor is on screen, so do it from a timer tick. Wait until
    // the window is also the active one: NVDA drops focus events from background windows, and
    // JUCE does not repeat the event when the window is activated later. Then wait a few more
    // ticks: NVDA announces the window activation itself a moment later, and if our focus
    // event arrives first, that later announcement leaves NVDA thinking the window has focus.
    auto *peer = getPeer();
    if ( ! initialFocusGiven && isShowing() && peer != nullptr && peer->isFocused()
         && ++initialFocusTicks >= 4 ) {
        initialFocusGiven = true;
        if ( ! hasKeyboardFocus(true) )
            grabKeyboardFocus();
    }

    if ( processor->forceRefreshUI ) {
        processor->forceRefreshUI = false;
        updateUI();
    }

    if ( ! processor->peekVoiceStatus() )
        return;

    for(int i=0;i<6;i++) {
        //operators[i].updateGain(sqrt(processor->voiceStatus.amp[5 - i]) / 8196);        // TODO: FUGLY !!!! change this sqrt nonsense

        const int amp_min = 1036152;
        const float one_per_amp_diff = (float)(1.0 / (259037922 - amp_min));
        // Note, that minimum and maximum values of ``amp`` are different and depend on the engines. 
        // These two constants were determined from the results produced by the OPL engine,
        // because its minimum is smaller, its maximum is higher than in the other cases. 

        int amp = processor->voiceStatus.amp[5 - i] - amp_min;
        if (amp <= 0) amp = 0;
        operators[i].updateGain(amp * one_per_amp_diff);


        operators[i].updateEnvPos(processor->voiceStatus.ampStep[5 - i]);
    }
    global.updatePitchPos(processor->voiceStatus.pitchStep);
    global.updateVu(processor->vuSignal);
}

void DexedAudioProcessorEditor::updateUI() {
    for(int i=0;i<processor->ctrl.size();i++) {
        processor->ctrl[i]->updateComponent();
    }
    for(int i=0;i<6;i++) {
        operators[i].updateDisplay();
    }
    rebuildProgramCombobox();
    global.updateDisplay();
    cartManager.updateCartFilename();
}

void DexedAudioProcessorEditor::rebuildProgramCombobox() {
    global.programs->clear(dontSendNotification);

    processor->currentCart.getProgramNames(processor->programNames);

    for(int i=0;i<processor->getNumPrograms();i++) {
        String id;
        id << (i+1) << ". " << processor->getProgramName(i);
        global.programs->addItem(id, i+1);
    }

    global.programs->setSelectedId(processor->getCurrentProgram()+1, dontSendNotification);

    String name = Cartridge::normalizePgmName((const char *) processor->data+145);
    cartManager.setActiveProgram(processor->getCurrentProgram(), name);
    if ( name != processor->getProgramName(processor->getCurrentProgram()) )
        global.programs->setText("**. " + name, dontSendNotification);

    cartManager.resetActiveSysex();
}

void DexedAudioProcessorEditor::storeProgram() {
    String currentName = Cartridge::normalizePgmName((const char *) processor->data+145);
    Cartridge destSysex = processor->currentCart;
    File *externalFile = NULL;

    bool activeCartridgeFound = processor->activeFileCartridge.exists();

    while (true) {
        String msg;

        if ( externalFile == NULL ) {
            if ( activeCartridgeFound )
                msg = "Store program to current (" + processor->activeFileCartridge.getFileName() + ") / new cartridge";
            else
                msg = "Store program to current / new cartridge";
        } else {
            msg = "Store program to " + externalFile->getFileName();
        }

        AlertWindow dialog("Store Program", msg, AlertWindow::NoIcon, this);
        dialog.addTextEditor("Name", currentName, String("Name"), false);
        // TODO: fix the name length to 10

        StringArray programs;
        destSysex.getProgramNames(programs);
        dialog.addComboBox("Dest", programs, "Program Destination");

        if ( externalFile == NULL ) {
            StringArray saveAction;
            saveAction.add("Store program to DAW plugin song state");
            saveAction.add("Store program and create a new copy of the .syx cartridge");
            if ( activeCartridgeFound )
                saveAction.add("Store program and overwrite current .syx cartridge");

            dialog.addComboBox("SaveAction", saveAction, "Store Action");
        }

        dialog.addButton("OK", 0, KeyPress(KeyPress::returnKey));
        dialog.addButton("CANCEL", 1, KeyPress(KeyPress::escapeKey));
        dialog.addButton("EXTERNAL FILE", 2, KeyPress());
        int response = dialog.runModalLoop();

        if ( response == 2 ) {
            FileChooser fc("Destination Sysex", processor->dexedCartDir, "*.syx;*.SYX;*.*", 1);

            if ( fc.browseForFileToOpen() ) {
                if ( externalFile != NULL ) 
                    delete externalFile;

                externalFile = new File(fc.getResults().getReference(0));
                if ( destSysex.load(*externalFile) == 0 )
                    continue;
                AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Read error", "Unable to read file");
            }
        }

        if ( response == 0 ) {
            TextEditor *name = dialog.getTextEditor("Name");
            ComboBox *dest = dialog.getComboBoxComponent("Dest");

            int programNum = dest->getSelectedItemIndex();
            String programName(name->getText());
            if ( programName.length() > 10 ) {
                int toStrip = programName.length() - 10;
                programName = programName.dropLastCharacters(toStrip);
            }

            if ( externalFile == NULL ) {
                processor->currentCart.packProgram((uint8_t *) processor->data, programNum, programName, processor->controllers.opSwitch);
                rebuildProgramCombobox();
                processor->setCurrentProgram(programNum);
                processor->updateHostDisplay();

                int action = dialog.getComboBoxComponent("SaveAction")->getSelectedItemIndex();
                if ( action > 0 ) {
                    File destination = processor->activeFileCartridge;
                    if ( action == 1 ) {
                        FileChooser fc("Destination Sysex", processor->dexedCartDir, "*.syx;*.SYX", 1);
                        if ( ! fc.browseForFileToSave(true) )
                            break;
                        destination = fc.getResult();
                    }

                    processor->currentCart.saveVoice(destination);
                    processor->activeFileCartridge = destination;
                }
            } else {
                destSysex.packProgram((uint8_t *) processor->data, programNum, programName, processor->controllers.opSwitch);
                if ( ! destSysex.saveVoice(*externalFile)) {
                    AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Write error", "Unable to write file");
                }
            }
        }
        break;
    }

    if ( externalFile != NULL )
        delete externalFile;
    cartManager.resetActiveSysex();
}

class MidiCCListener: public AlertWindow, Value::Listener {
    DexedAudioProcessorEditor *editor;
    Ctrl *target;
public :
    MidiCCListener(DexedAudioProcessorEditor *editor, Ctrl *target) : AlertWindow("","", AlertWindow::InfoIcon, editor) {
        this->editor = editor;
        this->target = target;
        setMessage("Mapping: " + String(target->label) + ", waiting for midi controller change (CC) message...");
        addButton("CANCEL", -1);
        editor->processor->lastCCUsed.setValue(-1);
        editor->processor->lastCCUsed.addListener(this);
    }

    ~MidiCCListener() {
        editor->processor->lastCCUsed.removeListener(this);
    }

    void valueChanged(Value &value) {
        int cc = value.getValue();
        editor->processor->mappedMidiCC.remove(cc);
        editor->processor->mappedMidiCC.set(cc, target);
        editor->processor->savePreference();
        exitModalState(0);
    }
};

void DexedAudioProcessorEditor::discoverMidiCC(Ctrl *ctrl) {
    MidiCCListener ccListener(this, ctrl);
    ccListener.runModalLoop();
}

float DexedAudioProcessorEditor::getLargestScaleFactor() {
    // constexpr float TESTING_SCALE_FACTOR[] = { 4.0f, 3.0f, 2.0f, 1.5f, 1.0f };
    //
    // for (float factor: TESTING_SCALE_FACTOR) {
    //     const juce::Rectangle<int> rect(WINDOW_SIZE_X * factor, WINDOW_SIZE_Y * factor);
    //
    //     // validate if there is really a display that can show the complete plugin size
    //     for (auto& display : Desktop::getInstance().getDisplays().displays) {
    //         int height = display.userArea.getHeight();
    //         int width = display.userArea.getWidth();
    //
    //         TRACE("Testing size %d x %d < Dexed Window %d x %d", height, width, rect.getWidth(), rect.getHeight() );
    //         if ( height > rect.getHeight() && width > rect.getWidth() ) {
    //             TRACE("Found factor %f for display %s with size %d x %d", factor, display.userArea.toString().toRawUTF8(), height, width );
    //             return factor;
    //         }
    //     }
    // }
    //
    // TRACE("No suitable display found, returning default scale factor 1.0");

    // For now, always return 4.0f as the maximum scale factor.
    return 4.0f;
}

void DexedAudioProcessorEditor::resetZoomFactor() {
    TRACE("Resetting zoom factor to 1.0");
    processor->setZoomFactor(1.0);
    processor->savePreference();
    AffineTransform scale = AffineTransform::scale(processor->getZoomFactor());
    frameComponent.setTransform(scale);
    resetSize();
}

bool DexedAudioProcessorEditor::isInterestedInFileDrag (const StringArray &files)
{
    if( files.size() != 1 ) return false;

    for( auto i = files.begin(); i != files.end(); ++i )
    {
        if( i->endsWithIgnoreCase( ".scl" ) || i->endsWithIgnoreCase( ".kbm" ) )
            return true;
    }
    return false;
}

void DexedAudioProcessorEditor::filesDropped (const StringArray &files, int x, int y )
{
    if( files.size() != 1 ) return;
    auto fn = files[0];
    try {
        std::ifstream in(fn.toStdString(), std::ifstream::ate | std::ifstream::binary);
        std::ifstream::pos_type filesize = in.tellg();
        if (fn.endsWithIgnoreCase(".scl"))
        {
            if (filesize == 0) {
                AlertWindow::showMessageBox(
                    AlertWindow::WarningIcon,
                    "File size error!",
                    "File \'" + fn.toStdString() + "\' is empty."
                );
            }
            else if (filesize > MAX_SCL_KBM_FILE_SIZE) {
                AlertWindow::showMessageBox(
                    AlertWindow::WarningIcon,
                    "File size error!",
                    "File \'" + fn.toStdString() + "\' has " + std::to_string(filesize) + " bytes, exceeding the maximum limit ("+std::to_string(MAX_SCL_KBM_FILE_SIZE)+")."
                );
            }
            else {
                processor->applySCLTuning(File(fn));
            }
        }
        if (fn.endsWithIgnoreCase(".kbm"))
        {
            if (filesize == 0) {
                AlertWindow::showMessageBox(
                    AlertWindow::WarningIcon,
                    "File size error!",
                    "File \'" + fn.toStdString() + "\' is empty."
                );
            }
            else if (filesize > MAX_SCL_KBM_FILE_SIZE) {
                AlertWindow::showMessageBox(
                    AlertWindow::WarningIcon,
                    "File size error!",
                    "File \'" + fn.toStdString() + "\' has " + std::to_string(filesize) + " bytes, exceeding the maximum limit (" + std::to_string(MAX_SCL_KBM_FILE_SIZE) + ")."
                );
            }
            else {
                processor->applyKBMMapping(File(fn));
            }
        }
    }
    catch (const std::ios_base::failure& ex) {
        AlertWindow::showMessageBox(
            AlertWindow::WarningIcon, 
            "I/O error!", 
            "Related to file \'" + fn.toStdString() + "\', an exception (std::ios_base::failure) occured: " + ex.what()
        );
    }
    catch (std::bad_alloc& ex) {
        AlertWindow::showMessageBox(
            AlertWindow::WarningIcon, 
            "I/O error!", 
            "Related to file \'" + fn.toStdString() + "\', an exception (std::bad_alloc) occured: " + ex.what());
    }
    catch (...) {
        AlertWindow::showMessageBox(
            AlertWindow::WarningIcon, 
            "I/O error!", 
            "Related to file \'"+fn.toStdString()+"\', an unknown exception occured.");
    };
}

bool DexedAudioProcessorEditor::keyPressed(const KeyPress& key, Component* originatingComponent) {
    int keycode = key.getKeyCode();
    ModifierKeys mods = key.getModifiers();

    #ifdef DEXED_EVENT_DEBUG
        TRACE("key pressed: %d\n", keycode);
    #endif

    // While the cartridge library is open, the rest of the main window is hidden from
    // accessibility on purpose (see setMainUIAccessible), to keep VoiceOver from wandering
    // out of the dialog. Command+1-6, Command+G, and Command+K would otherwise still move
    // real keyboard focus to one of those now-hidden controls, so VoiceOver would announce
    // the jump but then have nothing to land on. Block them here instead, and say why.
    if ( cartManagerCover.isVisible() ) {
        bool isBlockedJump = ( ( (keycode >= '1' && keycode <= '6') && mods.isCommandDown() && ! mods.isShiftDown() )
                               || ( keycode == 'G' && mods.isCommandDown() )
                               || ( keycode == 'K' && mods.isCommandDown() ) );
        if ( isBlockedJump ) {
            AccessibilityHandler::postAnnouncement("Press Escape to exit the cartridge library",
                                                    AccessibilityHandler::AnnouncementPriority::medium);
            return true;
        }
    }

    // macOS reports the shifted symbol above each number key, not the digit itself,
    // once Shift is held, so Command+Shift+1 arrives as '!' rather than '1'. Map the
    // six shifted symbols back to the operator they stand in for.
    int shiftedDigitOp = -1;
    switch ( keycode ) {
        case '!': shiftedDigitOp = 0; break;
        case '@': shiftedDigitOp = 1; break;
        case '#': shiftedDigitOp = 2; break;
        case '$': shiftedDigitOp = 3; break;
        case '%': shiftedDigitOp = 4; break;
        case '^': shiftedDigitOp = 5; break;
    }

    bool handled = false;

    if ( (keycode >= '1' && keycode <= '6') && mods.isCommandDown() ) {
        int op = keycode - '1';

        if ( mods.isShiftDown() ) {
            operators[op].toggleOpSwitch();
        } else {
            announceGroupThenFocus(operators[op], [this, op] { operators[op].grabKeyboardFocus(); });
        }

        handled = true;
    } else if ( shiftedDigitOp >= 0 && mods.isCommandDown() && mods.isShiftDown() ) {
        operators[shiftedDigitOp].toggleOpSwitch();
        handled = true;
    }

    if ( handled )
        return true;

    if ( keycode == 'G' && mods.isCommandDown() ) {
        announceGroupThenFocus(global, [this] { global.grabKeyboardFocus(); });
        return true;
    }

    if ( keycode == 'K' && mods.isCommandDown() ) {
        if ( midiKeyboard.isShowing() )
            announceGroupThenFocus(midiKeyboard, [this] { midiKeyboard.grabKeyboardFocus(); });
        return true;
    }

    if ( keycode == 'L' && mods.isCommandDown() ) {
        announceGroupThenFocus(cartManager, [this] { cartShow(); });
        return true;
    }

    if ( keycode == 'P' && mods.isCommandDown() ) {
        // The dialog itself doesn't exist until parmShow() builds it, so there's no group
        // Component yet to hand announceGroupThenFocus for the "already inside it" check. That
        // check isn't needed here anyway: you can never already be inside this dialog when
        // Command+P is pressed, since it doesn't exist until this opens it.
        //
        // Capture the return panel here, right now, rather than waiting for the delayed
        // parmShow() call to do it: by the time that runs, a second from now, focus may no
        // longer be what it was at the moment of the keypress.
        Component *returnGroup = findEnclosingNamedGroup(Component::getCurrentlyFocusedComponent(),
                                                           operators, global, midiKeyboard);
        if ( auto *f = fopen("/tmp/dexed_au_diag.log", "a") ) {
            fprintf(f, "[DIAG] Command+P pressed; captured return group = %s\n",
                    returnGroup != nullptr ? returnGroup->getTitle().toRawUTF8() : "nullptr");
            fclose(f);
        }

        AccessibilityHandler::postAnnouncement("Function Settings", AccessibilityHandler::AnnouncementPriority::medium);
        Timer::callAfterDelay(1000, [this, returnGroup] {
            Component *groupIfCapturedNow = findEnclosingNamedGroup(Component::getCurrentlyFocusedComponent(),
                                                                      operators, global, midiKeyboard);
            if ( auto *f = fopen("/tmp/dexed_au_diag.log", "a") ) {
                fprintf(f, "[DIAG] parmShow() about to run; group captured at keypress time = %s; group if captured right now instead = %s\n",
                        returnGroup != nullptr ? returnGroup->getTitle().toRawUTF8() : "nullptr",
                        groupIfCapturedNow != nullptr ? groupIfCapturedNow->getTitle().toRawUTF8() : "nullptr");
                fclose(f);
            }
            parmShow(returnGroup);
        });
        return true;
    }

    if ( key.getKeyCode() == KeyPress::escapeKey ) {
        closeCartridgeLibrary();
        return true;
    }

    return false;
}

// The single place that actually leaves the cartridge library, no matter which control the
// user used to do it: Escape, or any button inside the dialog that closes it. Both paths used
// to go their own way, so only Escape got the "you're back in Operator N" announcement and the
// dialog's own Close button didn't say anything. Routing both through here fixes that for good,
// instead of needing to remember to wrap every future close path by hand.
void DexedAudioProcessorEditor::closeCartridgeLibrary() {
    if ( cartManagerCover.isVisible() && preCartridgeLibraryGroup != nullptr ) {
        Component *returnGroup = preCartridgeLibraryGroup;
        preCartridgeLibraryGroup = nullptr;
        announceGroupThenFocus(*returnGroup, [this] { cartManager.hideCartridgeManager(); });
    } else {
        cartManager.hideCartridgeManager();
    }
}

void DexedAudioProcessorEditor::resetSize() {
    float factor = processor->getZoomFactor();
    setSize(WINDOW_SIZE_X * factor, (processor->showKeyboard ? WINDOW_SIZE_Y : WINDOW_SIZE_Y - 94) * factor);
}