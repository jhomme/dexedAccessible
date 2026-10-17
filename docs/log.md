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

After that, Jim found the algorithm dial itself had no spoken name, so VoiceOver reported it
as unlabeled. Earlier planning had assumed this dial already announced "algorithm," based on
Jim's recollection, but checking the code in GlobalEditor.cpp showed no setTitle call was ever
added for it, unlike its neighboring controls. Added `algo->setTitle("Algorithm")` next to the
file's other accessibility titles. Built, confirmed arm64, and Jim confirmed with VoiceOver
that it now announces a name. Committed and pushed, `9c0e188`.

Started a walkthrough where Jim tabs through the whole plugin with VoiceOver and calls out
each control that announces no name, one at a time, so we can fix them as we go.

Found the mono or poly switch, `monoMode` in GlobalEditor.cpp, had empty button text and no
title, so VoiceOver just said "button" with nothing else. Added
`monoMode->setTitle("Mono Mode")`. Jim confirmed with VoiceOver.

The next unlabeled button was harder to place. Jim described it as sitting in VoiceOver's
order between the "Global Parameters" group and the "Keyboard keys" group, and said pressing
it seemed to move something to the right of the keyboard, then pressing it again moved it
back, with nothing new appearing on screen. Reading the source code wasn't enough to identify
it, so we used two system tools instead of guessing further: a screenshot of the running app,
which needed Screen Recording permission for the terminal, and a direct query of the
currently focused accessibility element, using the system's own tool for talking to other
apps' interfaces, which needed Accessibility permission too. Jim turned both on. With Jim
parked on the control in VoiceOver, the query reported a plain button with no title, 12 points
wide and 90 points tall, sitting at the right edge of where the on-screen keyboard is drawn.

That size and position matched the on-screen keyboard's own built-in scroll button, which
JUCE, the framework this plugin is built on, adds automatically at each end of the keyboard to
shift the visible range of notes by an octave. JUCE never gives these two buttons a name. They
also show or hide themselves depending on whether there's anything left to scroll to, which
explains why the control seemed to shift position or vanish when pressed. This is in vendored
JUCE code, not something built for this project, so rather than edit the library itself, named
both buttons from our own code in PluginEditor.cpp, right after the keyboard component is set
up: they're always the keyboard's first two child components, in a fixed order, scroll down
first and scroll up second, so they can be reached and named safely from outside. Built,
confirmed arm64, and Jim confirmed with VoiceOver that both ends of the keyboard now announce
a name, "Scroll Keyboard Down an Octave" and "Scroll Keyboard Up an Octave."

Right after that, Jim raised a new finding: using VoiceOver's interact command on the
"Keyboard keys" area itself said "content is empty," which reads like a bug even though the
individual piano keys genuinely have no separate accessible objects behind them, they're only
painted, not real components, and there's no way to play a note from the computer keyboard
either, only mouse, touch, or a real MIDI keyboard. Jim uses a real MIDI keyboard day to day,
so the goal wasn't to make each key its own accessible object, just to stop the group from
sounding broken.

First tried giving the keyboard area an image role instead of a group role, on the idea that
images don't usually offer an interact command. That changed what VoiceOver called it, "image"
instead of implied group, but Jim correctly pushed back that calling a playable keyboard an
image is its own kind of wrong, and it didn't fix the "content is empty" message anyway, which
kept appearing exactly the same. Reading JUCE's own source for how the interact command decides
what a component contains showed why: it only looks inside a component if that component is
explicitly marked as a focus container, the same flag `Source/GlobalEditor.cpp` and
`Source/OperatorEditor.cpp` already set on themselves. The keyboard never had that flag set, so
it reported empty regardless of role or of what children it actually had.

Reverted the image role attempt and instead set that focus container flag on `midiKeyboard` in
`Source/PluginEditor.cpp`, keeping its default group-like role and adding a plain description,
"Shows which notes are playing. Play notes with a MIDI keyboard or the mouse." Built, confirmed
arm64, ran DexedTests, still passing. Jim confirmed with VoiceOver that interacting with the
keyboard area now lists its two scroll buttons by name instead of reporting empty content.

