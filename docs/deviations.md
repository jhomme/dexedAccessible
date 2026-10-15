# Deviations from prd.md and Plan.md

## 2026-10-10: merged the crash fix branch into accessibility without waiting for upstream review
What changed: `fix/parameter-registration-crash` was merged directly into `accessibility`,
rather than waiting for upstream pull request asb2m10/dexed#534 to be reviewed first.

Why: asb2m10/dexed, the real project, moves slowly. Several pull requests have sat open for
over a year, and others were closed without being merged after months. Treating upstream
review as a requirement would have blocked all accessibility work indefinitely.

Who decided: recorded in the project's CLAUDE.md as a standing rule, not a one time call. From
now on, upstream review is never a dependency for progress on the `accessibility` branch.
