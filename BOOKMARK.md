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

Also cleaned up the project docs: Plan.md had turned into a mix of task list, decision
narrative, and inline test plan, instead of staying a short punch list, so rewrote it to
point back to docs/log.md and docs/deviations.md for detail. Caught docs/acceptance.md up
with "done when" entries for everything that shipped since 2026-10-15. See docs/log.md,
2026-10-31, for the full account. Still deciding, with Jim, what to change about how we work
going forward so this doesn't happen again; see Open questions below.

## What's next
Jim decided, 2026-10-31, that this build is meant for other VoiceOver and screen reader
users too, not just him. That turns "release" into real, tracked work; see the release
checklist added to docs/Plan.md. Working on the first item now: turning on GitHub Actions
for the jhomme/dexedAccessible fork, which has never run there.

After the release checklist:
1. Jim's own VoiceOver pass over the latest round of fixes in Logic, same plan as before: a
   full pass in one host, a spot check in the other, since this project has already found real
   behavior differences between standalone and Logic's AU.
2. No upstream PR planned for now; see Open questions below. Keep developing on
   `accessibility` directly.
3. Track two, after the release is out: branch off `accessibility` and build a standalone
   Dexed user guide, for every user, from the "mixed" and "fully relevant" sections in
   `docs/dx7-manual-hardware-sections.md`, with a glossary covering both DX7-specific terms
   and general synthesis vocabulary for newcomers.

## Settled, 2026-10-31: not pursuing an upstream PR for now
asb2m10 closed PR 534, the earlier crash-fix PR, on 2026-10-31, saying it's "no more
applicable." The reason: upstream merged their own "Dexed 1.1.0 Refactor for JUCE 8" the same
day, which moved the entire source tree from `Source` to `source`, split it into subfolders,
and replaced the parameter-handling code our accessibility fixes are built on. Checked what
reconciling now would take: a real merge attempt produced 20 conflicts, most of them files
that no longer exist upstream at all, not simple line conflicts. Reconciling would mean
redoing the accessibility work by hand against upstream's new layout, not a rebase.

Jim's call: keep developing on this fork for now rather than take that on. Revisit later if
there's ever a natural pause worth spending on it. If that day comes, remember: our docs/
folder and this bookmark file are Jim's own working notes, not something upstream needs, and
they're committed on the `accessibility` branch itself, so a PR cut straight from that branch
would drag them into the diff. The clean path would be a separate branch built just for that
PR, the same way PR 534 only carried the crash fix, not the project's docs/.

Possible future idea, not yet a task: give the output meter and the operator meters a real
live spoken reading, the same pattern already used for `EnvDisplay`, `PitchEnvDisplay`, and
`LcdDisplay`, instead of leaving them hidden from accessibility.

Waiting on Jim's decision: whether to fix the gap in global CLAUDE.md that let
docs/acceptance.md fall behind, add a template project folder under his Claude folder with
the doc skeletons built in, or both. Recommendation given but not yet approved; this touches
files outside this project, so needs his go-ahead before anything changes.
