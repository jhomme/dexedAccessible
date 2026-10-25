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

## 2026-10-18
Built fresh VST3 and AU, confirmed arm64, overwriting the Oct 15 copies in
`~/Library/Audio/Plug-Ins`, so Jim could test the `detune` fix in Logic. In Logic, VoiceOver
still doesn't speak slider value changes live when moving a slider with arrow keys, the same
silence noted on 2026-10-17. Since nothing speaks at all, there's no way to tell from that
alone whether the every-other-press double-speak is also fixed in Logic.

Jim then tested `LcdDisplay` directly, to see whether the Logic silence is specific to sliders
or wider than that. Found something that narrows it down: the LCD's text does update when a
slider moves with arrow keys, confirmed by checking it afterward, but VoiceOver speaks nothing
in the moment. When Jim instead moves VoiceOver's own cursor with VO+Left or VO+Right onto the
LCD, after the fact, VoiceOver does speak its text, and the same is true moving onto a slider
that way. So the content and the accessible description are both correct in Logic; what's
missing is specifically the live, unprompted announcement that fires on its own when a value
changes, the kind that already works in the standalone app. Points at the live value-changed
accessibility event not reaching VoiceOver in Logic, not at anything wrong with the displays.

Settled a plan with Jim for the every-other-press double-speak: give every slider except
`algo` a tooltip that rebuilds to its name and current value on every change, the same pattern
now used for `detune`. Before writing that fix everywhere, ran a free test using tooltips that
already exist and never update: `sclLeftLevel`, `sclRightLevel`, `sclLvlBrkPt`, and
`sclRateScaling` in `Source/OperatorEditor.cpp`. Jim tested all four with VoiceOver. Three
double-speak, `sclLeftLevel`, `sclLvlBrkPt`, and `sclRateScaling`; one does not,
`sclRightLevel`. `sclLeftLevel` and `sclRightLevel` are set up identically in code, same range,
same dial style, same kind of fixed tooltip, same empty value-changed handler, differing only
in tooltip wording, screen position, and which operator parameter each is wired to, so nothing
in the code explains the difference. This weakens the idea that tooltip content alone is what
matters, and the broad name-and-value tooltip fix was set aside rather than written across all
~36 sliders until the diagnostic log can point at a real cause. Decided, going forward, to
always build with the diagnostic log turned on, so a test like this one isn't done blind.

## 2026-10-19
Rebuilt standalone, VST3, AU, and CLAP, all confirmed arm64. Checked existing copies in
`~/Library/Audio/Plug-Ins` and `/Applications` first and got Jim's go-ahead before the build
could overwrite anything, per this project's rules, though it turned out nothing had actually
changed since the Oct 18 build, so none of the four were overwritten with new content. Missed
checking the CLAP folder beforehand, flagged that to Jim after the fact rather than before.
Ran DexedTests, still passing.

Turned the diagnostic log on for both hosts Jim will test next: the standalone app, using the
same marker location confirmed working before, `$TMPDIR/Dexed/dexed-ax-trace.on`; and Logic,
a location not tried before, `$TMPDIR/Logic Pro/dexed-ax-trace.on`, named after Logic's own
executable since the plugin runs inside Logic's process rather than its own. Worked out the
Logic path by reading JUCE's `File::getSpecialLocation` source for macOS rather than guessing:
it's always `$TMPDIR/<executable name>`. Jim will test both later; still need to check whether
Logic actually wrote to its log file afterward; that confirms or rules out the guess about its
temp folder name.

Jim also asked why BOOKMARK.md had grown so large. Found the cause: its "Where we are"
section was never pruned after a task finished, so every session's detail kept accumulating
on top of the last, duplicating a lot of what already lives here in docs/log.md. Jim asked to
prune it, keeping this log as the full record and BOOKMARK.md as a short, current-state
recap.

## 2026-10-21
Found the standalone app's diagnostic marker and log files gone, the `$TMPDIR/Dexed` folder
was empty, no `.on` file and no `.log` file, even though both were working as of the last
session. Most likely cause: macOS clears that per-user temp folder between runs or reboots.
Recreated the marker file and confirmed logging worked again by having Jim reproduce the
double-speak and reading the fresh log afterward.

