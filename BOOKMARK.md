# Bookmark

## Where we are
- Added VoiceOver support for `AlgoDisplay`, the algorithm routing diagram, in
  `Source/AlgoDisplay.h` and `.cpp`, plus the ALGO dial's handler in `Source/GlobalEditor.cpp`.
  The dial already announced its own number; the new piece is a full operator breakdown,
  built from the DX7 engine's own `FmCore::algorithms` routing table in
  `Source/msfa/fm_core.h`, kept as the dial's help text so VoiceOver speaks it on demand with
  VoiceOver plus shift plus H, rather than automatically on every algorithm change. Jim
  confirmed with VoiceOver in the standalone app: the description for algorithm 1 matched the
  known structure, muting an operator is reflected correctly, and the dial stays quiet about
  operators until asked. Not committed or pushed yet.
- Along the way, fixed a separate, unrelated gap: `portamentoTm`, the portamento time dial in
  the parameter dialog, had a tooltip but no spoken title, so VoiceOver reported it as
  unlabeled. Added the missing title in `Source/ParamDialog.cpp`.
- Learned that VoiceOver Utility has its own setting to always speak help tags automatically.
  Since this plugin only ever sets the tooltip text and never speaks it itself, that setting
  already gives automatic announcement to anyone who wants it, so we dropped the idea of
  building our own toggle for that.
- Built the standalone app, confirmed arm64, and ran DexedTests, still passing. Have not yet
  tested the VST3 or AU builds in Logic for this change.
- Added accessibility handlers for `EnvDisplay`, `PitchEnvDisplay`, and `LcdDisplay` in
  `Source/DXComponents.h`, following the `ProgramLabelAH` pattern. VoiceOver now gets a name
  and a description of current values when it lands on any of these three displays, and
  speaks up again live when a value changes, without needing to move away and back. Jim
  confirmed this with VoiceOver in the standalone app. Committed and pushed.
- These displays also repaint on a fast timer during playback, to animate the currently
  playing envelope segment. That path was deliberately left alone, so VoiceOver won't chatter
  nonstop while a note is held. The announcement only fires where an actual slider value
  changes, or where the LCD readout message changes.
- Built the standalone app and confirmed arm64. Ran DexedTests, still passing. Have not yet
  built or tested VST3 or AU for this change.
- Logic test for the earlier crash fix and ComboBox cleanup passed. Jim confirmed, with
  VoiceOver on, that both hold up in Logic, not just the standalone build.
- Not waiting on PR #534 anymore. The accessibility branch already had the crash fix work
  independently, so it merged in cleanly without needing upstream to review anything.
- Set up the docs folder this project was missing: docs/prd.md, docs/Plan.md,
  docs/acceptance.md, docs/deviations.md, docs/log.md, and docs/postMortem.md. Backfilled from
  git history and CLAUDE.md.
- Looked through upstream, asb2m10/dexed, for accessibility work we don't have yet. Added a
  Future enhancements section to docs/prd.md: a discoverability fix for the cartridge load
  button (upstream issue 515), an option to keep keyboard focus on the on-screen keyboard
  while browsing patches (upstream pull request 181), and octave shift buttons (upstream pull
  request 409). All three are open or unresolved upstream, not on our Plan.md yet. Posted a
  comment on each saying Jim intends to fold them into this fork as he's able.
- Committed and pushed the docs updates. `d94b568`. `accessibility` and
  `origin/accessibility` are in sync.

## What's next
- Commit and push this AlgoDisplay work once Jim says to.
- Optional: test the VST3 and AU builds in Logic too, the way the earlier crash fix was
  tested, though the standalone test already covers the behavior itself.

## Open questions
- None right now.