Jim then checked today's whole batch of fixes in Logic. He first tested old VST3 and AU
builds that hadn't been rebuilt since before any of today's work, so understandably none of
it showed up, which was a useful reminder that building the standalone app doesn't also
rebuild the other formats. Built and installed Dexed_VST3 and Dexed_AU, confirmed both arm64.
One thing worth noting: both of those build targets install themselves straight into
~/Library/Audio/Plug-Ins automatically as part of the build, overwriting whatever was there,
before there was a chance to list existing copies and ask first, the way this project's rules
normally call for. Flagged that to Jim as it happened rather than after the fact. With the
freshly built VST3 and AU in place, Jim confirmed all of today's fixes work the same way in
Logic as they did in the standalone app.

Started a new usability task: how VoiceOver's arrow keys step through the DX7's many 0-to-99
parameters, envelope levels and rates, operator output level and fine frequency, keyboard
scaling, LFO settings, and pitch envelope. Before this, every dial moved by 1 per arrow press
no matter the range, so crossing a 0-to-99 dial took up to 99 presses.

Worked out the real mechanics through testing, not just reading code. VoiceOver's own
increase/decrease signal for a focused, interacted-with slider carries no information about
whether Shift is held, confirmed in both JUCE's and Apple's own accessibility code and by web
search; there's no OS-level spec for a Shift-modified step, so any such behavior is something
each app has to build itself. Jim then confirmed by testing that Shift+Up and Shift+Down
already reached the plugin as ordinary key presses, not through that signal, producing
`DXSlider`'s pre-existing ~10% jump, which proved the mechanism needed for a Shift-aware fine
or coarse step already worked in this plugin. Jim also tested Home and End directly: at first
they did nothing. Apple's own VoiceOver documentation says Home and End, with or without the
VoiceOver modifier, are already claimed by VoiceOver itself to move its own cursor to the top
or bottom of the current area, which looked like a hard wall. Added the Home/End handling in
code anyway rather than taking that as final, and Jim confirmed the plain Home and End keys,
without the VoiceOver modifier, do reach the plugin and jump to the minimum and maximum, so
that assumption turned out to be wrong, or at least incomplete, for this situation.

Final behavior, all in `DXSlider::keyPressed` in `Source/DXComponents.h`, no accessibility
handler changes needed since none of this goes through JUCE's accessibility
increment/decrement action, only ordinary key presses:
- Home jumps to the lowest value, End to the highest, on every slider.
- On 0-to-99 sliders only, detected by range rather than by name since a few existing
  variable names are misleading (`s_egv1..4` hold the envelope rate sliders despite the "v"):
  Shift+Right or Shift+Down jump up to the next multiple of 5, clamped at 99; Shift+Left or
  Shift+Up jump down to the previous multiple of 5, clamped at 0.
- Down now acts like Right (value up) and Up acts like Left (value down), on every slider,
  the opposite of JUCE's default pairing. This was a deliberate choice Jim made after testing,
  not a bug fix, so it's called out here in case it looks backwards later.
- Plain Right/Left are unchanged, they already moved by 1.

Went through several rounds of build, test, and adjust with Jim along the way, including one
false alarm where a value jump looked like a logic bug but turned out to be the keyboard's own
repeat rate firing the same key many times while held. Built, confirmed arm64, ran DexedTests,
still passing. Jim confirmed the full final behavior with VoiceOver in the standalone app.

Jim tested the same build in Logic, recreating the Dexed track so it picked up the fresh VST3
and AU. Arrow-key stepping itself works correctly there, same as standalone. But VoiceOver
does not speak a slider's new value after an arrow-key move, in Logic, where it does in the
standalone app. That ended the session with this unresolved, so the arrow-key-stepping code
and its doc updates went into a git stash rather than being committed.

Next session, read through `DXSlider`, the algorithm dial, and JUCE's own `Slider`
accessibility handler to look for a difference, since the algorithm dial already announces
its own value changes in Logic. Found none: both use the exact same `DXSlider` class and the
same built-in JUCE notification path, with no code difference at all. The one real
difference between the algorithm dial and the plain parameter sliders was a spoken name,
`setTitle("Algorithm")`, which the plain sliders don't have. Drafted a plan to give every
plain parameter slider a title as the fix.

Jim corrected that before any code changed: in Logic, the algorithm dial doesn't speak its
value change either, only its name. So the missing-title theory is wrong, and that draft fix
is set aside. Lesson worth keeping: a "same code, different outcome" comparison between two
controls isn't enough on its own if the actual symptom wasn't double-checked on both of them
first. Re-opened `BOOKMARK.md`'s open question rather than guessing at a new cause right
away.

