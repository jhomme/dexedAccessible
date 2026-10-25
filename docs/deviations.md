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
