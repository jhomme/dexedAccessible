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
- [ ] Jim's own VoiceOver pass over the full accumulated set of fixes above, in Logic.
- [x] Open the PR for `accessibility`. Closed instead: asb2m10 closed our earlier PR 534 after
  upstream's own 1.1.0 refactor, and Jim decided not to pursue an upstream PR for now. See
  deviations.md, 2026-10-31.
- [ ] Release checklist, since the accessible build is now meant for other VoiceOver and
  screen reader users, not just Jim. See deviations.md, 2026-10-31:
- [ ] Turn on GitHub Actions for the jhomme/dexedAccessible fork; it's never run there.
  Not started.
- [ ] Decide a version-numbering scheme for fork releases, separate from upstream's own
  numbers, since upstream is already at a 1.1.0 that's a different codebase. Not started.
- [ ] Decide on Mac code signing for release installers: get an Apple Developer ID to sign
  them, or ship unsigned with instructions for getting past Gatekeeper. Not started.
- [ ] Commit the outstanding working-tree changes before tagging anything. Not started.
- [x] First real GitHub Actions run on the fork surfaced two real build breaks: `JUCE_JACK=1`
  was set unconditionally in `Source/CMakeLists.txt`, so Windows failed looking for a Linux-only
  header, and the macOS job fails up front importing a code-signing certificate that doesn't
  exist on this fork. Fixed the JACK one: gated it to Linux only, matching how `JUCE_ALSA`
  already checks the platform. Built and ran DexedTests locally, confirmed arm64, both passing.
  The macOS signing-import failure is still open; see the next item.
- [ ] Fix the macOS CI job failing outright on the code-signing import step, since this fork
  has no Apple signing certificate. Needs the workflow to skip that step instead of failing
  when the secret isn't set. Not started.
- [ ] Tag and push the first release, once the above and Jim's VoiceOver pass are done.
  Not started.
- [ ] Track two, after the release is out: build a standalone Dexed user guide on a new
  branch off `accessibility`, covering the "mixed" and "fully relevant" manual sections, with
  a glossary for newcomers to synthesis. Not started.
