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
- [ ] Investigated the Logic-silence theory further, 2026-10-24, and parked it unresolved; see
  docs/log.md for the full detail. Built `front-watch`, a new tool in the reusable `event-watch`
  skill, that logs every change in which app the system treats as frontmost. Running it while
  Jim arrow-keyed a Dexed slider inside Logic showed frontmost stayed Logic Pro the entire
  time, never switching to the helper process actually running Dexed's UI, which looked like
  it confirmed the theory. A follow-up check, with a second new tool, `ax-focus-watch`, asking
  a more specific question, which process the accessibility system itself treats as focused,
  complicated that: the same silent test showed Logic holding accessibility focus, not the
  helper process either, and a comparison speaking test, using VoiceOver's own gesture instead
  of a real key, showed neither process holding it at any point. That doesn't fit a clean
  story, so the theory is neither confirmed nor ruled out. Jim's call: stop investigating the
  mechanism further for now and move on to other work. Jim's call, 2026-10-26: not pursuing
  this at all now. See docs/deviations.md.
- [ ] Second unexplained occurrence: arrow keys briefly doing nothing at all in Logic before
  starting to work, same symptom as the one-time episode on 2026-10-21. Happened again on
  2026-10-22. See docs/log.md for both occurrences. Jim's call, 2026-10-26: not pursuing this.
  See docs/deviations.md.
- [x] Jim's theory, from 2026-10-22, since tested: expected VoiceOver behavior for a blind
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
- [x] Ran the test plan above, 2026-10-24. Confirmed in Dexed: VO+Right always steps by 1,
  matching `setRange(..., 1)` in the code. VO+Shift+Right makes VoiceOver's own "no action"
  sound and changes nothing, in Dexed and on a Control Center brightness slider alike. On a
  native Logic instrument slider, VO+Right steps by 5 and VO+Shift+Right steps by 1, a real
  difference, confirmed by pressing 3 times and seeing exactly 3 steps each way. Tried to find
  out how: built `tools/event-watch`, a read-only watcher for real system-wide keyboard and
  mouse events, and confirmed neither VO+Right nor VO+Shift+Right ever produces a real
  keystroke or a real mouse click or drag, on that Logic slider. Also checked, with a new
  one-shot tool, `tools/ax-watch/ax-actions`, what actions that slider advertises: just the
  ordinary `AXIncrement` and `AXDecrement`, nothing extra. So the Shift step-size difference is
  real, but the mechanism behind it is still unexplained. Separately, Dexed's own `AxTrace` log
  still would not write at all in today's standalone testing, despite the marker file, the temp
  folder path, and the compiled-in code all checking out; left unresolved. See docs/log.md.
- [x] Parked for now, Jim's call on 2026-10-24: understanding the exact VO+Shift mechanism is
  not worth more time right now. Priority moves to getting a version of Dexed Jim considers
  acceptable to ship. Revisit this if it becomes relevant again.
- [x] Closed, Jim's call on 2026-10-26: the step-size discussion and testing is done. Dexed
  steps by 1 on plain VO+Right/Left, matching its JUCE-based code; non-JUCE sliders step by 5.
  The mechanism behind the Shift difference stays unexplained but isn't being chased further.
  Nothing left open on this topic.
- [x] Confirmed Shift+arrow in Logic behaves the same as plain arrows: the value changes
  correctly, but VoiceOver stays silent. Jim tested directly in Logic, 2026-10-24. Extends the
  Logic-silence finding from 2026-10-22, which had only been confirmed for plain arrows there.
- [x] Resolved the open question about a plain local build silently coming out x86_64 only.
  The real build folder already defaults to arm64, confirmed by building and checking with
  `file`, and a Release build already produces a universal binary covering both architectures,
  also confirmed by building. No `CMakeLists.txt` change was needed. Agreed process going
  forward: everyday test builds stay arm64 only, and release builds get a VoiceOver check of
  the x86_64 half through Rosetta before shipping. 2026-10-24, see docs/log.md.
- [x] Fixed the envelope and pitch envelope diagram double-announcement. `EnvDisplay` and
  `PitchEnvDisplay` were each telling VoiceOver about their own change on top of the slider's
  own announcement, every time one of their sliders moved. Removed the diagram's own live
  announcement on its own sliders, keeping the slider's normal announcement, same pattern as
  the LCD fix. `Source/DXComponents.h`, `Source/OperatorEditor.cpp`,
  `Source/GlobalEditor.cpp`, 2026-10-25. Built and ran DexedTests, both passing.
- [ ] Jim then found more sliders in Operator 1 that still double-speak in standalone: EG
  level 1 through 3, EG rate 3, AMP MOD SENS, output level, L and R scale depth, and rate
  scaling. Parked, not investigated further. Jim's call, 2026-10-25: stop chasing standalone
  double-speak, since most real users will run Dexed with VoiceOver inside Logic, where live
  announcements don't speak at all right now regardless, the separate Logic-silence issue. See
  docs/deviations.md.
