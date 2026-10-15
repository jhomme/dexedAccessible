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
#include <stdint.h>

//==============================================================================

class EnvDisplay : public Component {
public:
    EnvDisplay();
    uint8_t *pvalues;
    char vPos;
    void paint(Graphics &g);

    void notifyAccessibleValueChanged() {
        if (auto *handler = getAccessibilityHandler())
            handler->notifyAccessibilityEvent(AccessibilityEvent::valueChanged);
    }

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

    void notifyAccessibleValueChanged() {
        if (auto *handler = getAccessibilityHandler())
            handler->notifyAccessibilityEvent(AccessibilityEvent::valueChanged);
    }

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
    void setSystemMsg(String msg);
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

// Simple Slider to make 10 % jumps when shift is pressed on keypress, but more precise with mouse wheel.
class DXSlider : public Slider {
public:
    DXSlider(const String& componentName) : Slider(componentName) {
        setWantsKeyboardFocus(true);
    }

    bool keyPressed(const KeyPress &key) override {
        if ( key.getModifiers().isShiftDown() ) {
            float len = getRange().getLength() * 0.10;
            if (key.getKeyCode() == key.upKey) {
                setValue(getValue() + len);
                return true;
            }
            if (key.getKeyCode() == key.downKey) {
                setValue(getValue() - len);
                return true;
            }
        } else {
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
