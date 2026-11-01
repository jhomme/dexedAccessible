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

## 2026-10-26: split the documentation work into two tracks instead of one
What changed: the plan had been to pull control descriptions from the DX7 manual as source
material for tooltips only. Jim decided this should become two separate tracks: finish the
remaining accessibility work on `accessibility`, including tooltips, and open that PR; then,
afterward, start a new branch off `accessibility` to build a real, standalone Dexed user
guide for every user, not just accessibility-focused source material.

Why: Jim realized the control descriptions are worth turning into real documentation for all
Dexed users, sighted or not, not just scratch notes for tooltips. Keeping that as its own
branch and its own PR, done after the accessibility PR, keeps each PR focused on one concern
instead of mixing code changes with a large documentation addition.

Who decided: Jim, 2026-10-26.

## 2026-10-31: shipping the Mac release unsigned for now
What changed: the Mac installer will go out without an Apple code-signing certificate for
now. Anyone downloading it has to click past a Gatekeeper security warning once. The CI
workflow's certificate-import step was changed to skip itself when no certificate secret is
present, rather than fail the whole Mac build outright, so the macOS job can actually finish
and produce an installer.

Why: Jim doesn't have an Apple Developer ID yet. Getting one costs money, about 99 dollars a
year, and setup time he didn't want to spend right now.

Who decided: Jim, 2026-10-31. He intends to get the certificate later; once the real signing
secrets are added to the fork's settings, the workflow will start signing automatically,
since the skip is conditional on the secret being missing, not a permanent change.

## 2026-10-31: widened who this project is for, and started a real release
What changed: docs/prd.md scoped this project to jhomme alone. Jim decided the accessible
build should go to other VoiceOver and screen reader users too, which means producing a real,
downloadable release instead of jhomme rebuilding it for himself each time.

Why: Jim's call; he wants this available to other people, not just himself.

Who decided: Jim, 2026-10-31.

## 2026-10-31: decided not to pursue an upstream pull request for now
What changed: Plan.md's "open the PR for accessibility" step, and bookmark.md's open question
about whether to propose this work to asb2m10/dexed, are both set aside for now. Jim will
keep developing accessibility work on this fork directly instead.

Why: asb2m10 closed PR 534, our earlier crash-fix pull request, on 2026-10-31, calling it "no
more applicable." The cause: upstream merged their own "Dexed 1.1.0 Refactor for JUCE 8" the
same day, moving the whole source tree from `Source` to `source` and splitting it into
subfolders, and replacing the parameter-handling code our accessibility fixes depend on.
Tested what reconciling now would actually take: simulating a real merge produced 20
conflicts, most of them files that no longer exist upstream at all rather than ordinary line
conflicts. Reconciling would mean redoing the accessibility work by hand against upstream's
new layout, not a rebase, so Jim chose to keep going on the fork instead of taking that on
right now.

Who decided: Jim, 2026-10-31.

## 2026-10-26: closed the VO+Shift step-size investigation unresolved
What changed: Plan.md had an open investigation into why VO+Shift+Right/Left steps a value by
1 in Dexed but by 5 on native Logic sliders. Testing on 2026-10-24 confirmed the difference is
real, not imagined, but the mechanism behind it stayed unexplained: it isn't a real keystroke,
a real mouse event, or an extra accessibility action name. Jim decided to stop investigating
the mechanism and close the topic rather than keep digging.

Why: Jim's call, to put priority on shipping a version of Dexed he considers acceptable,
rather than chasing full VoiceOver step-size parity with native macOS controls.

Who decided: Jim, 2026-10-24, confirmed closed 2026-10-26.

## 2026-10-26: removed the ax-watch and event-watch diagnostic tools from the repo
What changed: `tools/ax-watch` and `tools/event-watch`, Swift diagnostic scripts used across
several sessions to debug VoiceOver's accessibility notifications, were removed from the
repo entirely, along with their untracked compiled binaries and old test logs.

Why: these were debugging instruments for investigating VoiceOver behavior, not part of
Dexed itself, and don't belong in the plugin's repo. Confirmed before deleting anything that
the `event-watch` Claude Code skill, at `~/.claude/skills/event-watch/`, keeps its own
independent copies of every script, so nothing was lost; the skill remains usable on this or
any other project.

Who decided: Jim, 2026-10-26.
