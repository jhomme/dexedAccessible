# Bookmark

## Where we are
- Simplified Shift+arrow "move by 5": now a plain add-5 or subtract-5 from wherever the
  value is, clamped at that slider's own min and max, on every slider in the app. It used to
  only work on 0-to-99 sliders, and snapped to the nearest multiple of 5 instead of moving a
  fixed amount. Built standalone, confirmed arm64, ran DexedTests, both passing. Still needs
  Jim's VoiceOver test.
- Fixed the envelope and pitch envelope diagram double-announcement. `EnvDisplay` and
  `PitchEnvDisplay` were separately telling VoiceOver about a change every time one of their
  own sliders moved, racing the slider's own announcement. Removed the diagram's own live
  announcement, keeping the slider's. Built standalone, confirmed arm64, ran DexedTests, both
  passing. This fix stays.
- Jim then found more sliders in Operator 1 that still double-speak in standalone: EG level 1
  through 3, EG rate 3, AMP MOD SENS, output level, L and R scale depth, and rate scaling.
  Jim's call: stop chasing standalone double-speak altogether. Most real users will run Dexed
  with VoiceOver inside Logic, and Logic doesn't speak live value changes at all right now
  regardless, so a standalone-only double-speak bug isn't reaching them. Recorded in
  docs/deviations.md.
- Dropped "acceptable to ship" from open questions; Jim will decide that himself.
- Still parked, not forgotten: the Logic-silence investigation (VoiceOver stays quiet on a
  live slider change in Logic), and the second unexplained "arrow keys briefly did nothing"
  episode. Full detail on both in docs/log.md.

## What's next
- Jim to test the move-by-5 simplification with VoiceOver: Shift+arrow should add or
  subtract 5 from wherever the value is, on any slider, not just ones that go 0 to 99.
- After that, slider naming cleanup: work out which sliders need clearer or more consistent
  spoken names, across `OperatorEditor.cpp` and `GlobalEditor.cpp`.

## Open questions
- Why VoiceOver stays silent on a live slider change in Logic: parked, unresolved, not a
  priority right now.
- The arrow-key hiccup in Logic, still open, not yet investigated.
