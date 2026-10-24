# Bookmark

## Where we are
- Went through the parked items one by one with Jim. Kept parked: the Logic-silence
  frontmost-process theory, the second unexplained arrow-key hiccup, and the envelope and
  pitch envelope double-announcement fix.
- Resolved the open question about a plain local build silently coming out x86_64 only. The
  real build folder already defaults to arm64 for everyday builds, and a Release build already
  produces a universal binary covering both arm64 and x86_64. No code change was needed;
  confirmed by building and checking with `file`, not just by reading code. Full detail in
  docs/log.md.
- Agreed process for releases: everyday test builds stay arm64 only, this Mac's native chip,
  and before shipping, build with `CMAKE_BUILD_TYPE=Release` and use Rosetta to run a
  VoiceOver check of the x86_64 half on this Mac.
- Jim chose to pick up slider naming cleanup next: making every slider's spoken name clearer
  and more consistent, including renaming some that already have a name.

## What's next
- Slider naming cleanup: work out which sliders need clearer or more consistent spoken names,
  across `OperatorEditor.cpp` and `GlobalEditor.cpp`.
- After that, still open: what "acceptable to ship" means to Jim, which bugs are must-fix
  versus nice-to-have.
- Still parked, not forgotten: the Logic-silence frontmost-process theory, the second
  unexplained "arrow keys briefly did nothing" episode, and the envelope and pitch envelope
  double-announcement fix.

## Open questions
- What does "acceptable to ship" mean to Jim: which bugs are must-fix, which are nice-to-have?
- The frontmost-process theory and the arrow-key hiccup in Logic, both still open, not yet
  investigated.