That first fresh log, covering `detune`, `opCoarse`, and `opFine`, both by arrow key and by
VoiceOver's own VO+Left/Right gesture, showed exactly one "value changed" line per action,
every time. That's the one line in JUCE's own `Slider.cpp` that tells VoiceOver about a
slider's new value, so this ruled out the plugin firing that notification twice for a single
press.

Jim pointed out VO+Left/Right never double-speaks, only plain arrow keys do, and asked what
else could be competing. Added the same diagnostic logging to `LcdDisplay::setSystemMsg`,
`Source/DXComponents.cpp`, and rebuilt standalone. The next log showed the LCD firing its own
live announcement at the same moment as the slider's, for every single parameter edit, arrow
key or VoiceOver gesture alike. The small LCD text at the bottom of the window updates on
practically every parameter change, and had been separately telling VoiceOver about its own
change this whole time, competing with the slider's own announcement.

Jim separately tested and found the very first arrow-key press on a freshly focused slider
sounds like one voice, and every press after that sounds like two, with the second cutting
off the first. Traced JUCE's `Slider.cpp` to check whether the two input paths, a plain arrow
key and a VoiceOver gesture, differ in our own code. They don't: both end up at the exact
same line, `handleAsyncUpdate()`, which fires the slider's notification exactly once either
way. So the difference in what gets heard isn't in the plugin's code, it has to be in how
VoiceOver itself treats a notification that results from its own gesture versus one from a
plain keystroke. Best guess, not provable from inside the plugin: VoiceOver quiets other
competing announcements while it's in the middle of its own adjust gesture, since it already
plans to speak the result itself, but doesn't do that for an ordinary key press. Parked as a
side issue for later, out of curiosity, not blocking the fix below.

Fixed the LCD/slider double announcement: `GlobalEditor::setParamMessage`, the path called on
every ordinary parameter edit, now passes `notify = false` into `LcdDisplay::setSystemMsg`,
so the LCD updates its visible text without telling VoiceOver about it, since the slider
already does that. `GlobalEditor::setSystemMessage`, a path reserved for real system messages
like a patch loading, with no slider of its own, keeps announcing live; it isn't called from
anywhere yet, but is there for when that need comes up. Rebuilt standalone, confirmed arm64.

Jim tested and reported it wasn't solved, but noticed `opFine` specifically had stopped
double-speaking. Turned out the diagnostic log couldn't tell the difference: the log line in
`LcdDisplay::setSystemMsg` printed "value changed" unconditionally, whether or not the real
VoiceOver notification fired, so the log looked identical whether the fix was active or not.
Also worth checking in hindsight: a running standalone instance keeps the old code in memory
until it's quit and relaunched, not just rebuilt on disk. Changed the log line to print
`notify 0` or `notify 1` so it actually shows what happened, rebuilt, and had Jim fully quit
and reopen the app before retesting. The fresh log showed `notify 0` on every parameter edit
for `detune`, `opCoarse`, and `opFine`, confirming the fix was active, and Jim confirmed with
VoiceOver that all three spoke only once now, including `opFine`.

Rebuilt VST3, AU, and CLAP with the same fix. First listed every existing Dexed copy in
`/Applications`, `~/Library/Audio/Plug-Ins`, and `/Library/Audio/Plug-Ins`, per this project's
rules: only the three user-level copies, VST3, AU, and CLAP, all already arm64, were ours and
would be overwritten; the root-owned copies in `/Library/Audio/Plug-Ins` are a separate, older
install this build never touches. Built, confirmed all three installed copies arm64.

Turning the diagnostic log on for Logic took an extra step this time. Jim loaded the AU in
Logic; its temp folder, where the log and its on/off marker file live, didn't exist yet,
since nothing had called into the plugin's code there yet. Had Jim press one throwaway key to
trigger it. Still nothing appeared at the location used last time, `$TMPDIR/Logic Pro`.
Checked `~/Library/Containers` for a sandbox container for Logic's own bundle ID,
`com.apple.logic10`; none existed, meaning Logic itself isn't sandboxed. Found the real folder
by listing everything under the shared per-user temp root and spotting `AUHostingService`,
Apple's own out-of-process host for Audio Unit plugins; Logic runs the AU in that separate
helper process rather than its own, so the plugin's code sees that helper's executable name,
not Logic's, and its own temp folder landed at `$TMPDIR/AUHostingService/Dexed`. Created the
marker file there.