Jim separately asked for the sliders' spoken names to be made clearer and more consistent,
even where that means renaming ones that already have a name. That's useful, but unrelated to
this bug, so it's deferred until after both the keyboard stepping and the Logic speaking
problem are settled.

Brought the stashed arrow-key-stepping code back into the working tree with `git stash pop`,
resuming where the paused session left off. The stash's own draft of `BOOKMARK.md` conflicted
with a later, more complete draft already sitting uncommitted in the working tree, since both
had edited the same section independently. Backed up the working tree's version to the
scratchpad directory first, reverted `BOOKMARK.md` to the last commit so the pop could apply
cleanly, then hand-merged the two drafts plus today's new findings into the current
`BOOKMARK.md` rather than losing either one.

Next step, still not done: with VoiceOver on, in Logic, check whether `LcdDisplay` or an
envelope display still speaks a live update, the way they were confirmed to earlier. That
tells us whether the silence is specific to sliders or affects the whole plugin's live
announcements in that host right now, before digging into JUCE or plugin code any further.

Jim tested the arrow-key stepping work in Logic, in VST3 and AU: it moves sliders correctly
there, same as standalone. Checked that off in docs/Plan.md.

While testing, Jim found a new, separate problem: a slider occasionally speaks its value
twice for one single action. Confirmed with physical arrow and Shift+arrow keys, not the
VoiceOver modifier keys, pressed slowly, one at a time, always waiting for VoiceOver to
finish first. Not every press. Always the exact same number both times.

Read code looking for a cause. Found a real, deterministic one, but only for some sliders:
`OperatorEditor.cpp`'s and `GlobalEditor.cpp`'s `sliderValueChanged` handlers for the
envelope level and rate sliders, and the global pitch envelope sliders, each call
`envDisplay->notifyAccessibleValueChanged()` or `pitchEnvDisplay->notifyAccessibleValueChanged()`
every time the slider moves, on top of the slider's own normal built-in announcement. Two
separate things tell VoiceOver something changed: the slider itself, and the diagram next to
it describing the whole curve. That's a real, fixable bug, worth its own fix later, but it
doesn't explain `detune`, which Jim confirmed also double-speaks, and which has no second
announcement wired to anything in the code.

Checked other possible causes for `detune` and ruled each out by reading code: the
parameter-to-slider sync in `PluginParam.cpp` always uses `dontSendNotification` when writing
a value back to a slider, so it can't be silently re-firing a second announcement; there's
only one registered global keyboard shortcut handler, in `PluginEditor.cpp`, and it only
reacts to Ctrl-held shortcuts and Escape, never plain or Shift-held arrow keys, so it can't
be double-handling the same key press. Found no code path left to blame.

Since nothing in the code visibly fires twice, but the behavior still happens sometimes,
reading more code without being able to watch it happen has little left to offer. Worked out
a plan instead: a small, switchable log, written to a file, of every keyboard and VoiceOver
action `DXSlider` receives and what happened as a result, so the actual sequence of events can
be read back after Jim reproduces the problem. Three things to log, all additions to
`DXSlider` in `Source/DXComponents.h`, none touching vendored JUCE: the raw key event in
`keyPressed`; VoiceOver's own direct value-setting action, by giving `DXSlider` its own
`createAccessibilityHandler()` override, the same pattern already used for `EnvDisplay`,
`PitchEnvDisplay`, and `LcdDisplay`, with a thin copy of JUCE's own value interface that logs
before handing off to the real one; and the actual value-changed notification, by making
`DXSlider` its own `Slider::Listener`, which fires at the exact point JUCE also tells
VoiceOver the value changed. Switched by whether a fixed marker file exists, checked fresh
each time, no rebuild needed to flip it. Saved as a plan, not yet built:
`~/.claude/plans/i-turned-plan-mode-melodic-kahn.md`. Jim wants to build this together next
time rather than have it done while he's away.

2026-10-17: Built the diagnostic logging plan from last time. Added `Source/AxTrace.h`, one
small header with a single `log` function: it does nothing unless a fixed marker file exists
in the system temp folder, and when it does, appends a timestamped line to a fixed log file
in the same folder. Both files live outside the git repo. I create or delete the marker file
myself with a shell command to flip logging on or off, so Jim never has to find it.

