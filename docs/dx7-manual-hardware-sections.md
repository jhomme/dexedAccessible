# DX7 manual: hardware vs. software relevance

A section-by-section pass over `docs/dx7-manual.md`, sorting each section by whether it's
about the physical DX7 hardware, the synthesis parameters Dexed actually implements, or a mix
of both. This is for Jim to review before deciding what to drop from the software
documentation and tooltip work.

## Purely about the original hardware

These describe the physical instrument: its switches, cartridges, cables, battery, and
casing. Nothing in them maps to anything Dexed does as software.

- Precautions (Location, Handling, Power Cord, Relocation, Connection, Cleaning, Save This
  Manual, Lightning, Other Appliances)
- DX7 Outline's numbered panel controls, items 1 through 9 (Volume knob, Data Entry keys and
  slider, Mode Select key, Display Panel, Voice/Parameter Select key, Voice Cartridge slot,
  Pitch Bend wheel, Modulation wheel, Keyboard) as physical descriptions, separate from the
  parameters they control
- Connections (Setting Up and Applying Power, Turn Power On)
- Play Mode's cartridge mechanics (inserting a cartridge, the A/B voice bank switch on the
  cartridge itself)
- Cartridge Formatting
- Battery Check
- Store/Save/Load, in full: Memory Protect, Storing Newly Created Voices, Saving Internal
  Voice Data, Loading Cartridge Data. All of it is about physical cartridges and the memory
  protect switch
- MIDI's Connecting the MIDI Cable section
- LFO Block Diagram (a wiring diagram, and the figure wasn't even in the source text)
- Specifications' general hardware specs: Keyboard, Connecting Terminal, Control Terminal,
  Others, Dimensions/Weight, Power Consumption, Accessories

## Mixed: has a real parameter inside hardware-specific instructions

These sections describe real synthesis parameters that Dexed does implement, but wrapped in
instructions for physical keys, cartridges, or MIDI cabling that don't apply to a plugin.
Worth keeping the parameter explanation, not the hardware procedure.

- Play Mode's voice selection (the idea of selecting a voice is relevant; the physical
  INTERNAL/CARTRIDGE key procedure isn't)
- Function Mode's Cartridge Save/Load (the idea of storing a voice carries over; the cartridge
  procedure doesn't)
- Function Mode's Modulation Wheel/Foot Controller/Breath Controller/After Touch (the RANGE,
  PITCH, AMPLITUDE, and EG BIAS parameters are real and Dexed has them; the specific hardware
  controllers, like the breath controller, aren't relevant to a plugin, though Dexed may still
  let a host's MIDI CC or aftertouch drive the same parameters)
- MIDI's Selecting the Receiving Channel, System Information, and Using MIDI sections (MIDI
  channel and sysex transmit/receive concepts exist in Dexed, but a plugin host handles MIDI
  routing, so the hardware-era channel-setting procedure likely doesn't apply)
- "Let's Actually Create a Voice," the tutorial (the parameter values and reasoning for the
  clarinet sound are useful worked examples; the button-pressing instructions aren't)

## Fully relevant to the software

These describe the actual synthesis parameters Dexed implements as sliders and controls.
This is the real source material for control descriptions and tooltips.

- FM Tone Generation, in full
- Edit Mode, in full: Operator On-Off/EG Copy, Algorithm, Feedback, LFO, Mod Sensitivity,
  Oscillator, EG, Keyboard Level Scaling, Keyboard Rate Scaling, Operator, Pitch EG, Voice Name
- Key Transpose
- Function Mode's Master Tune, Poly/Mono, Pitch Bend, Portamento, Edit Recall, and Voice Init
- Specifications' Voice Parameters and Function Parameters tables, as a compact cross-check
  of every parameter's name and range