Jim's first retest produced a confusing result: VoiceOver's own gesture, VO+Left/Right,
changed `detune`'s value and logged normally, but plain arrow keys did nothing, no log line of
any kind, not even the raw key-press line that should fire regardless of whether a value
actually changes. Guessed this might be a systemic difference between standalone and an
out-of-process AU host, keyboard focus not following VoiceOver's cursor across the process
boundary the way it does in-process. That guess didn't hold up: `docs/Plan.md` and this log
both already recorded arrow-key stepping working fine in Logic's AU on 2026-10-17, so this
wasn't a standing bug.

Had Jim retest more carefully, confirming he wasn't "interacting" with the slider through
VoiceOver, VO+Shift+Up to back out first, and confirming VoiceOver's Quick Nav, which can
claim plain arrow keys system-wide, was off. Neither changed anything, arrow keys still did
nothing, confirmed by Jim directly: the value itself wasn't moving, not just failing to
announce. Then, on a later retest, arrow keys suddenly started working normally, logging and
changing values correctly, LCD still correctly quiet per the fix. Jim separately realized his
usual way of reaching the very first control, VO+Right from the window's close button, assumes
a close button that standalone has and Logic's plugin window doesn't, so his early attempts in
Logic may not have landed real keyboard focus on `detune` the way he intended. That's the most
likely explanation for the whole episode, a one-time navigation mix-up right when the plugin
window first opened, not a defect in the plugin, though it couldn't be reproduced again to
confirm for certain. Jim confirmed with VoiceOver, once arrow keys were working, that `detune`
spoke only once per press in Logic too.

## 2026-10-22

Started digging into why Logic never speaks a live slider change, even though the value and
its description are both correct. Our own notification call on macOS,
`NSAccessibilityPostNotificationWithUserInfo`, has no return value and no error; from inside
the app, successfully sending a notification and successfully being heard by VoiceOver look
identical. That meant we needed a way to watch, from outside the app, whether the notification
really reaches the system at all, the same thing Apple's own Accessibility Inspector does.

Built a small standalone command line tool for this, `tools/ax-watch/ax-watch.swift`, not part
of the Dexed plugin. It subscribes to a chosen process's accessibility notifications using the
same macOS API Inspector and VoiceOver use, `AXObserverCreate` and
`AXObserverAddNotification`, and prints each one as a plain text line: `AXValueChanged`,
`AXFocusedUIElementChanged`, or `AXFocusedWindowChanged` by default, along with the role,
title, and value of whatever control it fired on. It only reads; nothing in it can set a
value, perform an action, or send a synthetic keystroke or click. Confirmed this directly by
rereading the whole file before running it, since it needed a real system permission.

Getting it running needed Accessibility access granted in System Settings to whichever process
runs it. Discussed two options: grant it to Jim's own Terminal, narrower in scope, or to "The
Chat Place," the app hosting this Claude Code session, broader in scope but lets the tool run
directly without involving Jim's Terminal. Jim chose the broader option. Added the standard
`AXIsProcessTrustedWithOptions` call with the prompt option on, so the tool registers itself
with the system properly instead of failing silently; Jim granted it, confirmed it can be
revoked any time.

Ran four comparison tests, each watching for the same three notifications:

1. Standalone, plain arrow keys through `detune`, `F COARSE`, `F FINE`. One `AXValueChanged`
   per keypress, correct new value each time, `AXFocusedUIElementChanged` correctly on every
   tab move. VoiceOver speaks correctly here, our known-good case.
2. Standalone, VoiceOver's own VO+Left/Right gesture only, same sliders. Identical pattern to
   test 1, same notifications, same shape, nothing extra and nothing missing. This answers the
   parked side issue from 2026-10-21: whatever makes VoiceOver stay quiet about a second
   announcement during its own gesture, it isn't a different notification stream. The two
   input methods are indistinguishable at this level; the difference must be entirely inside
   VoiceOver itself, past the point we can observe from outside Apple's own code.
3. Logic's AU, in the `AUHostingServiceXPC_arrow` helper process, plain arrow keys. Same
   pattern again, correct `AXValueChanged` and `AXFocusedUIElementChanged`, fully matching the
   standalone case. Jim confirmed VoiceOver said nothing at all during this test.
