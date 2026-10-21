# Bookmark

## Where we are
- Found and fixed the cause of the arrow-key double-speak: the small LCD display was
  separately announcing its own text change to VoiceOver on practically every parameter edit,
  at the same moment the slider announced its own change, two competing voices for one
  keypress. Fixed by making `GlobalEditor::setParamMessage` stop announcing the LCD's change
  live, since the slider already does. `Source/DXComponents.h`, `Source/DXComponents.cpp`,
  `Source/GlobalEditor.cpp`, 2026-10-21.
- Confirmed in standalone with VoiceOver: `detune`, `opCoarse`, and `opFine` all speak only
  once per arrow-key press now. Rebuilt standalone twice along the way, confirmed arm64 both
  times.
- Rebuilt VST3, AU, and CLAP with the same fix, confirmed arm64, and confirmed with VoiceOver
  in Logic's AU that `detune` also speaks only once now. Logic's AU runs the plugin in a
  separate helper process, `AUHostingService`, so its diagnostic log lives at
  `$TMPDIR/AUHostingService/Dexed`, not `$TMPDIR/Dexed` like standalone or `$TMPDIR/Logic Pro`
  as guessed before.
- Logic still doesn't speak a slider's or display's value live after an arrow-key move, even
  though the content and description are both correct; only the standalone app announces live.
  Untouched by today's fix.
- Hit a one-time scare while testing in Logic: arrow keys briefly did nothing at all, not even
  logging, while VoiceOver's own gesture still worked. Turned out to most likely be a
  navigation mix-up right after first opening the plugin window, not a real bug; it resolved
  on its own and couldn't be reproduced again. See docs/log.md, 2026-10-21, for the full story
  if it happens again.
- Side issue, parked for later out of curiosity, not blocking: why VoiceOver stays quiet about
  a competing announcement during its own VO+Left/Right adjust gesture, but not during a plain
  arrow-key press, even though the plugin sends the identical pair of notifications either
  way. See docs/log.md, 2026-10-21, for the reasoning so far.

## What's next
- Dig into why Logic never speaks a live value change at all, slider or LCD.
- After that: fix the already-understood double announcement on envelope and pitch envelope
  sliders, where the envelope diagram next to them also announces, every time, not just every
  other time, now that the LCD case shows the same pattern and the same kind of fix.
- Later: revisit slider naming for clarity and consistency.
- See docs/log.md for the full history and reasoning behind all of the above.

## Open questions
- Why does VoiceOver suppress a competing announcement during its own gesture-driven value
  change but not during a plain key-driven one? Parked for later, educational, not blocking.
- Why does a slider's or display's live value change speak in the standalone app but stay
  silent in Logic?
- The documented build command in CLAUDE.md, without `CMAKE_BUILD_TYPE=Release`, produces an
  x86_64-only binary on this Mac. Worked around with an extra CMake flag each time so far;
  needs a decision on what a plain local build should default to.
