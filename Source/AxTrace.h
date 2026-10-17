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

#ifndef AXTRACE_H_INCLUDED
#define AXTRACE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

// A diagnostic log for watching exactly what a slider does in response to a key press or a
// VoiceOver accessibility action. Off by default. Turned on or off by creating or deleting a
// marker file, so the switch works instantly, in a running standalone, VST3, or AU instance,
// without a rebuild or a restart. Both files live in the system temp directory, outside the
// git repository, so there's no risk of either one being committed.
namespace AxTrace {
    inline File markerFile() {
        return File::getSpecialLocation(File::tempDirectory).getChildFile("dexed-ax-trace.on");
    }

    inline File logFile() {
        return File::getSpecialLocation(File::tempDirectory).getChildFile("dexed-ax-trace.log");
    }

    inline void log(const String &message) {
        if (!markerFile().existsAsFile())
            return;

        logFile().appendText(Time::getCurrentTime().toString(false, true, true, true) + "  " + message + "\n");
    }
}

#endif  // AXTRACE_H_INCLUDED