4. Logic's AU, same process, VoiceOver's own gesture only, no plain keys. Same notification
   pattern once more. Jim confirmed VoiceOver spoke every value correctly this time.

Tests 3 and 4 are the key finding. Same plugin instance, same sliders, identical notifications
reaching the system either way, and the only thing that changed was which method triggered
them: silence for a plain key, correct speech for VoiceOver's own gesture. That rules out our
own code and the system-level delivery of the notification as the cause, both are proven
correct and complete. The most likely explanation left: VoiceOver always narrates the result of
its own gesture, since it just asked the question and is reporting back, regardless of which
process answered. For a live, unprompted announcement, it may only speak if that notification
comes from whichever single process the system currently treats as frontmost, which in Logic
is Logic's own process, not the separate helper process actually running Dexed's UI,
`AUHostingServiceXPC_arrow`. VoiceOver can still reach into that helper process's own
accessibility tree on demand, through Tab or its own cursor, which is a different mechanism
from a standing live subscription. If this read is right, the gap isn't fixable from inside
Dexed; it comes from how Logic hosts the plugin in a separate process, which is Apple's and
Logic's design, not ours. Not fully proven; the next check, not yet done, would be reading
which process VoiceOver currently treats as frontmost while sitting in the Dexed window inside
Logic, a one-time lookup rather than another live test.

Also flagging, not yet investigated: partway through today's Logic testing, arrow keys
briefly did nothing again, the same symptom as the one-time episode on 2026-10-21, before
starting to work moments later. This time Jim also had Logic Magician, his Keyboard Maestro
macro set, still on at the very start of testing, normally toggled off inside the Dexed window,
which may or may not be related. Noting this as a second occurrence of the same unexplained
pattern, since it happened again and we still haven't looked into why.

`ax-watch` is meant to be reusable beyond this project, any Mac app where we're checking
VoiceOver support could use the same check. Plan is to turn it into a general skill once it's
proven itself further, rather than packaging it prematurely.

Separately, Jim raised a theory: expected VoiceOver behavior for a blind Logic user is
VO+Right/Left stepping by 5, VO+Shift+Right/Left stepping by 1, and VO+End/Home jumping to the
max or min, but Dexed and Musio, another JUCE app, always step by 1 on VO+Right/Left. Checked
JUCE's own macOS code, `accessibilityPerformIncrement` and `accessibilityPerformDecrement` in
`juce_AccessibilitySharedCode_mac.mm`: JUCE always steps by the slider's own defined interval,
1 for Dexed's DX7 parameters, with no separate coarse and fine step and no check for a Shift
key at all. Not yet confirmed whether VO+Shift+Right/Left or VO+End/Home even reach JUCE in
some other form, since that one function has no way to know a modifier was held. Worked out a
test plan for next session rather than guessing further; see docs/Plan.md for the full
breakdown, across several sliders, several apps, and both VoiceOver gestures and real
keystrokes, paired with `AxTrace` for Dexed specifically.

## 2026-10-24
Ran the step-size test plan from 2026-10-22. Along the way, a separate session had given Jim
a theory to verify: JUCE reports a slider's step size to VoiceOver through its accessibility
value interface, falling back to 1% of the range if the step is set to 0. Checked this against
our own vendored JUCE and Dexed's own code: confirmed exactly, in `juce_Slider.cpp`'s
`getStepSize` and in Dexed's `setRange(..., 1)` calls across `GlobalEditor.cpp` and
`OperatorEditor.cpp`. That explains why VO+Right always steps by 1 in Dexed: the code tells it
to. A separate claim from that session, that JUCE's keyboard handler swallows Shift entirely,
didn't hold up against our own code: `DXSlider::keyPressed` already handles Shift for real
keystrokes. The real open question stayed the same one from before: does a VoiceOver Shift
gesture even reach that keyboard code.

