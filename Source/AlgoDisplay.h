/**
 *
 * Copyright (c) 2014-2017 Pascal Gauthier.
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

#ifndef ALGODISPLAY_H_INCLUDED
#define ALGODISPLAY_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class AlgoDisplay : public Component {
    void displayOp(Graphics &g, char id, int x, int y, char link, char fb);
public:
    const char *opStatus;
    AlgoDisplay();
    char *algo;
    void paint(Graphics &g);

    // Describes the routing of the given algorithm (0-31) and operator on/off flags
    // (opStatus, index 0 = operator 6 through index 5 = operator 1, same ordering as this
    // class's own opStatus member) in words, for VoiceOver. Built from the DX7 engine's own
    // routing table in Source/msfa/fm_core.h, so the words can't drift out of sync with the
    // actual sound.
    static String describeAlgorithm (int algo, const char *opStatus);

    void notifyAccessibleValueChanged() {
        if (auto *handler = getAccessibilityHandler())
            handler->notifyAccessibilityEvent(AccessibilityEvent::valueChanged);
    }

    struct AlgoDisplayAH : public juce::AccessibilityHandler {
        explicit AlgoDisplayAH(AlgoDisplay *s): display(s), juce::AccessibilityHandler(*s, juce::AccessibilityRole::label) {
        }

        virtual String getTitle() const override {
            return AlgoDisplay::describeAlgorithm(*display->algo, display->opStatus);
        }

        AlgoDisplay *display;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AlgoDisplayAH);
    };

    std::unique_ptr<AccessibilityHandler> createAccessibilityHandler() override {
        return std::make_unique<AlgoDisplayAH>(this);
    }
};

#endif  // ALGODISPLAY_H_INCLUDED
