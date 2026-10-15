# Plan

Tasks in order. Checked items are done.

- [x] Upgrade vendored JUCE to 8.0.15 and fix the resulting API breaks. `cfab624`, 2026-10-02.
- [x] Fix the crash that the JUCE upgrade introduced on any slider or control interaction.
  `6da8345`, 2026-10-09.
- [x] Add a headless regression test suite, DexedTests, so a break like that gets caught
  automatically next time. `d969d33` and `ea897f9`, 2026-10-09.
- [x] Open upstream pull request asb2m10/dexed#534 carrying the JUCE upgrade, the crash fix,
  and the test suite.
- [x] Merge that crash fix work into the `accessibility` branch directly, instead of waiting
  for PR 534 to be reviewed. `477cd03`, 2026-10-10. Recorded as a deviation; see
  docs/deviations.md.
- [x] Remove the temporary ComboBox accessibility workaround in `Source/DXComponents.h`, now
  that JUCE 8.0.15 fixes that bug internally. `6f99c9b`, 2026-10-10.
- [x] Test in Logic: build the VST3 and AU plugin formats, install them, and use VoiceOver to
  confirm the crash fix and the ComboBox cleanup both hold up outside the standalone build.
  Passed, 2026-10-14.
- [x] Add real accessibility handlers for `EnvDisplay` and `PitchEnvDisplay` in
  `Source/DXComponents.h`. These are custom drawn envelope diagrams with no name or role right
  now, so VoiceOver can't describe them. Follow the pattern already in the repo,
  `ProgramLabelAH` in `Source/ProgramListBox.h`: give VoiceOver a description of the current
  envelope values, and announce live updates when a value changes, not just the first time
  VoiceOver lands on the display. `LcdDisplay` gets the same treatment in the same change.
  Jim confirmed with VoiceOver in the standalone app, 2026-10-14.
- [ ] Add accessibility support for `AlgoDisplay`, the algorithm routing diagram, in
  `Source/AlgoDisplay.h` and `.cpp`. This is harder: describing six operators, feedback loops,
  and which operators carry the sound versus which shape it, in words, is its own design
  problem. It gets its own pass once the simpler displays prove the pattern out.
