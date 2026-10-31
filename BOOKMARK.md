# Bookmark

## Where we are
Jim tested the titles and tooltips work with VoiceOver in Logic and found a real string of
bugs, all now fixed. The big one: `Ctrl::bind()` in `Source/PluginParam.cpp` was silently
overwriting every control's real title with its raw internal DX7 name, after the editor
constructors had already set the right one. Removing that fixed titles across the whole
plugin, not just Operator 1 where Jim first noticed it.

Also fixed: four Operator 1 sliders with leftover placeholder tooltips, the keyboard scaling
curve buttons now show spelled-out words instead of abbreviations like "-LN" (the LCD's own
separate copy of those abbreviations was left alone on purpose, for players who know the
original DX7), a clearer Oscillator Mode tooltip, reworded cutoff and resonance tooltips,
"in half steps" added to the transpose tooltip, the algorithm diagram and all the VU meters
hidden from accessibility since none of their live readings reach VoiceOver right now, missing
tooltips on the keyboard's octave scroll buttons, a real wall added so VoiceOver can't escape
the cartridge manager dialog into the main window behind it (covering the operators, the
global section, and the on-screen keyboard), a real title for the Function Settings dialog
that used to announce as an unnamed "empty group," four buttons in that dialog that used to
say "button" twice, and Function Settings now lands VoiceOver on its first real control, the
pitch bend range up slider, instead of that empty group.

Built and reinstalled both the Standalone app and the AU several times across this round,
confirmed arm64 each time, always asked before overwriting the AU already installed at
`~/Library/Audio/Plug-Ins/Components/Dexed.component`.

## What's next
1. Jim's own VoiceOver pass over this full round of fixes in Logic, same plan as before: a
   full pass in one host, a spot check in the other, since this project has already found real
   behavior differences between standalone and Logic's AU.
2. Once that's clean, open the PR for `accessibility`.
3. After that PR, track two: branch off `accessibility` and build a standalone Dexed user
   guide, for every user, from the "mixed" and "fully relevant" sections in
   `docs/dx7-manual-hardware-sections.md`, with a glossary covering both DX7-specific terms
   and general synthesis vocabulary for newcomers.

## Open questions
Whether to open a pull request to asb2m10/dexed proposing they adopt the accessibility work,
same as the earlier crash-fix PR, or keep it in this fork only. Not a decision to make yet;
revisit once the tooltip work is tested and the PR for `accessibility` is actually ready.

Possible future idea, not yet a task: give the output meter and the operator meters a real
live spoken reading, the same pattern already used for `EnvDisplay`, `PitchEnvDisplay`, and
`LcdDisplay`, instead of leaving them hidden from accessibility.
