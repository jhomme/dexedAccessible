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

## 2026-10-15
Started accessibility work on AlgoDisplay, the algorithm routing diagram in
Source/AlgoDisplay.h and .cpp. Planned the approach with Jim: since the ALGO dial already
announces its own number on focus and on change, the missing piece is the full six operator
breakdown, available on demand rather than spoken automatically, using the help text VoiceOver
reads with VoiceOver plus shift plus H. Confirmed with Jim, using VoiceOver on the portamento
time dial in the parameter dialog, that this plugin's existing tooltip text already reaches
that command correctly.

Before writing the real description, built a small prototype in the ALGO dial's existing value
changed handler in GlobalEditor.cpp, setting its tooltip to a simple "Algorithm n of 32"
string, to test whether the help command reads fresh text each time or only grabs it once.
Jim confirmed with VoiceOver that it updates live, matching whatever number he'd just set, so
the approach is sound. Next: write the real description, using the DX7 engine's own
FmCore::algorithms routing table in Source/msfa/fm_core.h and .cc, so the words can't drift out
of sync with the actual sound.

One surprise along the way: found that portamentoTm, the portamento time dial just mentioned,
has a tooltip but no spoken title, so VoiceOver reports it as unlabeled. Unrelated to this
task, noted as a small separate fix in the plan. Fixed it by adding a setTitle call,
"Portamento Time", alongside this dialog's other accessibility titles in ParamDialog.cpp.

Wrote the real description, AlgoDisplay::describeAlgorithm in AlgoDisplay.cpp, from the DX7
engine's own FmCore::algorithms routing table in Source/msfa/fm_core.h. One wrinkle: that
table is protected, not public, and Source/msfa isn't ours to modify, so added a small,
never instantiated subclass local to AlgoDisplay.cpp that borrows read access to it, rather
than touching the engine file itself. Also found, while working this out, that the table's
array index 0 is operator 6, not operator 1, counting down to index 5 for operator 1, the
reverse of what seemed obvious at first glance. Confirmed this against algorithm 1's known
structure, feedback on operator 6 down to operator 3, operator 2 down to operator 1, before
trusting it for all 32.

Wired the description into the ALGO dial's tooltip, both when the dial itself changes and
whenever OperatorEditor's operator switches change, which already funnels through
GlobalEditor::updateDisplay(). Also added the fix for portamentoTm's missing title, found
earlier.

Built, ran DexedTests, still passing, and confirmed arm64. Jim tested algorithm 1 with
VoiceOver and confirmed the spoken description against the structure worked out ahead of
time: operator 6 has feedback and modulates operator 5, operator 5 modulates operator 4,
operator 4 modulates operator 3, operator 3 is a carrier, operator 2 modulates operator 1,
operator 1 is a carrier. Matched.

Jim also pointed out VoiceOver Utility has its own setting to always speak help tags
automatically. Since this plugin only ever sets the tooltip text and never speaks it itself,
that setting already gives automatic announcement for anyone who wants it, with no code on
our side. Dropped the optional toggle idea from the plan since it's redundant with that.