Tried to answer that with `tools/ax-watch` first. In Dexed's standalone build, VO+Right moved
`OP1 F FINE` by 1 each press, 5 presses, matching the code exactly. VO+Shift+Right made
VoiceOver's own "no action" sound and produced no `AXValueChanged` at all, confirmed with the
watcher. Jim pushed back on taking that at face value, since he'd seen Shift work before on
other sliders, so we tested a Control Center brightness slider the same way: VO+Right/Left
moved it by 10% a press, but VO+Shift+Right/Left changed nothing there either, silently this
time, no error sound. Then tried a native Logic instrument slider: VO+Right stepped by 5, three
presses moved it 55, 60, 65; VO+Shift+Right stepped by 1, three presses moved it 66, 67, 68.
Shift really does work differently there. That ruled brightness and Dexed out as universal
proof Shift never does anything, and confirmed Jim's original memory was right, at least for
some native controls.

Tried to find out how the Logic slider does it. Built a second diagnostic tool,
`tools/event-watch`, read-only like `ax-watch`, but watching real system-wide keyboard and mouse
events, key down and up, and mouse down, up, and dragged, instead of accessibility
notifications. Confirmed, twice, that neither VO+Right nor VO+Shift+Right on that Logic slider
ever produces a real keystroke or a real mouse click or drag anywhere on the system; both stay
entirely inside the accessibility layer. Also built a small one-shot tool,
`tools/ax-watch/ax-actions`, to read-only query whatever element currently has focus and list
its supported accessibility actions. The Logic slider reported only the ordinary `AXIncrement`
and `AXDecrement`, nothing beyond what any slider has. So the Shift behavior is real, but
still unexplained: not a real keystroke, not a real mouse event, not an extra action name.

Also tried, early on, to get Dexed's own `AxTrace` diagnostic log working again as a second
way to watch this from inside Dexed. It would not write at all in standalone, including the
catch-all `onValueChange` line that already existed from an earlier session and logs on any
value change regardless of cause. Checked everything that could explain a silent failure: the
marker file was present at the exact path the app would check, that path matched
`NSTemporaryDirectory()` exactly, the running process's own `TMPDIR` environment variable
matched too, the binary was confirmed to contain the compiled-in marker and log file names, and
the directory had normal permissions. No cause found. Left unresolved; `ax-watch` and
`event-watch` carried the rest of today's testing instead.

Jim's call at the end of the session: stop chasing the exact mechanism for now, and move
toward a version of Dexed he considers acceptable to ship, rather than continuing to chase
full VoiceOver step-size parity with native macOS controls.

Picked up the open question from 2026-10-17 about a plain local build silently coming out
x86_64 only, since `CMAKE_OSX_ARCHITECTURES` is only forced to include arm64 in
`CMakeLists.txt` when `CMAKE_BUILD_TYPE` is set to `Release`. Checked the actual build folder
used day to day: its cached settings already had `CMAKE_OSX_ARCHITECTURES` set to `arm64` with
no build type set, so the quiet x86_64 default from 2026-10-17 didn't reproduce here. First
attempt at a fix removed the `Release`-only condition so every build would always include both
architectures; building confirmed this worked, but also made the regular fast local test build
slow, a universal binary every time, which undoes the point of keeping daily builds to just
this Mac's native chip. Reverted that change; `git diff` confirmed `CMakeLists.txt` matches
the last commit exactly, so no code changed in the end.

Found a side effect of the reverted attempt worth remembering: once CMake writes
`CMAKE_OSX_ARCHITECTURES` into a build folder's `CMakeCache.txt`, reverting the project file
doesn't undo it, since CMake's `set(... CACHE ...)` without `FORCE` never overwrites an
existing cache entry. The existing build folder stayed stuck on the universal setting until
reset directly with `cmake -DCMAKE_OSX_ARCHITECTURES=arm64 .`. If a build folder ever seems to
ignore a `CMakeLists.txt` change to an architecture or similar cached setting, that's the
likely cause: delete the stale entry from `CMakeCache.txt`, pass the value directly on the
command line, or delete and recreate the build folder.

Confirmed, by building and checking with `file`, not just by reading code: the existing build
folder produces an arm64-only `DexedTests` binary with the documented plain build command,
matching what day-to-day testing wants, and a separate, throwaway build folder configured with
`-DCMAKE_BUILD_TYPE=Release` produces a universal `DexedTests` binary covering both
architectures, matching what a release build needs. Both already worked correctly before today;
nothing needed changing in `CMakeLists.txt` itself.