- [x] Simplified Shift+arrow "move by 5": it used to only work on sliders whose range is
  exactly 0 to 99, snapping to the nearest multiple of 5. Now it adds or subtracts a plain 5
  from the current value, clamped at that slider's own min and max, on every slider in the
  app. `Source/DXComponents.h`, 2026-10-25. Built and ran DexedTests, both passing. Jim
  confirmed with VoiceOver: clean on every slider tested, 2026-10-25.
- [x] Jim tested the envelope and pitch envelope fix with VoiceOver. Pitch EG Rate 1 now
  speaks once, confirming the fix. Pitch EG Level 1 still double-speaks, but the code for
  Rate 1 and Level 1 is now identical, neither calls the diagram's own announcement, so this
  isn't leftover from today's bug. Matches the same unexplained pattern as sclLeftLevel and
  sclRightLevel from 2026-10-19: identical code, different behavior. Folded into the same
  parked bucket as the rest of standalone double-speak; see docs/deviations.md. 2026-10-25.
- [ ] Make documentation for the software from `docs/00_DX7E.txt`, the original DX7 manual,
  as Jim directs step by step. In progress, 2026-10-26: converted the manual to Markdown at
  `docs/dx7-manual.md`, fixing OCR errors along the way, some cross-checked against the
  manual's own specifications table (for example, amplitude modulation sensitivity is 0 to 3,
  not the OCR'd "1 to 3"; operators are numbered 1 to 6, not "1 to 8"). The voice data list
  and MIDI data format appendices weren't in the source text; decided not worth chasing down,
  since the voice data list just repeats the specifications table and the MIDI format is
  already covered by `Documentation/sysex-format.txt`. Also sorted every section into
  purely hardware, mixed, or fully software-relevant, at
  `docs/dx7-manual-hardware-sections.md`, for Jim to review before deciding what to drop.
  Jim wants to talk over the results before continuing.
- [x] Decided, 2026-10-26, to split this into two separate tracks rather than one. Track one,
  on the `accessibility` branch: finish the remaining accessibility work, including pulling
  control descriptions for the slider naming cleanup and for tooltips, test with VoiceOver,
  then open the PR for `accessibility`. Track two, afterward, on a new branch off
  `accessibility`: build a real standalone user guide for Dexed, for every user, not just
  accessibility-focused source material. Jim's reasoning: a user guide is its own thing, not
  scratch notes for tooltips, and keeping it on its own branch keeps the accessibility PR
  focused on one concern. `docs/dx7-manual.md` and `docs/dx7-manual-hardware-sections.md`
  stay on `accessibility` for now since they already exist there; they'll get used once the
  docs branch starts.
- [ ] Track one: pull a one-sentence description for every control, not just sliders, out of
  `docs/dx7-manual.md`, as source material for the slider naming cleanup and for tooltips.
  Not started.
- [ ] Track one: put those descriptions into slider tooltips and test with VoiceOver, then
  open the PR for `accessibility`. Not started.
- [ ] Track two, after the `accessibility` PR: branch off `accessibility` and build a
  standalone Dexed user guide, covering the "mixed" and "fully relevant" sections from
  `docs/dx7-manual-hardware-sections.md`, for every user, not just accessibility source
  material. Not started.
- [x] Checked, 2026-10-26, how closely the remaining `docs/dx7-manual.md` content matches
  Dexed's actual code, parameter by parameter, before trusting it as source material. Every
  range and behavior that could be directly checked in `Source/PluginParam.cpp`,
  `Source/GlobalEditor.cpp`, and `Source/OperatorEditor.cpp` matched the manual: algorithm,
  feedback, LFO speed/delay/PMD/AMD, both mod sensitivities, EG rates and levels, output
  level, frequency coarse/fine, detune, keyboard scaling, key transpose, portamento time, the
  mod wheel/foot/breath/after-touch ranges, MIDI channel, Voice Init, and the internal vs
  cartridge voice concept. Two real divergences found: pitch bend range is a genuine software
  expansion, two independent up/down parameters each going to 0-48 (4 octaves each way)
  instead of the manual's single symmetric 0-12; and EDIT RECALL has no equivalent in the
  code at all, grepped for it and found nothing. Master Tune's exact cents range, the
  manual says plus or minus 75, couldn't be confirmed without tracing deeper into the
  synthesis engine's fixed-point math; left unresolved. Also found a live bug while checking:
  `Source/PluginParam.cpp` misspells the LFO waveform "TRIANGLE" as "TRIANGE," which
  VoiceOver will likely mispronounce. Parked for a future fix, not done tonight.
- [ ] Fix the "TRIANGE" typo for the LFO TRIANGLE waveform label in `Source/PluginParam.cpp`.
  Not started.
- [ ] Confirm Master Tune's actual cents range against `Source/msfa/dx7note.cc`'s fixed-point
  math; the manual says plus or minus 75 cents but this wasn't confirmed from code. Not
  started.
