# Bookmark

## Where we are
- Shift+arrow "move by 5" is done and confirmed. Now a plain add-5 or subtract-5 from
  wherever the value is, clamped at that slider's own min and max, on every slider in the
  app. Jim tested with VoiceOver: clean on every slider tried.
- The envelope and pitch envelope diagram double-announcement fix is in and mostly confirmed.
  `EnvDisplay` and `PitchEnvDisplay` no longer tell VoiceOver about a change themselves; the
  slider's own announcement covers it. Jim confirmed Pitch EG Rate 1 now speaks once. Pitch
  EG Level 1 still double-speaks, but its code is now identical to Rate 1's, so this isn't a
  leftover piece of today's bug, it's the same unexplained "identical code, different
  behavior" pattern seen with sclLeftLevel and sclRightLevel back on 2026-10-19.
- Jim's call: stop chasing double-speak in standalone altogether, including Pitch EG Level 1
  and the other sliders found double-speaking in Operator 1 (EG level 1 through 3, EG rate 3,
  AMP MOD SENS, output level, L and R scale depth, rate scaling). Most real users will run
  Dexed with VoiceOver inside Logic, and Logic doesn't speak live value changes at all right
  now regardless, so a standalone-only double-speak bug isn't reaching them. Recorded in
  docs/deviations.md.
- Dropped "acceptable to ship" from open questions; Jim will decide that himself.
- Still parked, not forgotten: the Logic-silence investigation (VoiceOver stays quiet on a
  live slider change in Logic), and the second unexplained "arrow keys briefly did nothing"
  episode. Full detail on both in docs/log.md.

## What's next
- Slider naming cleanup: work out which sliders need clearer or more consistent spoken names,
  across `OperatorEditor.cpp` and `GlobalEditor.cpp`.

## Open questions
- Why VoiceOver stays silent on a live slider change in Logic: parked, unresolved, not a
  priority right now.
- The arrow-key hiccup in Logic, still open, not yet investigated.
