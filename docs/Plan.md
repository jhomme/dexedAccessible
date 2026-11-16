# Plan

Tasks in order. A checked box means settled, either finished as planned or closed by a
decision. For the story behind any task, see docs/log.md by date. For a decision that changed
or closed something, see docs/deviations.md. Keep each line here short: what, status,
commit or date. Put narrative, investigation detail, and test steps in log.md or
acceptance.md instead, and link back here.

- [x] Upgrade vendored JUCE to 8.0.15 and fix the resulting API breaks. `cfab624`, 2026-10-02.
- [x] Fix the crash the JUCE upgrade introduced on any slider or control interaction.
  `6da8345`, 2026-10-09.
- [x] Add the DexedTests headless regression suite. `d969d33`, `ea897f9`, 2026-10-09.
- [x] Open upstream pull request asb2m10/dexed#534 with the JUCE upgrade, crash fix, and test
  suite.
- [x] Merge the crash fix into `accessibility` without waiting for upstream review. `477cd03`,
  2026-10-10. See deviations.md.
- [x] Remove the temporary ComboBox accessibility workaround in `Source/DXComponents.h`, now
  redundant after the JUCE upgrade. `6f99c9b`, 2026-10-10.
- [x] Test the crash fix and ComboBox cleanup in Logic, VST3 and AU. Passed, 2026-10-14.
- [x] Add real accessibility handlers for `EnvDisplay`, `PitchEnvDisplay`, and `LcdDisplay`.
  Passed, 2026-10-14.
- [x] Add accessibility support for `AlgoDisplay`. Passed, 2026-10-15.
- [x] Walk the whole plugin with VoiceOver and name every unlabeled control: the algorithm
  dial, the mono/poly switch, and the keyboard's two octave scroll buttons. `9c0e188`,
  `eedfb6b`. Passed, 2026-10-15.
- [x] Fix the "Keyboard keys" area reporting empty content to VoiceOver's interact command.
  Passed, 2026-10-15.
- [x] Test that whole batch, algorithm dial through keyboard content, in Logic, VST3 and AU.
  Passed, 2026-10-15.
- [x] Give VoiceOver proper arrow-key stepping on the DX7's parameters: Home/End jump to a
  slider's min/max, Shift+arrow moves by 5. Passed, 2026-10-15.
- [x] Test the arrow-key stepping work in Logic, VST3 and AU. Passed, 2026-10-17.
- [x] Find and fix the LCD/slider double-announcement, where a slider's value spoke twice per
  key press. `Source/DXComponents.h`, `Source/DXComponents.cpp`, `Source/GlobalEditor.cpp`,
  2026-10-21.
- [x] Rebuild VST3, AU, and CLAP with that fix and confirm with VoiceOver in Logic. Passed,
  2026-10-21.
- [x] Investigate why VoiceOver stays silent on live slider changes in Logic, including under
  Shift+arrow. Inconclusive; not pursued further. See deviations.md.
- [x] Investigate arrow keys briefly not responding in Logic on two separate occasions. Not
  pursued further. See deviations.md.
- [x] Investigate why VO+Shift+Right/Left steps by 1 in Dexed but by 5 on native Logic
  sliders. Confirmed the difference is real; the mechanism stays unexplained. Not pursued
  further. See deviations.md.
- [x] Confirm a plain local build defaults to arm64 and a Release build is universal; no
  `CMakeLists.txt` change needed. 2026-10-24.
- [x] Fix the envelope and pitch envelope diagram double-announcement. 2026-10-25.
- [x] Stop chasing further standalone double-speak reports on other sliders. See
  deviations.md.
- [x] Simplify Shift+arrow to move a plain 5 on every slider, not just ones ranging 0 to 99.
  2026-10-25.
- [ ] Fix the "TRIANGE" typo for the LFO TRIANGLE waveform label in `Source/PluginParam.cpp`.
  Not started.
- [ ] Confirm Master Tune's actual cents range against `Source/msfa/dx7note.cc`'s fixed-point
  math. Not started.
- [x] Convert the DX7 manual to Markdown and sort its sections by hardware/mixed/software
  relevance. `docs/dx7-manual.md`, `docs/dx7-manual-hardware-sections.md`, 2026-10-26.
