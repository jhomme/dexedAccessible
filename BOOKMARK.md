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
  interacting with the keyboard area now lists its two scroll buttons by name. Committed and
  pushed, `e4a3be0`.
- Jim then tested today's whole batch of fixes in Logic. He first tried old VST3 and AU
  builds that hadn't been rebuilt since before today's work, so none of it was there yet,
  which was a good reminder that building the standalone app doesn't rebuild the other
  formats. Built and installed fresh VST3 and AU, both confirmed arm64. Worth flagging: both
  of those build targets install themselves straight into `~/Library/Audio/Plug-Ins`
  automatically, overwriting what was there, before there's a chance to list existing copies
  and ask first the way this project's rules normally call for. With the fresh builds in
  place, Jim confirmed everything works the same in Logic as in the standalone app.

- Gave VoiceOver proper arrow-key stepping on the DX7's 0-to-99 parameters, in
  `DXSlider::keyPressed` in `Source/DXComponents.h`. Home and End now jump to the lowest and
  highest value on every slider. On 0-to-99 sliders specifically, detected by range rather
  than by name, Shift+Right or Shift+Down jump up to the next multiple of 5, Shift+Left or
  Shift+Up jump down to the previous multiple of 5, both clamped at the true min and max.
  Down acts like Right and Up acts like Left, on every slider, the opposite of JUCE's
  default pairing. That direction swap, and dropping JUCE's old 10% Shift+Up/Down jump in
  favor of this, were deliberate choices Jim made after testing, not bugs.
- Along the way, confirmed two things about how VoiceOver and the plugin actually talk to
  each other, worth remembering for any future keyboard work on these dials: VoiceOver's own
  increase/decrease signal for a slider carries no information about Shift being held, so a
  Shift-modified step has to be built as an ordinary key press, the same way the old
  Shift+Up/Down 10% jump already worked; and although Apple's own documentation says Home
  and End are claimed globally by VoiceOver for moving its own cursor, the plain Home and End
  keys, without the VoiceOver modifier, do reach this plugin, confirmed by testing rather
  than assumed from the docs.
- Jim confirmed the full final behavior with VoiceOver in the standalone app. Built, confirmed
  arm64, ran DexedTests, still passing.
- Jim then tested the same build in Logic, with a freshly recreated Dexed track so Logic
  would pick up the new VST3 and AU. Arrow-key stepping itself works correctly there, same as
  standalone. But VoiceOver does not speak a slider's new value after an arrow-key move, in
  Logic, where it does in the standalone app.
- Went looking for the difference by reading code and comparing to the algorithm dial, which
  has its own spoken name and was confirmed working in Logic in an earlier session. Drafted a
  theory that the missing piece was a spoken name, since most of the plain parameter sliders
  never got one. Jim corrected this: in Logic, the algorithm dial doesn't speak its value
  change either, only its name. So a missing spoken name is not the cause, and that fix is
  set aside.
- Jim also asked for the sliders' spoken names to be made clearer and more consistent later,
  even if that means renaming ones that already exist. That's separate from this bug and
  comes after both the keyboard stepping and the speaking problem are settled.
- Jim tested the arrow-key stepping work in Logic today, confirmed it works there the same
  as standalone.
- While doing that, Jim found a second, separate problem: sliders sometimes speak the same
  value twice for one single action, in the standalone app. Confirmed with `detune`
  specifically, using plain arrow and Shift+arrow keys, pressed slowly, one at a time, always
  waiting for VoiceOver to finish before the next press. Not the VoiceOver modifier keys. Not
  every time. Always the exact same number both times, not two different readings of it.
- Reading the code found one real, deterministic cause, but only for some sliders: the
  envelope level and rate sliders, and the global pitch envelope sliders, each have code in
  `OperatorEditor.cpp` and `GlobalEditor.cpp` that fires a second, separate announcement on
  the envelope diagram next to them, every single time, on top of the slider's own normal
  one. That's real and worth fixing on its own, but it doesn't explain `detune`, which has no
  such second announcement anywhere in the code, yet still occasionally repeats.
- Reading code alone couldn't settle the `detune` case, since nothing in our code obviously
  fires twice; the only way to really know is to watch it happen. Worked out a plan to have
  the plugin write down, to a file, every keyboard and VoiceOver action it receives on a
  slider and what it did in response, switchable on and off without rebuilding, so the log
  can be read after Jim reproduces the problem. This will likely help with the Logic silence
  question below too, since that's also a question of whether an announcement fired. Plan is
  saved at `~/.claude/plans/i-turned-plan-mode-melodic-kahn.md`. Not built yet; Jim wants to
  pick this up together next time, rather than have it built while he's away.

## What's next
- Put the logging plan above into code, build, and have Jim reproduce the `detune` double-
  speak with it on, slowly, waiting for VoiceOver between presses, repeating a few times
  since it's occasional. Read the resulting log to see whether the plugin itself is firing
  the announcement twice, or whether it only shows once, which would point outside the
  plugin entirely, to VoiceOver itself. Clear the log file before each new attempt.
- Separately, fix the deterministic double announcement on the envelope and pitch envelope
  sliders, once the `detune` mystery is far enough along not to lose track of this one.
- Once the double-speak picture is clearer, come back to the still-open Logic question
  below, using the same logging if it helps: with VoiceOver on, in Logic, check whether
  `LcdDisplay` or an envelope display still speaks a live update there, the way they did when
  confirmed earlier. That tells us whether the Logic silence is specific to sliders or wider
  than that.
- After any fix is built: standalone, VST3, and AU, arm64 confirmed, existing copies in
  `~/Library/Audio/Plug-Ins` listed before any overwrite, Logic track recreated so it picks up
  the fresh build.
- Later, once the keyboard stepping, the double-speak, and the Logic speaking problem are all
  settled: revisit slider naming for clarity and consistency, along with other things Jim
  will raise then.

## Open questions
- Why does `detune`, and presumably other plain sliders, occasionally speak its value twice
  for one single key press, with no second announcement anywhere in the code? Needs the
  logging plan above to find out.
- Why does a keyboard-driven slider value change speak the new value live in the standalone
  app, but stay silent in Logic, even for the algorithm dial, which does have a spoken name?
  Needs the `LcdDisplay`/`EnvDisplay` diagnostic above before guessing at a cause.
