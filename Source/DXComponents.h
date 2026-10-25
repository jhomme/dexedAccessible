/**
 *
 * Copyright (c) 2014-2024 Pascal Gauthier.
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

#ifndef DXCOMPONENTS_H_INCLUDED
#define DXCOMPONENTS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "AxTrace.h"
#include <stdint.h>

//==============================================================================

class EnvDisplay : public Component {
public:
    EnvDisplay();
    uint8_t *pvalues;
    char vPos;
    void paint(Graphics &g);

    struct EnvDisplayAH : public juce::AccessibilityHandler {
        explicit EnvDisplayAH(EnvDisplay *s): display(s), juce::AccessibilityHandler(*s, juce::AccessibilityRole::label) {
        }

        virtual String getTitle() const override {
            uint8_t *rates = display->pvalues;
            uint8_t *levels = display->pvalues + 4;
            String title("Envelope");
            for (int i = 0; i < 4; i++)
                title << ", rate " << (i + 1) << " " << (int) rates[i] << ", level " << (i + 1) << " " << (int) levels[i];
            return title;
        }

        EnvDisplay *display;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvDisplayAH);
    };

    std::unique_ptr<AccessibilityHandler> createAccessibilityHandler() override {
        return std::make_unique<EnvDisplayAH>(this);
    }
};

class PitchEnvDisplay : public Component {
    char rvalues[8];
public:
    PitchEnvDisplay();
    uint8_t *pvalues;
    char vPos;
    void paint(Graphics &g);

    struct PitchEnvDisplayAH : public juce::AccessibilityHandler {
        explicit PitchEnvDisplayAH(PitchEnvDisplay *s): display(s), juce::AccessibilityHandler(*s, juce::AccessibilityRole::label) {
        }

        virtual String getTitle() const override {
            uint8_t *rates = display->pvalues;
            uint8_t *levels = display->pvalues + 4;
            String title("Pitch envelope");
            for (int i = 0; i < 4; i++)
                title << ", rate " << (i + 1) << " " << (int) rates[i] << ", level " << (i + 1) << " " << (int) levels[i];
            return title;
        }

        PitchEnvDisplay *display;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PitchEnvDisplayAH);
    };

    std::unique_ptr<AccessibilityHandler> createAccessibilityHandler() override {
        return std::make_unique<PitchEnvDisplayAH>(this);
    }
};

class LcdDisplay : public Component {
public:
    LcdDisplay();
    void setSystemMsg(String msg, bool notify = true);
    String paramMsg;
    void paint(Graphics &g);

    void notifyAccessibleValueChanged() {
        if (auto *handler = getAccessibilityHandler())
            handler->notifyAccessibilityEvent(AccessibilityEvent::valueChanged);
    }

    struct LcdDisplayAH : public juce::AccessibilityHandler {
        explicit LcdDisplayAH(LcdDisplay *s): display(s), juce::AccessibilityHandler(*s, juce::AccessibilityRole::label) {
        }

        virtual String getTitle() const override {
            return display->paramMsg;
        }

        LcdDisplay *display;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LcdDisplayAH);
    };

    std::unique_ptr<AccessibilityHandler> createAccessibilityHandler() override {
        return std::make_unique<LcdDisplayAH>(this);
    }
};

class ComboBoxImage : public ComboBox {
    Image items;
    int itemHeight;
    PopupMenu popup;
    int itemPos[4];
public:
    ComboBoxImage();

    virtual void paint(Graphics &g) override;
    virtual void showPopup() override;
    void setImage(Image image);
    void setImage(Image image, int pos[]);
};

class ProgramSelector : public ComboBox {
    float accum_wheel;
public:
    ProgramSelector() {
        setWantsKeyboardFocus(true);
        setTitle("Program selector");
    }
    virtual void mouseDown(const MouseEvent &event) override;
    virtual void mouseEnter(const MouseEvent &event) override { accum_wheel = 0; }
    virtual void mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) override;
    virtual void paint(Graphics &g) override;
};

// Slider that jumps to the next or previous multiple of 5 on Shift+Right/Left, for 0-99
// parameters only, and is more precise with the mouse wheel when Shift is held.
class DXSlider : public Slider {
public:
    DXSlider(const String& componentName) : Slider(componentName) {
        setWantsKeyboardFocus(true);

        // "The result": fires once, from inside JUCE's own code, at the exact point where it
        // also tells VoiceOver the value changed, whatever caused the change. Comparing how
        // many of these lines show up for one single key press or VoiceOver gesture tells us
        // whether this plugin is announcing a value twice, or whether that's happening
        // somewhere outside it.
        onValueChange = [this] {
            AxTrace::log(getName() + ": value changed to " + String(getValue()));
        };
    }

    bool keyPressed(const KeyPress &key) override {
        // "The action", for an ordinary physical key press, Shift held or not. A VoiceOver
        // gesture mostly does not arrive here at all; see DXSliderAH below for that path.
        AxTrace::log(getName() + ": key pressed, code " + String(key.getKeyCode())
                     + ", shift " + String((int) key.getModifiers().isShiftDown())
                     + ", value before " + String(getValue()));

        // Home and End jump straight to the lowest and highest value, on every slider.
        if (key.getKeyCode() == key.homeKey) {
            setValue(getRange().getStart());
            return true;
        }
        if (key.getKeyCode() == key.endKey) {
            setValue(getRange().getEnd());
            return true;
        }

        if ( key.getModifiers().isShiftDown() ) {
            auto range = getRange();

            // Shift+Right or Shift+Down add 5 to the current value, and Shift+Left or
            // Shift+Up subtract 5, clamped at the slider's own min and max, on every slider
            // in the app, so VoiceOver users can cross a range in a handful of presses
            // instead of one value at a time. Down pairs with Right, and Up pairs with Left,
            // matching the plain-arrow mapping below.
            if (key.getKeyCode() == key.rightKey || key.getKeyCode() == key.downKey) {
                setValue(jmin(range.getEnd(), getValue() + 5.0));
                return true;
            }
            if (key.getKeyCode() == key.leftKey || key.getKeyCode() == key.upKey) {
                setValue(jmax(range.getStart(), getValue() - 5.0));
                return true;
            }
        } else {
            // Down moves the value up and Up moves it down, the opposite of JUCE's default
            // pairing, per Jim's preference. Reuse the default handling by swapping which
            // key code we hand to it, rather than duplicating its step logic here.
            if (key.getKeyCode() == key.upKey || key.getKeyCode() == key.downKey) {
                KeyPress swapped (key.getKeyCode() == key.upKey ? key.downKey : key.upKey,
                                   key.getModifiers(), key.getTextCharacter());
                return Slider::keyPressed(swapped);
            }
            return Slider::keyPressed(key);
        }
        return false;
    }

    void mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) override {
        auto newWheel = wheel;
        const auto speed = event.mods.isShiftDown() ? 0.05 : 1.0;
        newWheel.deltaY *= speed;
        Slider::mouseWheelMove(event, newWheel);
    }

    // A VoiceOver gesture on a slider, like VO+Right or VO+Shift+Down, mostly skips
    // keyPressed entirely: VoiceOver asks the accessibility handler to set a new value
    // directly instead. JUCE's own handler for that, inside Slider.cpp, isn't ours to log
    // from, so this is our own copy of it, logging "the action" for a VoiceOver gesture
    // specifically, the moment before the new value reaches the slider.
    struct DXSliderAH : public juce::AccessibilityHandler {
        explicit DXSliderAH(DXSlider *s)
            : slider(s),
              juce::AccessibilityHandler(*s, juce::AccessibilityRole::slider,
                                          juce::AccessibilityActions{},
                                          juce::AccessibilityHandler::Interfaces { std::make_unique<ValueInterface>(s) }) {
        }

        String getHelp() const override { return slider->getTooltip(); }

        class ValueInterface : public juce::AccessibilityRangedNumericValueInterface {
        public:
            explicit ValueInterface(DXSlider *s) : slider(s) {}

            bool isReadOnly() const override { return false; }
            double getCurrentValue() const override { return slider->getValue(); }

            void setValue(double newValue) override {
                AxTrace::log(slider->getName() + ": VoiceOver action requests value " + String(newValue)
                             + ", value before " + String(slider->getValue()));
                Slider::ScopedDragNotification drag(*slider);
                slider->setValue(newValue, sendNotificationSync);
            }

            juce::AccessibilityValueInterface::AccessibleValueRange getRange() const override {
                auto range = slider->getRange();
                auto interval = slider->getInterval();
                return { { range.getStart(), range.getEnd() },
                         interval > 0.0 ? interval : range.getLength() * 0.01 };
            }

        private:
            DXSlider *slider;
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueInterface);
        };

        DXSlider *slider;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DXSliderAH);
    };

    std::unique_ptr<AccessibilityHandler> createAccessibilityHandler() override {
        return std::make_unique<DXSliderAH>(this);
    }
};

class FocusLogger final : public juce::FocusChangeListener
{
public:
    FocusLogger ()
    {
        juce::Desktop::getInstance ().addFocusChangeListener (this);
    }

    ~FocusLogger () override
    {
        juce::Desktop::getInstance ().removeFocusChangeListener (this);
    }

    void globalFocusChanged (juce::Component * focusedComponent) override
    {
        if (focusedComponent == nullptr)
            return;

        DBG ("Component title: " << focusedComponent->getTitle ());
        DBG ("Component type: " << typeid (*focusedComponent).name ());
        DBG ("---");
    }
};

#endif  // DXCOMPONENTS_H_INCLUDED
