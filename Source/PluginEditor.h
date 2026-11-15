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
 */

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "OperatorEditor.h"
#include "GlobalEditor.h"
#include "DXComponents.h"
#include "DXLookNFeel.h"
#include "CartManager.h"

//==============================================================================
/**
*/
class DexedAudioProcessorEditor  : public AudioProcessorEditor, public ComboBox::Listener, public Timer,
                                   public FileDragAndDropTarget, public KeyListener {
    MidiKeyboardComponent midiKeyboard;
    OperatorEditor operators[6];
    Colour background;
    CartManager cartManager;
    // This cover is used to disable main window when cart manager is shown
    Component cartManagerCover;

    SharedResourcePointer<DXLookNFeel> lookAndFeel;
    std::unique_ptr<juce::DialogWindow> dexedParameterDialog;
    #ifdef DEXED_EVENT_DEBUG
        FocusLogger focusLogger;
    #endif

    void resetSize();
    bool initialFocusGiven = false;
    int initialFocusTicks = 0;

    // Whichever operator panel, Global Parameters, or the on-screen keyboard had keyboard
    // focus right before the cartridge library, or the Function Settings parameters dialog,
    // opened, so closing either one can announce that panel's name again on the way back out,
    // the same way jumping to it does.
    Component *preCartridgeLibraryGroup = nullptr;
    Component *preParamDialogGroup = nullptr;

    // The panel whose name was most recently spoken, whether by a Command-key jump shortcut
    // or by plain Tab/Shift+Tab moving into it. Shared between the two so neither one repeats
    // what the other just said.
    Component *lastAnnouncedGroup = nullptr;

    // Set by keyPressed for a real Tab or Shift+Tab, right before JUCE's own key handling
    // moves focus in response to it, and cleared again once groupAnnouncer checks it. Lets
    // groupAnnouncer tell a real Tab press apart from every other way focus can change,
    // including VoiceOver exploring by group, which already announces the group's name
    // itself; without this, that got announced a second time. See the comment on
    // keyPressed's handling of KeyPress::tabKey, and on groupAnnouncer's definition, in
    // PluginEditor.cpp.
    bool tabKeyMovedFocus = false;

    // Speaks a panel's name when Tab or Shift+Tab moves focus into it, since JUCE's own Tab
    // handling never does. See the comment on its definition in PluginEditor.cpp.
    std::unique_ptr<juce::FocusChangeListener> groupAnnouncer;

    Component frameComponent;
public:
    DexedAudioProcessor *processor;
    GlobalEditor global;
    
    DexedAudioProcessorEditor (DexedAudioProcessor* ownerFilter);
    ~DexedAudioProcessorEditor();
    virtual void timerCallback() override;

    virtual void paint (Graphics& g) override;
    virtual void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void updateUI();
    void rebuildProgramCombobox();
    void loadCart(File file);
    void saveCart();
    void initProgram();
    void storeProgram();
    void cartShow();
    void closeCartridgeLibrary();
    void parmShow(Component *returnGroupOverride = nullptr);
    void tuningShow();
    void setMainUIAccessible(bool accessible);
    void discoverMidiCC(Ctrl *ctrl);

    static float getLargestScaleFactor();
    void resetZoomFactor();

    virtual bool isInterestedInFileDrag (const StringArray &files) override;
    virtual void filesDropped (const StringArray &files, int x, int y ) override;
    std::unique_ptr<ComponentTraverser> createFocusTraverser() override;

    bool keyPressed(const KeyPress& key, Component* originatingComponent) override;

    static const int WINDOW_SIZE_X = 866;
    static const int WINDOW_SIZE_Y = 674;
};


#endif  // PLUGINEDITOR_H_INCLUDED
