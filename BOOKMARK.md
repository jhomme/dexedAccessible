# Bookmark

## Where we are
- Added VoiceOver support for `AlgoDisplay`, the algorithm routing diagram, in
  `Source/AlgoDisplay.h` and `.cpp`, plus the ALGO dial's handler in `Source/GlobalEditor.cpp`.
  The dial already announced its own number; the new piece is a full operator breakdown,
  built from the DX7 engine's own `FmCore::algorithms` routing table in
  `Source/msfa/fm_core.h`, kept as the dial's help text so VoiceOver speaks it on demand with
  VoiceOver plus shift plus H, rather than automatically on every algorithm change. Jim
  confirmed with VoiceOver in the standalone app. Committed and pushed, `e67e911`.
- Along the way, fixed a separate, unrelated gap: `portamentoTm`, the portamento time dial in
  the parameter dialog, had a tooltip but no spoken title, so VoiceOver reported it as
  unlabeled. Added the missing title in `Source/ParamDialog.cpp`. Same commit, `e67e911`.
- Learned that VoiceOver Utility has its own setting to always speak help tags automatically.
  Since this plugin only ever sets the tooltip text and never speaks it itself, that setting
  already gives automatic announcement to anyone who wants it, so we dropped the idea of
  building our own toggle for that.
- Added accessibility handlers for `EnvDisplay`, `PitchEnvDisplay`, and `LcdDisplay` in
  `Source/DXComponents.h`, following the `ProgramLabelAH` pattern. VoiceOver now gets a name
  and a description of current values when it lands on any of these three displays, and
  speaks up again live when a value changes, without needing to move away and back. Jim
  confirmed this with VoiceOver in the standalone app. Committed and pushed, `f15bc98`.
- These displays also repaint on a fast timer during playback, to animate the currently
  playing envelope segment. That path was deliberately left alone, so VoiceOver won't chatter
  nonstop while a note is held. The announcement only fires where an actual slider value
  changes, or where the LCD readout message changes.
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
  request 409). Posted a comment on each saying Jim intends to fold them into this fork as
  he's able. Committed and pushed, `d94b568`.
- Did a fresh pass where Jim walked through the plugin with VoiceOver, tab stop by tab stop,
  calling out each control that announced no name. Found and fixed three, all confirmed with
  VoiceOver, committed and pushed together with the docs, `9c0e188` and `eedfb6b`:
  - The algorithm number dial itself had no spoken name. Added a title, "Algorithm", in
    `Source/GlobalEditor.cpp`.
  - The mono or poly switch, `monoMode` in `Source/GlobalEditor.cpp`, had no button text and
    no title, so VoiceOver just called it "button" with no name. Added a title, "Mono Mode".
  - A small button at the right edge of the on-screen keyboard, one of two scroll buttons
    JUCE, the framework this plugin is built on, adds automatically to each end of the
    keyboard to shift the visible range of notes by an octave. Took some digging to find
    since it couldn't be identified from source code alone: used a screenshot of the running
    app and a direct query of whatever VoiceOver currently has focused, both needing a
    privacy permission turned on for the terminal running Claude Code, Screen Recording and
    Accessibility. Jim granted both. This is vendored JUCE code, not something built for this
    project, so rather than edit the library, named both buttons from our own code in
    `Source/PluginEditor.cpp`, right after the keyboard is set up, since they're always its
    first two children, in a fixed order. Now announce "Scroll Keyboard Down an Octave" and
    "Scroll Keyboard Up an Octave".
  - Jim confirmed there were no more unlabeled controls left to find.
- Right after that, Jim found a related but different problem: using VoiceOver's interact
  command on the "Keyboard keys" area itself said "content is empty," which sounds like a
  bug. The piano keys really don't have separate accessible objects behind them, they're only
  painted, and there's no way to play a note from the computer keyboard at all, only mouse,
  touch, or a real MIDI keyboard, which is what Jim actually uses. First tried giving the
  keyboard area an image role, which Jim correctly pointed out was its own kind of wrong for
  something you can play, and it didn't even fix the empty message. The real cause, found by
  reading JUCE's source, was that the keyboard was never marked as a focus container, the
  same flag `GlobalEditor` and `OperatorEditor` already set on themselves, so VoiceOver's
  interact command always treated it as having nothing inside, no matter what. Set that flag
  on `midiKeyboard` in `Source/PluginEditor.cpp` instead, and added a plain description,
  "Shows which notes are playing. Play notes with a MIDI keyboard or the mouse." Built,
  confirmed arm64, ran DexedTests, still passing. Jim confirmed with VoiceOver that
  interacting with the keyboard area now lists its two scroll buttons by name. Not committed
  or pushed yet.

## What's next
- Commit and push the keyboard keys group content fix once Jim says to.
- Optional: test the VST3 and AU builds in Logic too, the way the earlier crash fix was
  tested, though the standalone test already covers the behavior itself.

## Open questions
- None right now.
