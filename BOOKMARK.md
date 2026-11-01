# Bookmark

## Where we are
v0.2.0 is out and confirmed real, with all three platforms' installers, both checksum
files, and the source tarball, at
https://github.com/jhomme/dexedAccessible/releases/tag/v0.2.0.

This release renames the plugin away from the original Dexed. A different Claude session,
asked to teach Jim how to publish a locally edited README, found the plugin's build identity
still exactly matched the original: same manufacturer code, plugin code, and bundle ID, a
real clash risk for anyone with both installed. Jim chose DexedAccessible as the new name.
That rename is done, built, tested with VoiceOver in Logic, committed, and released. Full
account in docs/log.md and docs/deviations.md, entries dated 2026-11-01.

Along the way, also fixed: a mismatched local `cmake` that produced x86_64-only binaries, a
stale macOS plugin cache that kept the renamed AU from loading in Logic, two leftover "Dexed"
references in the About box that the rename missed on the first pass, the About box's
credits text never having been readable by VoiceOver at all, and a GitHub Actions macOS
runner image, `macos-14`, that GitHub retired the day after it broke this release's first
build attempt.

## What's next
1. Send the announcement (announcement.md, gitignored, not committed) now that v0.2.0 is
   confirmed real.

After that:
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