Agreed with Jim on the release process this points to: keep everyday test builds arm64 only,
this Mac's native chip, and before any release, build with `CMAKE_BUILD_TYPE=Release` and use
Rosetta, the Mac's built-in translator for Intel-only apps, to run a VoiceOver check of the
x86_64 half on this Mac before shipping.

Jim separately confirmed, testing directly in Logic, that Shift+arrow behaves the same as
plain arrow keys there: the value changes correctly, but VoiceOver stays silent. Extends the
2026-10-22 finding, which had only been confirmed for plain arrows in Logic.

Went back to the open question from 2026-10-22 about why Logic never speaks a live slider
change: the leading theory, not yet proven, was that VoiceOver only announces live changes
from whichever process the system currently treats as frontmost, which in Logic is Logic's
own process, not the separate helper process actually running Dexed's UI. The planned next
check was a one-time look at which process VoiceOver treats as frontmost while sitting in the
Dexed window inside Logic.

Built that check as a new tool in the existing `event-watch` skill, outside this project at
`~/.claude/skills/event-watch`, rather than as a one-off script, so it's reusable for other
apps later. Backed up the whole skill folder first, to `~/.claude/skill-backups`, before
touching it. The new tool, `front-watch.swift`, watches for changes in which app the system
treats as frontmost and logs each one with a timestamp; unlike `ax-watch` and `event-watch`, it
needs no extra system permission. Designed around a real problem Jim raised: a one-time check
would most likely just catch whichever app hosts this chat session as frontmost, not Logic,
since that's what's frontmost while typing to Claude. Settled on having it run continuously in
the background and log every change, the same pattern `ax-watch` already uses, so Jim could
switch over and test at his own pace with nothing to time. Documented in the skill's
`SKILL.md` alongside the other three tools.

Ran it live: started `front-watch` in the background, had Jim switch to Logic, get VoiceOver
onto a slider, and press plain arrow keys a few times, then switch back. The log showed
frontmost switching from the chat app to Logic Pro, staying Logic Pro for the entire 45-second
test window, then switching back to the chat app at the end. Nothing else appeared in between.
This looked like it confirmed the theory directly: while Jim was arrow-keying a slider inside
Dexed's window, the system never once treated anything but Logic's own process as frontmost,
never the helper process actually running Dexed's UI.

Jim pushed to check this more precisely before resting on it, pointing out that `front-watch`
only asks the ordinary, simple version of "which app is frontmost," not the more specific
thing the accessibility system itself tracks, which can differ from it. Built a second new
tool for this, `ax-focus-watch`, also added to the `event-watch` skill: given a short list of
specific apps by pid, it reports which one, if any, the accessibility system currently has
focus sitting inside.

First attempt had it ask the simpler, system-wide version of this question, the same shape as
`front-watch`'s question but for accessibility focus instead of ordinary frontmost status.
That failed outright with a real error, `kAXErrorCannotComplete`, when run through this chat
session specifically, even though plain Accessibility access was already granted. Rewrote it
to ask about specific, named apps by pid instead, the same basic method `ax-watch` already
uses successfully, which did work, confirmed against two ordinary running apps as a sanity
check before using it for real.

Ran it for the silent test, watching Logic's own process and the `AUHostingServiceXPC_arrow`
helper process actually running Dexed's UI, while Jim repeated the plain-arrow-key test. Result:
accessibility focus sat on Logic Pro itself for the whole 13-second window, never the helper
process, matching `front-watch`'s finding, but notably not pointing at the helper process
either, the one actually running Dexed's slider.

Tried the same check against the speaking case, VoiceOver's own gesture, VO+Left or VO+Right,
for comparison. First run came back empty because the tool, run through this chat session,
hit the same kind of error as the earlier system-wide attempt, a real limit of asking this
particular question from that execution context rather than a mistake in the test. Worked
around it by having Jim run the compiled tool directly from Terminal instead, after granting
Terminal its own Accessibility permission, the same way Jim's own Terminal usage would need to.
That approach worked for a second silent-test rerun, confirming the Logic Pro result above
independently.

