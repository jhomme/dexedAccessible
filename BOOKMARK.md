# Bookmark

## Where we are
v0.1.0 is still the only published release. Since then, a different Claude session, asked to
teach Jim how to publish a locally edited README, found that the plugin's build identity
still exactly matched the original Dexed: same manufacturer code, plugin code, and bundle ID.
That's a real clash risk for anyone with both the real Dexed and this fork installed. Jim
chose DexedAccessible as the new name, and that rename is now done, built, and tested.

What's done, all confirmed working with VoiceOver in Logic, 2026-11-01:
- Renamed the plugin's identity everywhere it lives: `Source/CMakeLists.txt`,
  `CMakeLists.txt`, both installer scripts, and two source files that picked folder and log
  names (`Source/PluginData.cpp`, `Source/PluginProcessor.cpp`). See docs/log.md and
  docs/deviations.md.
- Hit and fixed two local-machine problems along the way: a mismatched `cmake` that produced
  x86_64-only binaries, and a stale macOS plugin cache that kept the renamed AU from loading
  in Logic until a restart. See docs/log.md.
- Fixed the About box: button name, dialog title, swapped the first link from the original
  Dexed site to this fork's repo, added a link to the releases/latest page, and made the
  credits text readable by VoiceOver for the first time, since it had only ever been painted
  graphics.
- Bumped the version number to 0.2.0 in both places that carry one, since the rename breaks
  projects saved under v0.1.0.
- Added a paragraph to README.md and announcement.md pointing at the releases/latest page,
  instead of per-file links that go stale every release.

None of this is committed or pushed yet. Jim's VoiceOver testing of all of it came back
clean, nothing else found.

## What's next
1. Get Jim's go-ahead on the commit, then commit.
2. Tag and push `v0.2.0`, same release process as last time.
3. Send the announcement (announcement.md, gitignored, not committed) once the new release is
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
