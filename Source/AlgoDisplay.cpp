/**
 *
 * Copyright (c) 2014, 2017 Pascal Gauthier.
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

#include "AlgoDisplay.h"
#include "DXLookNFeel.h"
#include "Dexed.h"
#include "msfa/fm_core.h"

namespace {
    // FmCore::algorithms is protected, so this empty subclass, never instantiated, borrows
    // access to it for read only lookups. Source/msfa stays untouched.
    class FmCoreAccess : public FmCore {
    public:
        static const FmAlgorithm &algorithmFor (int algo) { return algorithms[algo]; }
    };
}

AlgoDisplay::AlgoDisplay() {
    static char tmpAlgo = 0;
    algo = &tmpAlgo;
}

void AlgoDisplay::displayOp(Graphics &g, char id, int x, int y, char link, char fb) {
    const int LINE_SZ = 3;
    String t(id);
    bool opOn = opStatus[6-id] == '1';
    
    x *= 25;
    x += 3;
    y *= 21;
    y += 5;
    
    if ( opOn )
        g.setColour(Colours::white);
    else
        g.setColour(DXLookNFeel::roundBackground);
    
    g.drawText(t, x, y, 16, 12, Justification::centred, true);
    
    if ( opOn )
        g.setColour(DXLookNFeel::fillColour);
    else
        g.setColour(DXLookNFeel::roundBackground);
    
    switch(link) {
        case 0 : // LINE DOWN
            g.drawLine(x+8, y+12, x+8, y+21, LINE_SZ);
            break;
        case 1: // ARROW TO RIGHT
            g.drawLine(x+8, y+12, x+8,  y+18, LINE_SZ);
            g.drawLine(x+7, y+18, x+34, y+18, LINE_SZ);
            break;
        case 2: // ARROW TO RIGHT JOIN
            g.drawLine(x+8,  y+12, x+8, y+19, LINE_SZ);
            break;
        case 3: // ARROW TO RIGHT AND DOWN
            g.drawLine(x+8, y+12, x+8, y+21, LINE_SZ);
            g.drawLine(x+7, y+18, x+34, y+18, LINE_SZ);
            g.drawLine(x+34, y+17, x+34, y+21, LINE_SZ);
            break;
        case 4: // ARROW TO RIGHT+LEFT AND DOWN
            g.drawLine(x+8, y+12, x+8, y+21, LINE_SZ);
            g.drawLine(x+7, y+18, x+34, y+18, LINE_SZ);
            g.drawLine(x+34, y+17, x+34, y+21, LINE_SZ);
            g.drawLine(x-17, y+18, x+8, y+18, LINE_SZ);
            g.drawLine(x-17, y+17, x-17, y+21, LINE_SZ);
            break;
        case 6:
            g.drawLine(x+8, y+12, x+8,  y+18, LINE_SZ);
            g.drawLine(x+7, y+18, x+58, y+18, LINE_SZ);
            break;
        case 7: // ARROW TO LEFT
            g.drawLine(x+8,  y+12, x+8, y+19, LINE_SZ);
            g.drawLine(x-17, y+18, x+9, y+18, LINE_SZ);
            break;
    }
    
    switch(fb) {
        case 0:
            break;
        case 1:
            g.drawLine(x+7, y, x+8, y-5, LINE_SZ);
            g.drawLine(x+8, y-4, x+21, y-4, LINE_SZ);
            g.drawLine(x+20, y-4, x+20, y+15, LINE_SZ);
            g.drawLine(x+19, y+15, x+20, y+16, LINE_SZ);
            g.drawLine(x+8, y+15, x+20, y+15, LINE_SZ);
            break;
        case 2: // ALGO 4
            g.drawLine(x+7, y, x+8, y-5, LINE_SZ);
            g.drawLine(x+8, y-4, x+20, y-4, LINE_SZ);
            g.drawLine(x+19, y-4, x+19, y+59, LINE_SZ);
            g.drawLine(x+8, y+58, x+19, y+58, LINE_SZ);
            break;
        case 3: // ALGO 6
            g.drawLine(x+7, y, x+8, y-5, LINE_SZ);
            g.drawLine(x+8, y-4, x+20, y-4, LINE_SZ);
            g.drawLine(x+19, y-4, x+19, y+37, LINE_SZ);
            g.drawLine(x+8, y+36, x+19, y+36, LINE_SZ);
            break;
        case 4:
            g.drawLine(x+7, y, x+8, y-5, LINE_SZ);
            g.drawLine(x+8, y-4, x-4, y-4, LINE_SZ);
            g.drawLine(x-3, y-4, x-3, y+15, LINE_SZ);
            g.drawLine(x-3, y+15, x+8, y+15, LINE_SZ);
            g.drawLine(x+8, y+15, x+8, y+12, LINE_SZ);
            break;
    }
    
}

void AlgoDisplay::paint(Graphics &g) {    
    g.setColour(DXLookNFeel::fillColour);
    g.fillRect(1, 3, 20, 15);
    String n = String(*algo +1);
    
    g.setColour(Colours::white);
    g.drawText(n, 1, 3, 20, 15, Justification::centred, true);
    
    switch(*algo) {
        case 0:
            displayOp(g, 6, 3, 0, 0, 1);
            displayOp(g, 5, 3, 1, 0, 0);
            displayOp(g, 4, 3, 2, 0, 0);
            displayOp(g, 3, 3, 3, 2, 0);
            displayOp(g, 2, 2, 2, 0, 0);
            displayOp(g, 1, 2, 3, 1, 0);
            break;
        case 1:
            displayOp(g, 6, 3, 0, 0, 0);
            displayOp(g, 5, 3, 1, 0, 0);
            displayOp(g, 4, 3, 2, 0, 0);
            displayOp(g, 3, 3, 3, 2, 0);
            displayOp(g, 2, 2, 2, 0, 1);
            displayOp(g, 1, 2, 3, 1, 0);
            break;
        case 2:
            displayOp(g, 6, 3, 1, 0, 1);
            displayOp(g, 5, 3, 2, 0, 0);
            displayOp(g, 4, 3, 3, 2, 0);
            displayOp(g, 3, 2, 1, 0, 0);
            displayOp(g, 2, 2, 2, 0, 0);
            displayOp(g, 1, 2, 3, 1, 0);
            break;
        case 3: //
            displayOp(g, 6, 3, 1, 0, 2);
            displayOp(g, 5, 3, 2, 0, 0);
            displayOp(g, 4, 3, 3, 2, 0);
            displayOp(g, 3, 2, 1, 0, 0);
            displayOp(g, 2, 2, 2, 0, 0);
            displayOp(g, 1, 2, 3, 1, 0);
            break;
        case 4:
            displayOp(g, 6, 4, 2, 0, 1);
            displayOp(g, 5, 4, 3, 2, 0);
            displayOp(g, 4, 3, 2, 0, 0);
            displayOp(g, 3, 3, 3, 1, 0);
            displayOp(g, 2, 2, 2, 0, 0);
            displayOp(g, 1, 2, 3, 1, 0);
            break;
        case 5: //
            displayOp(g, 6, 4, 2, 0, 3);
            displayOp(g, 5, 4, 3, 2, 0);
            displayOp(g, 4, 3, 2, 0, 0);
            displayOp(g, 3, 3, 3, 1, 0);
            displayOp(g, 2, 2, 2, 0, 0);
            displayOp(g, 1, 2, 3, 1, 0);
            break;
        case 6:
            displayOp(g, 6, 4, 1, 0, 1);
            displayOp(g, 5, 4, 2, 7, 0);
            displayOp(g, 4, 3, 2, 0, 0);
            displayOp(g, 3, 3, 3, 2, 0);
            displayOp(g, 2, 2, 2, 0, 0);
            displayOp(g, 1, 2, 3, 1, 0);
            break;
        case 7:
            displayOp(g, 6, 4, 1, 0, 0);
            displayOp(g, 5, 4, 2, 7, 0);
            displayOp(g, 4, 3, 2, 0, 4);
            displayOp(g, 3, 3, 3, 2, 0);
            displayOp(g, 2, 2, 2, 0, 0);
            displayOp(g, 1, 2, 3, 1, 0);
            break;
        case 8:
            displayOp(g, 6, 4, 1, 0, 0);
            displayOp(g, 5, 4, 2, 7, 0);
            displayOp(g, 4, 3, 2, 0, 0);
            displayOp(g, 3, 3, 3, 2, 0);
            displayOp(g, 2, 2, 2, 0, 1);
            displayOp(g, 1, 2, 3, 1, 0);
            break;
        case 9:
            displayOp(g, 6, 2, 2, 0, 0);
            displayOp(g, 5, 1, 2, 1, 0);
            displayOp(g, 4, 2, 3, 1, 0);
            displayOp(g, 3, 3, 1, 0, 1);
            displayOp(g, 2, 3, 2, 0, 0);
            displayOp(g, 1, 3, 3, 2, 0);
            break;
        case 10:
            displayOp(g, 6, 2, 2, 0, 1);
            displayOp(g, 5, 1, 2, 1, 0);
            displayOp(g, 4, 2, 3, 1, 0);
            displayOp(g, 3, 3, 1, 0, 0);
            displayOp(g, 2, 3, 2, 0, 0);
            displayOp(g, 1, 3, 3, 2, 0);
            break;
        case 11:
            displayOp(g, 6, 3, 2, 7, 0);
            displayOp(g, 5, 2, 2, 0, 0);
            displayOp(g, 4, 1, 2, 1, 0);
            displayOp(g, 3, 2, 3, 6, 0);
            displayOp(g, 2, 4, 2, 0, 1);
            displayOp(g, 1, 4, 3, 2, 0);
            break;
        case 12:
            displayOp(g, 6, 3, 2, 7, 1);
            displayOp(g, 5, 2, 2, 0, 0);
            displayOp(g, 4, 1, 2, 1, 0);
            displayOp(g, 3, 2, 3, 6, 0);
            displayOp(g, 2, 4, 2, 0, 0);
            displayOp(g, 1, 4, 3, 2, 0);
            break;
        case 13:                                                                       
            displayOp(g, 6, 4, 1, 7, 1);// displayOp(g, 6, 3, 1, 0, 1);
            displayOp(g, 5, 3, 1, 0, 0);// displayOp(g, 5, 2, 1, 1, 0);
            displayOp(g, 4, 3, 2, 0, 0);
            displayOp(g, 3, 3, 3, 2, 0);
            displayOp(g, 2, 2, 2, 0, 0);
            displayOp(g, 1, 2, 3, 1, 0);
            break;
        case 14:
            displayOp(g, 6, 4, 1, 7, 0);// displayOp(g, 6, 3, 1, 0, 0);
            displayOp(g, 5, 3, 1, 0, 0);// displayOp(g, 5, 2, 1, 1, 0);
            displayOp(g, 4, 3, 2, 0, 0);
            displayOp(g, 3, 3, 3, 2, 0);
            displayOp(g, 2, 2, 2, 0, 4);
            displayOp(g, 1, 2, 3, 1, 0);
            break;
        case 15:
            displayOp(g, 6, 4, 1, 0, 1);
            displayOp(g, 5, 4, 2, 7, 0);
            displayOp(g, 4, 3, 1, 0, 0);
            displayOp(g, 3, 3, 2, 0, 0);
            displayOp(g, 2, 2, 2, 1, 0);
            displayOp(g, 1, 3, 3, 0, 0);
            break;
        case 16:
            displayOp(g, 6, 4, 1, 0, 0);
            displayOp(g, 5, 4, 2, 7, 0);
            displayOp(g, 4, 3, 1, 0, 0);
            displayOp(g, 3, 3, 2, 0, 0);
            displayOp(g, 2, 2, 2, 1, 4);
            displayOp(g, 1, 3, 3, 0, 0);
            break;
        case 17:
            displayOp(g, 6, 4, 0, 0, 0);
            displayOp(g, 5, 4, 1, 0, 0);
            displayOp(g, 4, 4, 2, 7, 0);
            displayOp(g, 3, 3, 2, 0, 4);
            displayOp(g, 2, 2, 2, 1, 0);
            displayOp(g, 1, 3, 3, 0, 0);
            break;
        case 18:
            displayOp(g, 6, 3, 2, 3, 1);
            displayOp(g, 5, 4, 3, 2, 0);
            displayOp(g, 4, 3, 3, 1, 0);
            displayOp(g, 3, 2, 1, 0, 0);
            displayOp(g, 2, 2, 2, 0, 0);
            displayOp(g, 1, 2, 3, 1, 0);
            break;
        case 19:
            displayOp(g, 6, 4, 2, 0, 0);
            displayOp(g, 5, 3, 2, 1, 0);
            displayOp(g, 4, 4, 3, 2, 0);
            displayOp(g, 3, 1, 2, 3, 1);
            displayOp(g, 2, 2, 3, 6, 0);
            displayOp(g, 1, 1, 3, 1, 0);
            break;
        case 20:
            displayOp(g, 6, 3, 2, 3, 0);
            displayOp(g, 5, 4, 3, 2, 0);
            displayOp(g, 4, 3, 3, 1, 0);
            displayOp(g, 3, 1, 2, 3, 1);
            displayOp(g, 2, 2, 3, 1, 0);
            displayOp(g, 1, 1, 3, 1, 0);
            break;
        case 21:
            displayOp(g, 6, 3, 2, 4, 1);
            displayOp(g, 5, 4, 3, 2, 0);
            displayOp(g, 4, 3, 3, 1, 0);
            displayOp(g, 3, 2, 3, 1, 0);
            displayOp(g, 2, 1, 2, 0, 0);
            displayOp(g, 1, 1, 3, 1, 0);
            break;
        case 22: // CC
            displayOp(g, 6, 3, 2, 3, 1);
            displayOp(g, 5, 4, 3, 2, 0);
            displayOp(g, 4, 3, 3, 1, 0);
            displayOp(g, 3, 2, 2, 0, 0);
            displayOp(g, 2, 2, 3, 1, 0);
            displayOp(g, 1, 1, 3, 1, 0);
            break;
        case 23: // CC
            displayOp(g, 6, 3, 2, 4, 1);
            displayOp(g, 5, 4, 3, 2, 0);
            displayOp(g, 4, 3, 3, 1, 0);
            displayOp(g, 3, 2, 3, 1, 0);
            displayOp(g, 2, 1, 3, 1, 0);
            displayOp(g, 1, 0, 3, 1, 0);
            break;
        case 24: // CC
            displayOp(g, 6, 3, 2, 3, 1);
            displayOp(g, 5, 4, 3, 2, 0);
            displayOp(g, 4, 3, 3, 1, 0);
            displayOp(g, 3, 2, 3, 1, 0);
            displayOp(g, 2, 1, 3, 1, 0);
            displayOp(g, 1, 0, 3, 1, 0);
            break;
        case 25:
            displayOp(g, 6, 4, 2, 0, 1);
            displayOp(g, 5, 3, 2, 1, 0);
            displayOp(g, 4, 4, 3, 2, 0);
            displayOp(g, 3, 2, 2, 0, 0);
            displayOp(g, 2, 2, 3, 6, 0);
            displayOp(g, 1, 1, 3, 1, 0);
            break;
        case 26:
            displayOp(g, 6, 4, 2, 0, 0);
            displayOp(g, 5, 3, 2, 1, 0);
            displayOp(g, 4, 4, 3, 2, 0);
            displayOp(g, 3, 2, 2, 0, 1);
            displayOp(g, 2, 2, 3, 6, 0);
            displayOp(g, 1, 1, 3, 1, 0);
            break;
        case 27:
            displayOp(g, 6, 4, 3, 2, 0);
            displayOp(g, 5, 3, 1, 0, 1);
            displayOp(g, 4, 3, 2, 0, 0);
            displayOp(g, 3, 3, 3, 1, 0);
            displayOp(g, 2, 2, 2, 0, 0);
            displayOp(g, 1, 2, 3, 1, 0);
            break;
        case 28:
            displayOp(g, 6, 4, 2, 0, 1);
            displayOp(g, 5, 4, 3, 2, 0);
            displayOp(g, 4, 3, 2, 0, 0);
            displayOp(g, 3, 3, 3, 1, 0);
            displayOp(g, 2, 2, 3, 1, 0);
            displayOp(g, 1, 1, 3, 1, 0);
            break;
        case 29:
            displayOp(g, 6, 4, 3, 2, 0);
            displayOp(g, 5, 3, 1, 0, 1);
            displayOp(g, 4, 3, 2, 0, 0);
            displayOp(g, 3, 3, 3, 1, 0);
            displayOp(g, 2, 2, 3, 1, 0);
            displayOp(g, 1, 1, 3, 1, 0);
            break;
        case 30:
            displayOp(g, 6, 4, 2, 0, 1);
            displayOp(g, 5, 4, 3, 2, 0);
            displayOp(g, 4, 3, 3, 1, 0);
            displayOp(g, 3, 2, 3, 1, 0);
            displayOp(g, 2, 1, 3, 1, 0);
            displayOp(g, 1, 0, 3, 1, 0);
            break;
        case 31:
            displayOp(g, 6, 5, 3, 2, 1);
            displayOp(g, 5, 4, 3, 1, 0);
            displayOp(g, 4, 3, 3, 1, 0);
            displayOp(g, 3, 2, 3, 1, 0);
            displayOp(g, 2, 1, 3, 1, 0);
            displayOp(g, 1, 0, 3, 1, 0);       
            break;
        default:
            break;
    }
}

String AlgoDisplay::describeAlgorithm (int algo, const char *opStatus) {
    const FmAlgorithm &alg = FmCoreAccess::algorithmFor (algo);

    // Index i here is operator (6 - i), matching this engine's own convention: array index 0
    // is operator 6, index 5 is operator 1. Confirmed against algorithm 1's known routing,
    // operator 6 with feedback down to operator 3, operator 2 down to operator 1, and against
    // how opStatus is already indexed elsewhere in this class.
    int lastWriter[3] = { -1, -1, -1 };
    int parentIndex[6];

    for (int i = 0; i < 6; ++i) {
        parentIndex[i] = -1;
        int flags = alg.ops[i];
        int inBus = (flags >> 4) & 3;
        int outBus = flags & 3;

        if (inBus != 0 && lastWriter[inBus] != -1)
            parentIndex[i] = lastWriter[inBus];

        if (outBus != 0)
            lastWriter[outBus] = i;
    }

    String text;
    text << "Algorithm " << (algo + 1) << " of 32. ";

    for (int i = 0; i < 6; ++i) {
        int opNumber = 6 - i;
        text << "Operator " << opNumber;

        if (opStatus[i] == '0')
            text << ", muted,";

        bool hasFeedback = (alg.ops[i] & (FB_IN | FB_OUT)) == (FB_IN | FB_OUT);
        if (hasFeedback)
            text << " has feedback and";

        StringArray children;
        for (int j = 0; j < 6; ++j)
            if (parentIndex[j] == i)
                children.add (String (6 - j));

        bool carrier = FmCore::isCarrier (algo, i);

        if (children.size() > 0 && carrier)
            text << " modulates operator " << children.joinIntoString (" and ") << " and is a carrier.";
        else if (children.size() > 0)
            text << " modulates operator " << children.joinIntoString (" and ") << ".";
        else if (carrier)
            text << " is a carrier.";
        else
            text << " produces no direct output.";

        text << " ";
    }

    return text.trimEnd();
}

