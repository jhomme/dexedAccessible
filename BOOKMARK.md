# Bookmark

## Where we are
Slider and diagram accessibility fixes from earlier this week are done: move-by-5 is
simplified and confirmed clean with VoiceOver on every slider, and the envelope/pitch
envelope diagram double-announcement fix is in. A handful of standalone-only double-speak
sliders, and the Logic-silence and arrow-key-stops-responding issues, are all parked per
Jim's decisions; detail in docs/deviations.md and docs/log.md.

Converted `docs/00_DX7E.txt`, the original DX7 manual, into Markdown at
`docs/dx7-manual.md`, fixing OCR errors as we went, some cross-checked against the manual's
own specifications table. Sorted every section into purely hardware, mixed, or fully
software-relevant, at `docs/dx7-manual-hardware-sections.md`. Removed the purely-hardware
sections (precautions, physical panel description, cartridge/battery/power-on mechanics, all
of Store/Save/Load, the MIDI cable section, the LFO block diagram, general hardware specs)
from `docs/dx7-manual.md`, confirmed with Jim first. Also removed `tools/ax-watch` and
`tools/event-watch` from the repo entirely; those were just diagnostic scripts used to debug
VoiceOver behavior, not part of Dexed, and the real copies live in the reusable `event-watch`
Claude Code skill outside this repo, confirmed before deleting anything.

Jim decided, 2026-10-26, to split the remaining work into two tracks instead of one: finish
accessibility work on `accessibility` first (tooltips included), then build a real,
standalone Dexed user guide afterward on its own branch. Detail in docs/Plan.md.

## What's next
Two tracks now, track one first:
1. Track one, on `accessibility`: pull a one-sentence description for every control, not
   just sliders, out of `docs/dx7-manual.md`, as source material for the slider naming
   cleanup and for tooltips. Then put those descriptions into slider tooltips and test with
   VoiceOver. Then open the PR for `accessibility`.
2. Track two, after that PR: branch off `accessibility` and build a standalone Dexed user
   guide, for every user, from the "mixed" and "fully relevant" sections in
   `docs/dx7-manual-hardware-sections.md`.

## Open questions
None right now.