Wired three log calls into `DXSlider`, in `Source/DXComponents.h`, matching the plan: the raw
key event, at the top of `keyPressed`; VoiceOver's own direct value-setting action, through a
new `createAccessibilityHandler()` override on `DXSlider`, a small copy of JUCE's own slider
value interface that logs the requested value before handing it to the real `setValue`; and
the actual value-changed notification, using `Slider`'s own `onValueChange` callback rather
than making `DXSlider` a full `Slider::Listener` as originally planned, since `onValueChange`
fires from the exact same place, right before VoiceOver is told, and is one line instead of a
second inherited interface. A purely internal choice, noted here rather than asked about,
since it changes nothing Jim sees, hears, or does.

Built the standalone app. First build came out x86_64 only: the project's own
`CMakeLists.txt` only asks for both architectures when `CMAKE_BUILD_TYPE` is set to
`Release`, which the documented build command in CLAUDE.md doesn't set, so on this machine it
silently defaulted to x86_64 alone. Reconfigured with `-DCMAKE_OSX_ARCHITECTURES=arm64` and
rebuilt; confirmed arm64 with the `file` command. Worth fixing properly later, so a plain
build on this Mac can't quietly produce an unusable binary again; recorded as an open
question below rather than changed on the spot, since it touches the documented build
command. Ran DexedTests, still passing.

Turned logging on and opened the standalone app for Jim to test: a single, slow, deliberate
arrow-key press on `detune`, repeated a few times since the double-speak is occasional,
waiting for VoiceOver to finish each time before pressing again.

Jim came back the next session to retest. Checking the log file first turned up a problem
with the logging switch itself, not with the slider: the marker file a previous session had
created to turn logging on was sitting in the plain, shared `/tmp` folder, but the plugin
framework's actual temp folder on a Mac is different, a subfolder named after the running
program inside the user's own session temp folder, something confirmed by reading the
framework's own source rather than assumed. The marker was never where the app was looking,
so logging had effectively been off the whole time despite appearing to be on. Also owned up
to a mistake in the same stretch: claimed the marker had been turned back on without actually
running the command to do it. Moved the marker and log to the right folder and cleared the
log so the next test would actually capture something.

Jim retested with logging genuinely on this time, pressing plain arrows, Shift+arrows, Home,
and End on four different sliders, `detune`, `opCoarse`, `opFine`, and `algo`. Reading the
resulting log line by line turned up two things. First, every single key press in the log
produced at most one internal value-changed notification, for all four sliders including
`algo`, so the plugin itself isn't firing its own announcement twice at that point. Second,
Shift+arrow produced no response at all, not even a value change, on `detune` and `algo`,
confirmed as intentional: the multiple-of-5 jump in `DXSlider::keyPressed` only runs for
sliders whose range is exactly 0 to 99, and neither of those two qualifies.

Jim reported what he actually heard: double-speak on exactly every other press, across every
combination of plain arrow, Shift+arrow, Home, and End, on `detune`, `opCoarse`, and `opFine`,
but never once on `algo`. That pointed at something structurally different about `algo`
rather than at the key type or the press pattern. Comparing `algo`'s handling in
`GlobalEditor.cpp` against the other three in `OperatorEditor.cpp` found it: `algo`'s
`sliderValueChanged` branch calls `setTooltip` on itself every time its value changes,
refreshing its own description. `detune`, `opCoarse`, and `opFine`'s branches all call a
function that updates a separate frequency readout label elsewhere in the editor, but none of
them ever call `setTooltip` on themselves, before or after this session. Checked every
`setTooltip` call in both files to be sure nothing else was setting one some other way; found
none for those three sliders.

Tested that as the explanation, not just the theory, by giving `detune` the same kind of
self-refresh: added a `setTooltip` call to its `sliderValueChanged` branch in
`OperatorEditor.cpp`, rewriting itself to "Detune: " plus the current number, with a plus
sign added for positive values, every time the value changes. Built the standalone app,
confirmed arm64. Jim retested `detune` alone, same procedure as before, and the double-speak
was gone. `opCoarse` and `opFine` don't have this yet and almost certainly still double-speak,
since nothing about them changed.