- [x] Split documentation work into two tracks: finish and ship accessibility work first;
  build a standalone user guide afterward, on its own branch. See deviations.md.
- [x] Check the DX7 manual against Dexed's actual code, parameter by parameter. Found two
  real divergences and one typo bug; see log.md, 2026-10-26.
- [x] Pull a one-sentence description for every control from the manual, as source material
  for titles and tooltips. `docs/control-manual-mapping.md`, `docs/tooltip-descriptions.md`.
- [x] Jim's review pass on `docs/tooltip-descriptions.md` before writing any code. 2026-10-26
  and 2026-10-31.
- [x] Put the approved titles and tooltips into real code across the plugin. 2026-10-31.
- [x] Test the new titles and tooltips with VoiceOver in Logic; found and fixed several real
  bugs along the way, including `Ctrl::bind()` silently overwriting titles. See log.md,
  2026-10-31.
- [x] Add Home/End jumping to the cartridge manager's program lists, and announce which list,
  browser or active, a slot belongs to when focus arrives from elsewhere. Jim confirmed with
  VoiceOver; not yet committed as of 2026-10-31.
- [x] Jim's own VoiceOver pass over the full accumulated set of fixes above, in Logic.
  Confirmed clean, 2026-11-01. Jim is holding off on tagging the release until he
  understands some things first; see bookmark.md.
- [x] Open the PR for `accessibility`. Closed instead: asb2m10 closed our earlier PR 534 after
  upstream's own 1.1.0 refactor, and Jim decided not to pursue an upstream PR for now. See
  deviations.md, 2026-10-31.
- [x] Release checklist, since the accessible build is now meant for other VoiceOver and
  screen reader users, not just Jim. See deviations.md, 2026-10-31:
- [x] Turn on GitHub Actions for the jhomme/dexedAccessible fork; it had never run there.
  Needed a one-time click on github.com, no API for it. Confirmed working, 2026-11-01.
- [x] Decide a version-numbering scheme for fork releases, separate from upstream's own
  numbers, since upstream is already at a 1.1.0 that's a different codebase. Jim's call,
  2026-11-01: start at v0.1.0, three-part major.minor.patch, matching the "v" prefix
  upstream's own tags use and that this project's CI already expects when it reads a
  version number from a tag.
- [x] Decide on Mac code signing for release installers. Jim's call, 2026-10-31: ship
  unsigned for now, get a real certificate later. See the next item and deviations.md.
- [x] First real GitHub Actions run on the fork surfaced two real build breaks: `JUCE_JACK=1`
  was set unconditionally in `Source/CMakeLists.txt`, so Windows failed looking for a Linux-only
  header, and the macOS job failed up front importing a code-signing certificate that doesn't
  exist on this fork. Fixed the JACK one: gated it to Linux only, matching how `JUCE_ALSA`
  already checks the platform. Fixed the macOS one: the certificate-import step now skips
  itself when the secret is missing, instead of failing the whole job. Both confirmed working
  in a real CI run, 2026-11-01: Linux, Windows, and macOS all build successfully now.
- [x] Stop doc-only pushes from triggering the full three-platform build. Added
  `paths-ignore` to `.github/workflows/project-pipeline.yml`'s push trigger, confirmed
  working with a real readme-only push, 2026-11-01. See log.md for a caveat worth checking
  when the real release tag gets pushed.
- [x] Tag and push the first release. `v0.1.0`, 2026-11-01. The release-creation job first
  failed on a missing permission; fixed and retagged. See log.md.
- [x] Fix the version number baked into the actual release files, separate from the version
  the git tag carries. `CMakeLists.txt`'s `project(Dexed VERSION 1.0.1)`, left over from
  before this project forked away from upstream, feeds the installer's real file names and
  version property, not the tag-derived `BUILD_ID`. Confirmed by downloading a real nightly
  build's artifacts and finding "1.0.1" baked into every file name. Changed to
  `VERSION 0.1.0` to match the release scheme. Built and ran DexedTests locally, confirmed
  arm64, both passing, 2026-11-01.
- [x] Fix the real GitHub release failing to get created: the workflow's automatic token had
  only read access, so the release-creation job got a 403. Scoped write access to just that
  job in `.github/workflows/project-pipeline.yml`, `167d038`, 2026-11-01. Confirmed a real,
  non-draft release now exists with all three platform installers attached.
