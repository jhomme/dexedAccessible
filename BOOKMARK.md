# Bookmark

## Where we are
GitHub issue #3, keyboard navigation shortcuts: switched the five existing shortcuts from
Control to Command on Mac (Windows keeps Control, unchanged). Testing turned up two real,
pre-existing bugs, neither caused by the switch: Command+Shift+1-6 didn't match at all
because macOS reports the shifted symbol above a number key, not the digit, once Shift is
held; and even once matched, the operator mute toggle never really applied because
`toggleOpSwitch()` suppressed the notification that makes the change real. Both fixed. Jim
confirmed all five shortcuts, including both toggle fixes, work in the standalone app and as
an AU plugin in Logic. Not yet committed. Full account in docs/log.md, entry dated
2026-11-14. Still open from issue #3: comma/period group-stepping, and the preset save/picker
tab-order fix, neither started.

Separately, still open, untouched since 2026-11-10: GitHub issue #1, "Parameters not
correct," the CLAP build on Windows not applying typed parameter values in Reaper. Found and
fixed the real cause for plain numeric controls; switches, note names, labeled dropdowns, and
master tune still need their own design pass. Built clean, confirmed arm64, `DexedTests`
passed. Not committed yet: holding until Jim can test the real CLAP plugin in Reaper, which
he doesn't have and is arranging to test through the Reapers Without Peepers list. The
separate Reaper-crash-on-close part of the report is still unlooked-at. Full account in
docs/log.md, entry dated 2026-11-10.

Before that: closed out PR #2, a contributor's keyboard-focus fix, after chasing down an
unrelated Linux CI failure that looked at first like the PR had broken something. It hadn't;
the Linux build's own package list was broken by an Ubuntu runner image change. Fixed in two
commits. Also found the PR's commit message overstated what it built: it claims computer-
keyboard note-playing works, but that's not wired up. Logged it as item 3 in `docs/0.3.0.md`
and said so in a PR comment, without reverting the PR itself.

Also fixed a stale part of this file's own Plan section: it said `EnvDisplay` and
`PitchEnvDisplay` still needed accessibility handlers, but both already have them, confirmed
by `git log`. Corrected it, and pointed the real next display task at `AlgoDisplay`, which
still only has a static tooltip, plus `VuMeter`/`VuMeterOutput`, the same gap, which the
Plan section hadn't mentioned at all. See docs/log.md, entry dated 2026-11-10.

## v0.3.0: collecting accessibility changes, not building yet
Jim is gathering a list of accessibility problems to fix before building any of them,
spread over several days of VoiceOver testing rather than one sitting. The list lives in
`docs/0.3.0.md`. Four items so far: VoiceOver's own arrow navigation in the sound list
not moving keyboard focus with it; Escape in the function dialog not returning focus to the
function button; the on-screen keyboard having no real per-key accessibility and no
computer-keyboard note-playing despite JUCE supporting it; and no way to type an exact
number into a slider with the keyboard, the way Surge XT's "Edit Value" lets you press
Enter, type a number, and press Enter again to commit. Confirmed that Surge feature is real
by reading Surge's own docs; building the same here would mean a new popup component, since
JUCE's own built-in slider text box is reportedly hard to make accessible.

## What's next
1. Get Jim set up to test the CLAP parameter fix in Reaper, then commit it if it works.
2. Look into the Reaper-crash-on-close half of issue #1.
3. Design comma/period group-stepping for issue #3, and the preset save/picker tab-order fix
   it also asked for.
4. Jim keeps adding accessibility problems to `docs/0.3.0.md` as he finds them. Nothing in
   that list gets built until he says so.
5. Send the announcement (announcement.md, gitignored, not committed) now that v0.2.0 is
   confirmed real. Still sitting unsent.
6. Track two, still not started: branch off `accessibility` and build a standalone Dexed
   user guide from the "mixed" and "fully relevant" sections in
   `docs/dx7-manual-hardware-sections.md`, with a glossary for newcomers to synthesis.
7. Jim said he'll get a real Apple Developer Certificate "soon," which would let future Mac
   releases be signed, removing the Gatekeeper warning. Not scheduled yet.

## Not pursuing an upstream PR for now
asb2m10 closed our earlier crash-fix PR after upstream's own incompatible refactor. Jim's
call: keep developing on this fork, revisit only if there's a natural pause worth spending on
it. See docs/deviations.md.
