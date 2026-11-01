# Bookmark

## Where we are
v0.1.0 is out. The tag is pushed, all three platforms (Mac, Windows, Linux) built, and a
real GitHub release exists with correctly named installer files for each platform, plus
checksums and a source tarball. See the release at
https://github.com/jhomme/dexedAccessible/releases/tag/v0.1.0.

Getting here took two tries. The first tag push built fine on all three platforms but the
job that creates the actual release failed: GitHub's automatic token only had read access,
so creating a release returned a permission error. Fixed by giving just that one job write
access in `.github/workflows/project-pipeline.yml`, then moving the `v0.1.0` tag onto the
fixed commit and pushing again. The retry succeeded end to end. See docs/log.md, 2026-11-01,
for the full account.

Also fixed, same session: the repo had Issues turned off, a typo in its description, and no
topic tags, all now corrected, since the announcement text tells people to open an issue.

## What's next
Jim wants to send out his announcement (announcement.md, gitignored, not committed) now that
the release is confirmed real. Nothing blocking that.

After the announcement goes out:
1. Track two: branch off `accessibility` and build a standalone Dexed user guide, for every
   user, from the "mixed" and "fully relevant" sections in
   `docs/dx7-manual-hardware-sections.md`, with a glossary covering both DX7-specific terms
   and general synthesis vocabulary for newcomers.
2. Jim said he'll get a real Apple Developer Certificate "soon," which would let future Mac
   releases be signed, removing the Gatekeeper warning. Not scheduled yet.

## Not pursuing an upstream PR for now
asb2m10 closed our earlier crash-fix PR after upstream's own incompatible refactor. Jim's
call: keep developing on this fork, revisit only if there's a natural pause worth spending on
it. See docs/deviations.md.
