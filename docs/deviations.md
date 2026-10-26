# Deviations from prd.md and Plan.md

## 2026-10-10: merged the crash fix branch into accessibility without waiting for upstream review
What changed: `fix/parameter-registration-crash` was merged directly into `accessibility`,
rather than waiting for upstream pull request asb2m10/dexed#534 to be reviewed first.

Why: asb2m10/dexed, the real project, moves slowly. Several pull requests have sat open for
over a year, and others were closed without being merged after months. Treating upstream
review as a requirement would have blocked all accessibility work indefinitely.

Who decided: recorded in the project's CLAUDE.md as a standing rule, not a one time call. From
now on, upstream review is never a dependency for progress on the `accessibility` branch.

## 2026-10-25: stopped chasing remaining double-speak reports in standalone
What changed: Plan.md had further double-speak investigation as open work. Jim stopped it
after today's envelope diagram fix, rather than continuing to chase newly reported
double-speak on other sliders, OP1's EG level 1 through 3, EG rate 3, AMP MOD SENS, output
level, L and R scale depth, and rate scaling.

Why: most real users will run Dexed with VoiceOver inside Logic, not the standalone app. In
Logic, live value-changed announcements don't speak at all right now, the separate,
still-unresolved Logic-silence issue, so a double-speak bug that only shows up in standalone
isn't reaching the people this project is for.

Who decided: Jim, 2026-10-25.

Also folded in: Pitch EG Level 1, found still double-speaking after testing the envelope
diagram fix, even though its code is now identical to Pitch EG Rate 1, which doesn't. Same
unexplained pattern as sclLeftLevel and sclRightLevel from 2026-10-19. Jim, 2026-10-25.

## 2026-10-26: stopped pursuing arrow keys stopping responding in Logic
What changed: Plan.md had a second, unexplained occurrence of arrow keys stopping
responding for a stretch, then starting to work again on their own, in Logic, from
2026-10-21 and again 2026-10-22, listed as not yet investigated. Jim decided not to pursue
this.

Why: Jim's call; no further reason given.

Who decided: Jim, 2026-10-26.

## 2026-10-26: stopped pursuing VoiceOver's silence on arrow-key presses in Logic
What changed: Plan.md had an open investigation into why VoiceOver stays silent when a
slider's value changes live from an arrow-key press in Logic, while the same arrow-key
press speaks correctly in standalone. Jim decided not to pursue this further.

Why: Jim's call; no further reason given.

Who decided: Jim, 2026-10-26.