- [ ] Track two, after the release is out: build a standalone Dexed user guide on a new
  branch off `accessibility`, covering the "mixed" and "fully relevant" manual sections, with
  a glossary for newcomers to synthesis. Not started.
- [x] Rename the plugin's identity away from Dexed and Digital Suburban, to DexedAccessible
  and jhomme, across build settings, both installer scripts, and two places in source code
  that picked app data and log folder names. See deviations.md. Confirmed working in Logic,
  2026-11-01.
- [x] Fix a local build tool mismatch on this Mac that produced x86_64-only binaries after
  the rename, and a stale macOS plugin cache that kept the new build from loading in Logic.
  See log.md, 2026-11-01.
- [x] Fix the About box for the rename: button name, dialog title, repo and latest-release
  links, and make its credits text readable by VoiceOver instead of only painted graphics.
  Confirmed working, 2026-11-01.
- [x] Bump the version number to 0.2.0 in both places that carry one, since the identity
  rename breaks projects saved under v0.1.0. See deviations.md, 2026-11-01.
- [x] Add a paragraph to README.md and announcement.md pointing at the releases/latest page,
  instead of per-file links that go stale every release. Done in both files, 2026-11-01; not
  yet committed.
- [x] Commit, push, and tag `v0.2.0`. `2930a9e`, 2026-11-01.
- [x] Fix the macOS build pointing at a GitHub runner image, `macos-14`, that GitHub retired
  the day after this build ran. Changed to `macos-15`. Not yet committed. See log.md,
  2026-11-01.
- [x] Switch the five keyboard navigation shortcuts from issue #3 to use Command instead of
  Control on Mac, and fix two real bugs found while testing the switch: the operator mute
  toggle never actually applied, and Shift held with a number key broke the match entirely.
  `2dfe5e1`, 2026-11-14.
- [x] Announce context when a keyboard shortcut jumps or returns focus across panels:
  Command+1-6, Command+G, Command+K, Command+L, and Command+P all announce the panel's name
  before landing on its first control, but only when arriving from somewhere else. Toggling
  an operator with Command+Shift+1-6 announces on/off without moving focus. Closing the
  cartridge library or the function dialog, by Escape or by its own Close button, announces
  the panel focus is returning to. See log.md and deviations.md, 2026-11-14.
- [x] Release v0.3.0: the Command-key shortcuts and panel announcements above, plus the
  CLAP/Reaper typed-value fix released without a Reaper test. Bumped CMakeLists.txt to
  0.3.0, disclosed the untested Reaper fix in README.md and the release notes, attached
  Documentation/Keybindings.md to the release. Local build clean, arm64 confirmed on all four
  targets, DexedTests passed. Committed `26326bf`, tagged and pushed, 2026-11-15. Confirmed
  published with all three installers, the Keybindings.md download, and the disclosure text
  in the release notes. See deviations.md.
- [x] Release v0.4.0: the accessibility-refactor branch's work, merged into `accessibility`:
  `AlgoDisplay`'s live handler and Command+Shift+A, `CartridgeFileDisplay`'s live handler,
  the explicit group role on `TuningShow`/`ParamDialog`/`ProgramListBox`, and the
  Tab/Shift+Tab panel-crossing announcement, plus the still-carried-over CLAP typed-value
  fix from v0.3.0, still untested in a real CLAP host. Bumped `CMakeLists.txt` to 0.4.0,
  rewrote `docs/release-notes.md` and the relevant README.md paragraph for this release's
  actual content, including the Windows/NVDA announcement-routing gap that applies to both
  this release's panel-crossing announcement and the panel-landing ones from v0.3.0. Added
  the same new content to `announcement.md` (gitignored, not yet sent). Added the new
  Command+Shift+A/Control+Shift+A shortcut and a general Tab/Shift+Tab panel-announcement
  line to `Documentation/Keybindings.md`, in its existing Mac/Windows-separated format, so
  the attached download at the release stays current. Local build clean, arm64 confirmed on
  all four targets, `DexedTests` passed. Jim re-tested in standalone the two pieces only
  confirmed in Logic before, both passed; corrected a real overclaim in
  `docs/release-notes.md`'s first draft, which had said all four pieces were confirmed in
  both hosts before that was actually true. Committed `b964b56`, tagged and pushed,
  2026-11-15. CI built macOS, Windows, and Linux successfully and published the release:
  all three installers, checksums, the source tarball, and Keybindings.md are attached, and
  the release notes show the corrected disclosures.
