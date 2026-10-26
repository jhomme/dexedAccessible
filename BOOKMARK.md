# Bookmark

## Where we are
Slider and diagram accessibility fixes from earlier this week are done: move-by-5 is
simplified and confirmed clean with VoiceOver on every slider, and the envelope/pitch
envelope diagram double-announcement fix is in. A handful of standalone-only double-speak
sliders, and the Logic-silence and arrow-key-stops-responding issues, are all parked per
Jim's decisions; detail in docs/deviations.md and docs/log.md.

Current thread, step 1 of the 3-stage documentation plan below: converted
`docs/00_DX7E.txt`, the original DX7 manual, into Markdown at `docs/dx7-manual.md`, fixing
OCR errors as we went, some cross-checked against the manual's own specifications table. The
voice data list and MIDI data format appendices weren't in the source file; decided not to
chase them down, since Dexed already has `Documentation/sysex-format.txt` for MIDI, and the
voice data list is just a blank worksheet repeating the specifications table. Also sorted
every section of the manual into purely hardware, mixed, or fully software-relevant, at
`docs/dx7-manual-hardware-sections.md`. Jim wants to talk over that sorting before deciding
what to carry into the software documentation and tooltips. Paused here, nothing more to do
until that conversation.

Also removed `tools/ax-watch` and `tools/event-watch` from the repo entirely. Those were
Swift diagnostic scripts used across several sessions to debug VoiceOver's accessibility
notifications, not part of Dexed itself, and they'd gotten swept into an earlier commit
without Jim intending it. Confirmed first that the reusable `event-watch` Claude Code skill,
at `~/.claude/skills/event-watch/`, keeps its own independent copies of every script, so
nothing was lost; the skill remains usable on this or any other project.

## What's next
Jim is working through this in stages, directing each step himself. Three items, in order:
1. Make documentation for the software from `docs/00_DX7E.txt`. In progress; paused for
   discussion of `docs/dx7-manual-hardware-sections.md`.
2. Pull a one-sentence description for every control, not just sliders, out of
   `docs/00_DX7E.txt`, as source material for the slider naming cleanup and for tooltips.
3. Put those descriptions into slider tooltips and test with VoiceOver.

## Open questions
None right now.