The actual speaking-case test, run the same way from Terminal, came back with nothing at all:
neither Logic nor the helper process ever showed up holding accessibility focus, for the whole
test window, even though Jim confirmed performing the gesture correctly on a real Dexed slider
and VoiceOver did speak the value. That's a real, clean result, not a technical failure this
time, but it doesn't fit a tidy story: the silent case pointed at Logic, not the helper; the
speaking case pointed at neither. Best guess, not confirmed: VoiceOver's own gesture may not
touch this particular piece of system bookkeeping at all, since it works through a direct
accessibility action on the control rather than anything resembling normal keyboard focus, so
this check may simply be the wrong lens for that case, not evidence that nothing is focused.

Jim's call: stop investigating the mechanism further for now. The original, simpler
`front-watch` finding, frontmost stayed Logic throughout the silent test, stands as the best
evidence gathered, suggestive but not proof; the more precise follow-up check complicated
rather than confirmed the theory, and chasing it further didn't look likely to pay off. Both
`front-watch` and `ax-focus-watch` stay in the `event-watch` skill for reuse if this becomes
worth revisiting later, or for debugging similar focus questions in other apps.

## 2026-10-25

Fixed the envelope and pitch envelope diagram double-announcement, the last known
double-speak bug on the parked list. `EnvDisplay` and `PitchEnvDisplay`, in
`Source/DXComponents.h`, each had a `notifyAccessibleValueChanged()` method called from every
one of their own rate and level sliders' `sliderValueChanged` handlers, in
`Source/OperatorEditor.cpp` and `Source/GlobalEditor.cpp`, on top of the slider's own normal
announcement. Same root shape as the LCD fix from 2026-10-21: two things telling VoiceOver
about one change.

Walked Jim through two ways to fix it, since it changes what he hears: stop the diagram's live
announcement during its own sliders and rely on the slider's own announcement, matching the
LCD fix, or keep the diagram's full-curve announcement live and suppress the slider's own
built-in one instead. Recommended the first as the simpler, already-proven pattern. Jim chose
it.

Removed the sixteen `notifyAccessibleValueChanged()` calls, eight each in
`OperatorEditor.cpp`'s and `GlobalEditor.cpp`'s slider handlers, keeping the `repaint()` calls
next to them so the diagrams still redraw. Checked both files first and confirmed those were
the only call sites for either display; the diagrams' other update paths, when switching
operators or loading a patch, only ever called `repaint()`, never the announcement. With the
calls gone, the `notifyAccessibleValueChanged()` methods on `EnvDisplay` and `PitchEnvDisplay`
had no callers left anywhere, so removed those methods too rather than leave dead code behind.
`LcdDisplay`'s own version of the same method stays, since it's still used by
`LcdDisplay::setSystemMsg` for real system messages.

Built the standalone app, confirmed arm64, ran `DexedTests`, still passing. Not yet tested
with VoiceOver; that's next.

Jim tested Operator 1 with VoiceOver and found several more sliders that double-speak in
standalone, beyond the eight just fixed: EG level 1 through 3, EG rate 3, AMP MOD SENS, output
level, L and R scale depth, and rate scaling. Before digging in, asked which build Jim tested,
since a running standalone instance keeps old code in memory until it's quit and relaunched,
the same mistake that cost a session on 2026-10-21. Didn't get to resolve that, because Jim
stopped the investigation first.

Jim's call: stop chasing double-speak in standalone entirely. Reason: most real users will run
Dexed with VoiceOver inside Logic, not standalone, and Logic doesn't speak live value changes
at all right now regardless, the separate, still-unresolved Logic-silence issue from
2026-10-22. A double-speak bug that only shows up in standalone isn't reaching the people this
project is for. Recorded in docs/deviations.md. Today's envelope diagram fix stays, since it's
a real improvement and carries no cost, but no further double-speak investigation is planned.

Simplified the Shift+arrow "move by 5" step. It used to only apply to sliders whose range is
exactly 0 to 99, and on those, it snapped to the next or previous multiple of 5 rather than
moving a fixed amount from wherever the value actually was. Changed `DXSlider::keyPressed`,
`Source/DXComponents.h`, to a plain add-5 or subtract-5 from the current value, clamped at
that slider's own real min and max, on every slider in the app regardless of its range. Jim's
choice, simpler and more predictable than the old snap-to-grid behavior. Built standalone,
confirmed arm64, ran DexedTests, both passing. Not yet tested with VoiceOver.