- [x] Comma/period group-stepping, from issue #3: Comma jumps to the previous named panel
  (an Operator, Global Parameters, or the on-screen keyboard when showing), Period to the
  next, cycling in that order and wrapping at either end, landing on the panel's first
  control, via the same `announceGroupThenFocus()` the Command+1-6/G/K/L shortcuts already
  use. Neither key needs a modifier, since nothing in those three panels has a text field
  for either character to type into. Blocked while the cartridge library is open, the same
  way Command+1-6/G/K already are, since those panels are hidden from accessibility then.
  `Source/PluginEditor.cpp`. Added both keys to `Documentation/Keybindings.md`. Build clean,
  arm64 confirmed on all four targets. Jim's testing found a real bug: stepping backward
  with Comma onto the keyboard, then pressing Comma again, stuck there instead of
  continuing to Global Parameters. Root cause and fix in `findEnclosingNamedGroup()`; see
  log.md. Confirmed fixed, 2026-11-16: both the forward and backward full cycle through all
  panels now work, and the cartridge-library block was separately confirmed working too.
  Committed `e063816`, pushed.
- [x] Release v0.5.0: comma/period group-stepping and the `findEnclosingNamedGroup` fix
  above. Bumped `CMakeLists.txt` to 0.5.0, rewrote `docs/release-notes.md`, the matching
  README.md paragraph, and `announcement.md` for this release's actual content. Local
  build clean, arm64 confirmed on all four targets, `DexedTests` passed.

## accessibility-refactor branch: systematic AccessibilityHandler coverage

Jim's call, 2026-11-15: branch off `accessibility` to apply JUCE's `AccessibilityHandler`
class across the whole app in one pass, instead of the current one-component-at-a-time
approach, and see how much it improves things. Test, then decide whether to merge back into
`accessibility`.

- [x] Survey every custom `Component` for real accessibility handler coverage. Done as part
  of planning, 2026-11-15: `AlgoDisplay`, `VuMeterBase` (backs `VuMeter` and `VuMeterOutput`),
  `CartManager`/`CartridgeFileDisplay`, `TuningShow`, and `ParamDialog` have no real handler,
  only a static tooltip or nothing. `EnvDisplay`, `PitchEnvDisplay`, `LcdDisplay`, `DXSlider`,
  `ProgramLabel`, `OperatorEditor`, and `GlobalEditor` already do, each with its own nested
  `AccessibilityHandler` subclass. Not started as code yet.
- [x] Add a real `createAccessibilityHandler()` to `AlgoDisplay`, reusing
  `describeAlgorithm()`, with a live announcement when the algorithm or an operator's
  on/off state changes. Moved the live description off the ALGO dial's tooltip onto the
  diagram itself, per Jim's call, so the same text isn't heard twice in a row; the dial
  now just says "Algorithm" plus a short static tooltip. Added a new shortcut,
  Command+Shift+A (Control+Shift+A on Windows), that announces the diagram's description
  from anywhere without moving focus, `Source/PluginEditor.cpp`, `Source/GlobalEditor.h`.
  Build clean, arm64 confirmed on all four targets. Confirmed working with VoiceOver in
  Logic, 2026-11-15, including that Logic does not intercept the new shortcut. Also
  confirmed in the standalone app, 2026-11-15. Not yet committed.
- [x] Add a real `createAccessibilityHandler()` to `CartManager` and `CartridgeFileDisplay`
  for browsing and loading cartridges. `CartridgeFileDisplay`, the small line showing which
  cartridge file the current program is based on, gets a live label handler following the
  `LcdDisplay` pattern; its mouse-only "reveal in Finder" action was left as is, since that
  action has no non-visual equivalent worth building. `CartManager` itself now states an
  explicit `group` role, the same NVDA-on-Windows fix already used by `GlobalEditor` and
  `OperatorEditor`. `Source/CartManager.h`. Build clean, arm64 confirmed on all four targets.
  Jim confirmed working with VoiceOver in Logic, and later also in the standalone app,
  2026-11-15. Windows/NVDA not yet tested. Not yet committed.
