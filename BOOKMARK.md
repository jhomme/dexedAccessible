# Bookmark

## Where we are
GitHub issue #3, keyboard navigation shortcuts: switched the five existing shortcuts from
Control to Command on Mac (Windows keeps Control, unchanged), and fixed two real,
pre-existing bugs found while testing the switch. Committed as `2dfe5e1`.

On top of that, in the same session: VoiceOver now announces which panel a shortcut lands
on or returns to, not just the control it lands on, such as "Operator 3" before "Detune 0."
Covers Command+1-6, Command+Shift+1-6 (announces on/off without moving focus), Command+G,
Command+K, Command+L, and Command+P, plus closing the cartridge library or the function
dialog by Escape or by its own Close button. Along the way, fixed a real bug where the
cartridge library's jump shortcuts announced a panel but landed nowhere (now blocked with a
spoken "press Escape to exit" message instead), and a real JUCE double-invocation bug where
a single Escape press closed the function dialog twice, cutting its own announcement off.
Decided to announce context instead of forcing focus onto the function button, Jim's
preference after testing; and to ship these announcements on Windows now even though that
path bypasses NVDA, with proper NVDA routing as separate future work. Both decisions in
deviations.md. Jim confirmed working with VoiceOver in standalone and in Logic (AU). Not yet
tested on Windows or with NVDA. Full account in docs/log.md, entry dated 2026-11-14. Not yet
committed.

Still open from issue #3: comma/period group-stepping, and the preset save/picker
tab-order fix, neither started.

Separately, GitHub issue #1, "Parameters not correct," the CLAP build on Windows not
applying typed parameter values in Reaper. Found and fixed the real cause for plain numeric
controls; switches, note names, labeled dropdowns, and master tune still need their own
design pass. Built clean, confirmed arm64, `DexedTests` passed. This was committed on
2026-11-14 as `62cd6dc`, correcting this file's earlier note that it was being held back.
Jim decided on 2026-11-15 to release it without waiting for the Reaper test, since he hasn't
set up Reaper yet; see deviations.md. The separate Reaper-crash-on-close part of the report
is still unlooked-at. Full account in docs/log.md, entries dated 2026-11-10 and 2026-11-15.

Today, 2026-11-15, preparing to build and release the current state of `accessibility`:
logged that deviation, added a line to README.md disclosing the untested Reaper fix and
linking Documentation/Keybindings.md, added a short release-notes blurb
(docs/release-notes.md) that the release workflow now injects into the GitHub release's own
notes, and changed `.github/workflows/project-pipeline.yml` to attach Keybindings.md to the
release as its own download. None of this is committed yet. Still open: what version number
to use for this release.

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
8. Better documentation overall, beyond Keybindings.md, is a planned future item. Not
   scheduled yet.

## Not pursuing an upstream PR for now
asb2m10 closed our earlier crash-fix PR after upstream's own incompatible refactor. Jim's
call: keep developing on this fork, revisit only if there's a natural pause worth spending on
it. See docs/deviations.md.
