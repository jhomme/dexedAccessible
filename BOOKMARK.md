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
- Jim's call, 2026-10-26: not pursuing either of the two remaining Logic issues. One, where
  VoiceOver stays silent on a live slider change in Logic even though the value changes
  correctly. Two, where arrow keys stop responding for a stretch, then start working again on
  their own. Both recorded in docs/deviations.md; full investigation detail stays in
  docs/log.md.

## What's next
Jim wants to work through this in stages, directing each step himself. Three items, in
order:
1. Make documentation for the software from `docs/00_DX7E.txt`, the original DX7 manual, as
   Jim directs. Next up: a short brainstorm with Jim before starting.
2. Pull a one-sentence description for every control, not just sliders, out of
   `docs/00_DX7E.txt`, as source material for the slider naming cleanup and for tooltips.
3. Put those descriptions into slider tooltips and test with VoiceOver.

## Open questions
None right now.