- [x] Add the explicit `group` role to `TuningShow`, `ParamDialog`, and `ProgramListBox`,
  the same NVDA-on-Windows fix already used by `GlobalEditor`, `OperatorEditor`, and
  `CartManager`. `ParamDialog`'s own controls already had real titles and tooltips from the
  earlier pass and are already VoiceOver-tested via Command+P; `ProgramListBox` already had
  a real title and keyboard focus. Neither had the explicit role. `TuningShow`'s tuning
  table itself is a separate, bigger question; see the note below. `Source/ParamDialog.h`,
  `Source/TuningShow.h`, `Source/ProgramListBox.h`. Build clean, arm64 confirmed on all four
  targets. Windows/NVDA not yet tested, so neither is Jim's own VoiceOver pass on these
  three specifically, since the change has no Mac-visible effect. Committed `e3284d6`,
  pushed.
- [x] VoiceOver test each handler as it's built, in Logic, VST3 and AU. Jim confirmed
  working, 2026-11-15.
- [x] Jim's full VoiceOver pass over the whole plugin on this branch, compared against the
  current `accessibility` branch, to judge how much the systematic approach actually
  improves things. Jim confirmed it worked, 2026-11-15.
- [x] Decide whether to merge `accessibility-refactor` into `accessibility`, based on that
  comparison. Jim's call, 2026-11-15: merge it.

## Keeping accessibility-refactor alive after the merge
Jim found something while testing this branch worth a closer look: plain Tab and Shift+Tab
between panels do not announce the panel's name, unlike the Command+1-6/G/K/L/P jump
shortcuts, which do, via `announceGroupThenFocus()` in `Source/PluginEditor.cpp`. Not
investigated yet. Keeping `accessibility-refactor` checked out rather than deleting it after
the merge, so this can be looked at there.

- [x] Investigate why plain Tab/Shift+Tab does not announce a panel's name on entry, the way
  the Command-key jump shortcuts already do, and fix it. A new `GroupAnnouncer`, registered
  as a `juce::FocusChangeListener` on `Desktop`, hears about every real keyboard focus change
  there is, not just ones caused by a Command-key shortcut, and speaks the new panel's name
  when it differs from the last one spoken, at low announcement priority so it queues behind
  VoiceOver's own announcement of the newly focused control instead of cutting it off. Gated
  to only fire for a real Tab or Shift+Tab (a new `tabKeyMovedFocus` flag, set in `keyPressed`
  and consumed by the listener) so it stays quiet for every other way focus can change,
  including VoiceOver exploring by group, which already announces the group's name itself;
  without that gate the name got spoken twice. Shares `lastAnnouncedGroup` with the existing
  `announceGroupThenFocus()` so the two don't repeat each other. `Source/PluginEditor.h`,
  `Source/PluginEditor.cpp`. See log.md for two real bugs found and fixed during testing: a
  timing race in an early version, and a one-off standalone failure that turned out to be
  stale earlier-session state, not the code. Build clean, arm64 confirmed on all four targets.
  Jim confirmed working with VoiceOver in both Logic and the standalone app, 2026-11-15.
  Windows/NVDA not yet tested. Committed `d131138`, merged into `accessibility` (fast-forward
  to `d131138`), both pushed. `accessibility-refactor` deleted, locally and on GitHub, its
  purpose served.

## Not pursuing a VuMeterBase handler for now
Jim's call, 2026-11-15: out of scope for this branch, and not to be brought up again on its
own. A VU meter's level changes many times a second, so a real handler needs a design
decision first about what VoiceOver should actually say and when, rather than announcing
every change. Revisit only if someone actually asks for it.

## Not pursuing a real TuningShow table audit for now
Jim's call, 2026-11-15: same category as the VU meter above, and not to be brought up again
on its own. The tuning table's 128 rows are drawn by a third-party component, Surge Synth
Team's `TuningTableListBoxModel`, not this project's own code. Whether VoiceOver already
reads it row by row is unknown without testing, and if it does not, fixing it is open-ended
work affecting only users who load a custom SCL or KBM tuning file. Revisit only if someone
actually asks for it.
