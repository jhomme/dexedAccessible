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
- Started a fresh pass where Jim walks through the plugin with VoiceOver, tab stop by tab
  stop, calling out each control that announces no name, and we fix them one at a time. Three
  found and fixed so far, all confirmed with VoiceOver in the standalone app, none committed
  or pushed yet except the first:
  - The algorithm number dial itself had no spoken name. Added a title, "Algorithm", in
    `Source/GlobalEditor.cpp`. Committed and pushed, `9c0e188`.
  - The mono or poly switch, `monoMode` in `Source/GlobalEditor.cpp`, had no button text and
    no title, so VoiceOver just called it "button" with no name. Added a title, "Mono Mode".
  - A trickier one: a button sitting, in VoiceOver's order, between the "Global Parameters"
    group and the "Keyboard keys" group. Couldn't identify it from source code alone, so set
    up two system tools to find it directly: a screenshot of the running app, and a query of
    whatever control VoiceOver currently has focused, using the system's own tool for
    inspecting other apps. Both needed a privacy permission turned on for the terminal
    running Claude Code, Screen Recording for the first and Accessibility for the second.
    Jim granted both. With Jim parked on the control, the query showed a small, 12 by 90
    point button at the right edge of where the on-screen keyboard sits. That matches one of
    two scroll buttons JUCE, the framework this plugin is built on, adds automatically to
    each end of the keyboard, to shift the visible range of notes by an octave. JUCE never
    names these two buttons, and this is vendored library code, not something built for this
    project, so rather than edit the library, named both buttons from our own code in
    `Source/PluginEditor.cpp`, right after the keyboard is set up. They're always the
    keyboard's first two children, in a fixed order, so they can be reached safely from
    outside. Now announce "Scroll Keyboard Down an Octave" and "Scroll Keyboard Up an Octave".

## What's next
- Commit and push the mono mode title and the keyboard scroll button titles.
- Continue the walkthrough for any further unlabeled controls Jim finds.
- Optional: test the VST3 and AU builds in Logic too, the way the earlier crash fix was
  tested, though the standalone test already covers the behavior itself.

## Open questions
- None right now.
