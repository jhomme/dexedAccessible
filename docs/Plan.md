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
- [x] Add accessibility support for `AlgoDisplay`, the algorithm routing diagram, in
  `Source/AlgoDisplay.h` and `.cpp`. The ALGO dial already announced its own number, so the
  missing piece was the full operator breakdown, built from the DX7 engine's own routing
  table and made available on demand through the dial's help text, rather than spoken
  automatically. Jim confirmed with VoiceOver, 2026-10-15.
- [x] Walk through the whole plugin with VoiceOver, tab stop by tab stop, and name every
  control that announced no name. Found and fixed three: the algorithm dial, `9c0e188`, the
  mono or poly switch and the on-screen keyboard's two octave scroll buttons, `eedfb6b`. Jim
  confirmed each with VoiceOver and found no more, 2026-10-15.
- [x] Fix VoiceOver's interact command reporting the "Keyboard keys" area as having empty
  content. The on-screen keyboard was never marked as a focus container, so VoiceOver couldn't
  see its two scroll buttons were really there. Marked it one in `Source/PluginEditor.cpp` and
  added a plain description of what the keyboard is for. Jim confirmed with VoiceOver,
  2026-10-15.
- [x] Test today's batch of fixes, the algorithm dial, the mono mode switch, the keyboard
  scroll buttons, and the keyboard keys group content, in Logic, in both the VST3 and AU
  builds. Passed, 2026-10-15.
- [x] Give VoiceOver proper arrow-key stepping on the DX7's 0-to-99 parameters, in
  `DXSlider::keyPressed`, `Source/DXComponents.h`. Home and End jump to the lowest and
  highest value on every slider. On 0-to-99 sliders, Shift+Right or Shift+Down jump up to
  the next multiple of 5, Shift+Left or Shift+Up jump down to the previous multiple of 5,
  both clamped at the real min and max. Down acts like Right and Up acts like Left,
  everywhere, by Jim's choice. Jim confirmed the full behavior with VoiceOver, 2026-10-15.
- [x] Test the arrow-key stepping work in Logic, VST3 and AU. The stepping itself works
  correctly there, same as standalone. Jim confirmed, 2026-10-17.
- [x] Found why VoiceOver speaks a slider's value twice for one arrow-key press. The small LCD
  display at the bottom of the window updates its text on practically every parameter edit,
  and was separately telling VoiceOver about its own change at the same moment the slider
  told VoiceOver about its own change, two competing announcements for one keypress. Confirmed
  with the diagnostic log after adding the same logging to the LCD that sliders already had.
  Fixed by having `GlobalEditor::setParamMessage` stop announcing the LCD's change live, since
  the slider doing the changing already announces itself; the LCD still updates its text and
  still reads correctly on demand. `Source/DXComponents.h`, `Source/DXComponents.cpp`, and
  `Source/GlobalEditor.cpp`, 2026-10-21. Jim confirmed with VoiceOver in standalone,
  arrow-keying through `detune`, `opCoarse`, and `opFine`: single speech on all three now,
  2026-10-21.
- [x] Rebuilt VST3, AU, and CLAP with the same fix, confirmed arm64, and confirmed with
  VoiceOver in Logic's AU that `detune` speaks only once per arrow-key press now, 2026-10-21.
  Along the way, found that Logic's AU runs in a separate helper process, `AUHostingService`,
  with its own temp folder at `$TMPDIR/AUHostingService/Dexed`, not the plain `$TMPDIR/Dexed`
  standalone uses; needed for turning the diagnostic log on there. Also hit a confusing patch
  where arrow keys briefly did nothing at all in Logic, with no log line of any kind; turned
  out to be a one-time focus mix-up right after first opening the plugin window, not a real
  bug, since it resolved on its own and could not be reproduced afterward. See docs/log.md,
  2026-10-21.
- [x] Side issue, parked on 2026-10-21 for curiosity and education: why VoiceOver stays quiet
  about a competing announcement during its own VO+Left/Right gesture, but not during a plain
  arrow-key press. Built a small diagnostic tool, `tools/ax-watch`, that watches a process's
  real macOS accessibility notifications from outside the app, the same way Apple's
  Accessibility Inspector does, read-only, no ability to act on anything. Confirmed in
  standalone that a plain arrow key and VoiceOver's own gesture send the identical notification
  either way, one `AXValueChanged` per step, same shape. The two input methods are
  indistinguishable at this level, so whatever VoiceOver does differently is internal to
  VoiceOver, past what we can observe from outside. 2026-10-22, see docs/log.md.
- [x] Dug into why a slider's value change speaks live in standalone but stays silent in
  Logic. Using the same `ax-watch` tool against Logic's `AUHostingServiceXPC_arrow` helper
  process, confirmed the system-level notifications are correct and complete either way, not
  the cause. Comparing a plain arrow key, silent, against VoiceOver's own gesture, which spoke
  correctly, on the same instance, points to VoiceOver only live-announcing notifications from
  whichever process the system treats as frontmost, which is Logic itself, not the helper
  process actually running Dexed's UI. If right, this isn't fixable from inside Dexed; it comes
  from how Logic hosts the plugin in a separate process. Not fully proven yet. 2026-10-22, see
  docs/log.md.
- [ ] Confirm the Logic-silence theory by checking which process VoiceOver currently treats as
  frontmost while sitting in the Dexed window inside Logic, a one-time lookup with `ax-watch`
  or similar, not another live test.
- [ ] Second unexplained occurrence, not yet investigated: arrow keys briefly doing nothing at
  all in Logic before starting to work, same symptom as the one-time episode on 2026-10-21.
  Happened again on 2026-10-22. See docs/log.md for both occurrences.
- [ ] Jim's theory, from 2026-10-22, not yet tested: expected VoiceOver behavior for a blind
  Logic user is VO+Right/Left steps by 5, VO+Shift+Right/Left steps by 1, VO+End goes to the
  maximum, VO+Home goes to the minimum. In Dexed and in Musio, another JUCE app, VO+Right/Left
  always steps by 1 instead. Confirmed in JUCE's own macOS code,
  `juce_AccessibilitySharedCode_mac.mm`, `accessibilityPerformIncrement` and
  `accessibilityPerformDecrement`: JUCE always steps by the slider's own defined interval, 1
  for Dexed's DX7 parameters, with no separate coarse and fine step and no check for Shift at
  all. Not yet confirmed whether VO+Shift+Right/Left and VO+Home/End even reach JUCE in some
  other form, such as falling through as a real keystroke, since JUCE's increment and
  decrement code has no way to tell a modifier was held.
  Test plan for next session, don't assume the step size is always 5 and 1 for every slider,
  observe and compare instead:
  - Run every one of these on each control: VO+Right, VO+Left, VO+Shift+Right,
    VO+Shift+Left, VO+End, VO+Home, and the same six as real keystrokes, not VoiceOver
    gestures. Repeat each a few times per control, since clamping near the top or bottom of a
    range can look like a different step size even when it isn't.
  - Try this on several different sliders in Dexed, a native Logic instrument with several
    sliders, and a non-Logic, non-JUCE app, such as a System Settings slider, Sound or
    Trackpad or Accessibility Zoom, to separate generic macOS slider behavior from anything
    specific to Logic itself.
  - For Dexed specifically, run `tools/ax-watch` and Dexed's own `AxTrace` log at the same
    time. The watcher alone only shows that a value changed, not how; `AxTrace` already tells
    apart a real keystroke, including whether Shift was held, from VoiceOver's own action,
    which the watcher can't do on its own.
