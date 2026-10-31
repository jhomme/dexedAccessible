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

Checked how closely the remaining `docs/dx7-manual.md` content matches Dexed's actual code,
before trusting it as source material. Everything checkable matched: algorithm, feedback,
LFO, both mod sensitivities, EG rates/levels, output level, frequency coarse/fine, detune,
keyboard scaling, key transpose, portamento time, the mod wheel/foot/breath/after-touch
ranges, MIDI channel, Voice Init, and the internal/cartridge voice concept. Two real
divergences: pitch bend range is a genuine software expansion (independent up/down
parameters to 4 octaves each, not the manual's single 0-12), and EDIT RECALL has no code
equivalent at all. Master Tune's exact cents range is unconfirmed. Also found a live bug:
`Source/PluginParam.cpp` misspells the LFO "TRIANGLE" waveform as "TRIANGE." Full detail in
docs/Plan.md; the typo fix and the Master Tune check are both parked as separate items, not
done yet.

Pulled a one-sentence description for every control, not just sliders, out of
`docs/dx7-manual.md`. `docs/control-manual-mapping.md` holds the full version, descriptions,
manual sections, and value ranges included, kept as groundwork for track two's user guide.
`docs/tooltip-descriptions.md` holds the short, tooltip-ready version: each control's internal
code name, a suggested full-word title to replace the raw name VoiceOver currently falls back
to, and a one-sentence tooltip. That doc uses "preset" instead of the manual's "voice," avoids
circular definitions, manual citations, value ranges, physical-knob language, and dates, per
rules recorded in docs/Plan.md. Jim proofread it by hand and caught a couple of bad commas;
fixed those and did a full grammar pass.

## What's next
Two tracks now, track one first:
1. Track one, on `accessibility`: put the descriptions from `docs/tooltip-descriptions.md`
   into the actual code, `setTitle()` calls and tooltips in `Source/GlobalEditor.cpp`,
   `Source/OperatorEditor.cpp`, and `Source/ParamDialog.cpp`, then test with VoiceOver. Then
   open the PR for `accessibility`.
2. Track two, after that PR: branch off `accessibility` and build a standalone Dexed user
   guide, for every user, from the "mixed" and "fully relevant" sections in
   `docs/dx7-manual-hardware-sections.md`, with a glossary covering both DX7-specific terms
   and general synthesis vocabulary for newcomers.

## Open questions
None right now.
