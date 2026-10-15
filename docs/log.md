# Log

## 2026-10-02
Upgraded vendored JUCE to 8.0.15 and fixed the API breaks that came with it. `cfab624`.

## 2026-10-09
Fixed a crash on any slider or control interaction, introduced by the JUCE upgrade.
`6da8345`. Added a headless regression test suite, DexedTests, to catch this kind of break
automatically going forward. `d969d33`, `ea897f9`.

## 2026-10-10
Merged `fix/parameter-registration-crash` into `accessibility` directly, instead of waiting on
upstream pull request 534 to be reviewed. `477cd03`. Recorded as a deviation, since the plan
had assumed we'd follow upstream's normal review process. Removed a temporary ComboBox
accessibility workaround in `Source/DXComponents.h`, since JUCE 8.0.15 now fixes that same bug
internally, making the workaround redundant. `6f99c9b`. Rewrote CLAUDE.md with the fork's
remotes, branches, full architecture, and the real next steps.

## 2026-10-14
Set up the missing docs folder for this project: prd.md, Plan.md, acceptance.md,
deviations.md, log.md, and postMortem.md. They should have existed from the first session but
didn't, so this entry backfills the history from git log and CLAUDE.md rather than starting
from nothing.

Jim tested the crash fix and ComboBox cleanup in Logic, using VoiceOver, in both VST3 and AU.
Both passed. Marked that task done in Plan.md.

Added accessibility handlers for EnvDisplay, PitchEnvDisplay, and LcdDisplay in
Source/DXComponents.h, following the ProgramLabelAH pattern in Source/ProgramListBox.h. Each
one reports a role of label and a title built from its current values: EnvDisplay and
PitchEnvDisplay describe all four rate and level pairs using the same numbering the DX7 manual
uses, and LcdDisplay reports its current message.

Also wired up live announcements. EnvDisplay and PitchEnvDisplay fire a valueChanged
accessibility event, meaning VoiceOver speaks up again without needing to refocus, at the
eight rate and level sliders in OperatorEditor.cpp and GlobalEditor.cpp that already repainted
these displays. LcdDisplay fires the same event from setSystemMsg, and from
GlobalEditor::setParamMessage, which is the function that updates the LCD readout every time
any parameter changes, so that's the big one for accessibility.

One thing to call out: these displays also repaint on a fast timer during playback, to animate
which envelope segment is currently playing. That's a different code path
(OperatorEditor::updateEnvPos and GlobalEditor::updatePitchPos, called from
PluginEditor::timerCallback), and I deliberately left it alone. Firing an announcement on every
timer tick while a note plays would make VoiceOver talk constantly and the plugin unusable
during playback. The announcements only fire where a slider value actually changes.

Built the standalone target and confirmed the binary is arm64. Ran DexedTests, still passing.

Jim tested the EnvDisplay, PitchEnvDisplay, and LcdDisplay changes with VoiceOver, in the
standalone app. Passed. Marked that task done in Plan.md and acceptance.md. Committed and
pushed, `f15bc98`. Still not tested in VST3 or AU inside Logic.

Looked through the upstream repo, asb2m10/dexed, for accessibility related pull requests and
issues, at Jim's request. Found that pull request 449, "238 accessibility," is already merged
and already part of our codebase, since it's the origin of the ProgramLabelAH pattern we've
been following. Found three things upstream hasn't built yet: issue 515, about a cartridge
load button that's hard for a screen reader user to find, and two open, unmerged pull
requests, 181, keeping keyboard focus on the on-screen keyboard while browsing patches, and
409, octave shift buttons. Added these three as a new Future enhancements section in
docs/prd.md, not on Plan.md yet since they're ideas, not committed tasks.

At Jim's request, posted a comment on each of the three upstream items, issue 515 and pull
requests 181 and 409, saying he maintains this fork and intends to fold each one in as he's
able.
