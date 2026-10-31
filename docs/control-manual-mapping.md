# Control to manual mapping

This lists every interactive control in the Dexed plugin UI, built from the real control
names in `Source/GlobalEditor.cpp`, `Source/OperatorEditor.cpp`, and `Source/ParamDialog.cpp`,
with a one-sentence plain description of what each one does, and a note on which section of
`docs/dx7-manual.md` it came from. These sentences are the draft source for tooltips, not the
final tooltip text yet; wording may still change once we test with VoiceOver.

The operator panel, `Source/OperatorEditor.cpp`, is a single class used six times, once per
operator, confirmed at `Source/PluginEditor.h:37` with `OperatorEditor operators[6]`. All six
panels have identical controls, so each operator control below is listed once, not six times,
with "this operator" standing in for whichever of the six it is.

Counting every operator separately, the plugin has about 34 controls on the global panel, 22
controls per operator panel times 6 operators, and about 35 controls in the Function dialog,
for roughly 200 interactive controls total.

## Global panel, `Source/GlobalEditor.cpp`

- algo: selects one of the 32 preset algorithms, which set how the 6 operators connect as
  carriers and modulators. From Edit Mode's Algorithm section.
- feedback: sets how much of the feedback operator's output feeds back into itself, from 0 to
  7, with higher values adding more harmonics and noise-like character. From Edit Mode's
  Feedback section.
- lfoSpeed: sets the LFO's speed, from 0, slowest, to 99, fastest. From LFO's SPEED.
- lfoDelay: sets how long the LFO waits after a key is pressed before it starts affecting the
  sound, from 0, no delay, to 99, longest delay. From LFO's DELAY.
- lfoPitchDepth: sets how deeply the LFO modulates pitch, from 0, no effect, to 99, maximum
  vibrato depth. From LFO's PMD.
- lfoAmDepth: sets how deeply the LFO modulates amplitude, from 0, no effect, to 99, maximum
  tremolo depth. From LFO's AMD.
- lfoType: chooses the LFO's waveform, triangle, saw down, saw up, square, sine, or sample and
  hold. From LFO's WAVE. Note the code misspells TRIANGLE as TRIANGE in
  `Source/PluginParam.cpp`, a separate tracked bug.
- lfoSync: turns LFO key sync on or off. When on, the LFO restarts at the same point every
  time a key is pressed. From LFO's SYNC.
- oscSync: turns oscillator sync on or off. When on, all operators start from the same phase
  angle for every note. From Oscillator's SYNC.
- pitchModSens: sets how sensitive the voice is to pitch modulation from the LFO and
  controllers, from 0, no sensitivity, to 7, maximum. From Mod sensitivity's PITCH.
- transpose: shifts the keyboard's pitch up or down in semitones, centered on C3. From the Key
  Transpose section.
- pitchRate1 through pitchRate4: sets how fast the pitch envelope moves to its matching level,
  from 0, slowest, to 99, fastest. From Pitch EG's RATE.
- pitchLevel1 through pitchLevel4: sets the pitch envelope's level at that stage, from 0, 4
  octaves down, through 50, standard pitch, to 99, 4 octaves up. From Pitch EG's LEVEL.
- output: sets the overall output volume. Loosely matches Play Mode's "set the desired volume
  level," which is really about the physical knob rather than this parameter.
- tune: fine-tunes the overall pitch of the instrument to match other instruments. From the
  Master Tune section.
- cutoff: sets the resonant filter's cutoff frequency. No manual match; a Dexed addition with
  no equivalent on the original DX7.
- reso: sets the resonant filter's resonance amount. No manual match; a Dexed addition with no
  equivalent on the original DX7.
- monoMode: switches between polyphonic play, multiple notes at once, and monophonic play, one
  note at a time. From the Poly/mono section.
- initButton: resets the current voice to its basic initial settings, ready for building a new
  sound from scratch. From the Voice init section.
- parmButton: opens the Function dialog, where pitch bend, portamento, MIDI, and controller
  settings live. No single manual section; see the Function dialog list below.
- cartButton: opens the cartridge and program list, for choosing internal or cartridge voices.
  From Play Mode's cartridge voice selection.
- storeButton: stores the current edited voice into memory. From the surviving reference to
  storing voices in DX7 Outline and the Cartridge section; the full Store/Save/Load procedure
  was removed from the manual as hardware-only.
- programSelector: lists the available voices for selection. From Play Mode's voice selection.
- algoDisplay: shows a diagram of the current algorithm's operator connections and which
  operators are on or off. From the Edit Mode indicator figure.
- pitchEnvDisplay: shows a graph of the pitch envelope's current shape. From the Pitch EG
  section, shown as a graph instead of numbers.
- lcdDisplay: shows the plugin's current status message. No single manual match.
- vuOutput: shows the overall output level on a VU meter. No manual match, a software-only
  aid.
- aboutButton: opens the About box with version and credit information. No manual match.

## Operator panel, `Source/OperatorEditor.cpp`, applies to all 6 operators

- opLevel: sets this operator's output level, from 0 to 99, the main control for how loud or
  strong this operator is. From Operator's OUTPUT LEVEL.
- opCoarse: sets this operator's coarse frequency ratio, from 0.5 to 31 times the base pitch.
  From Oscillator's FREQUENCY COARSE.
- opFine: fine-tunes this operator's frequency within its coarse setting. From Oscillator's
  FREQUENCY FINE.
- detune: detunes this operator slightly sharp or flat, from minus 7 to plus 7. From
  Oscillator's DETUNE.
- opMode: switches this operator between ratio mode, where pitch follows the keyboard, and
  fixed mode, where it stays at one frequency regardless of the key played. From Oscillator's
  MODE.
- s_egl1 through s_egl4: sets this operator's envelope level at that stage, from 0, silent, to
  99, maximum. From EG's LEVEL.
- s_egv1 through s_egv4, internally named egr1 through egr4: sets how fast this operator's
  envelope moves to its matching level, from 0, slowest, to 99, fastest. From EG's RATE.
- sclLeftLevel: sets how much this operator's level drops or rises for keys to the left of the
  breakpoint. From Keyboard level scaling's DEPTH.
- sclRightLevel: sets how much this operator's level drops or rises for keys to the right of
  the breakpoint. From Keyboard level scaling's DEPTH.
- sclLvlBrkPt: sets the keyboard breakpoint key that keyboard level scaling measures from, A-1
  to C8. From Keyboard level scaling's BREAK POINT.
- sclRateScaling: sets how much faster this operator's envelope runs for higher keys, from 0,
  no change, to 7, maximum. From the Keyboard rate scaling section.
- kbdLeftCurve: chooses the scaling curve shape for keys to the left of the breakpoint,
  negative linear, negative exponential, positive exponential, or positive linear. From
  Keyboard level scaling's CURVE.
- kbdRightCurve: chooses the scaling curve shape for keys to the right of the breakpoint,
  negative linear, negative exponential, positive exponential, or positive linear. From
  Keyboard level scaling's CURVE.
- keyVelSens: sets how much this operator responds to how hard a key is struck, from 0, no
  response, to 7, maximum. From Operator's KEY VELOCITY SENSITIVITY.
- ampModSens: sets how sensitive this operator is to amplitude modulation from the LFO and
  controllers, from 0 to 3. From Mod sensitivity's AMPLITUDE.
- opSwitch: turns this operator on or off. From Edit Mode's Operator on-off section.
- envDisplay: shows a graph of this operator's envelope shape. From the EG section, shown as a
  graph instead of numbers.
- vu: shows this operator's output level on a VU meter. No manual match, a software-only aid.
- khzDisplay: shows this operator's current frequency as a number. From the Oscillator
  section's frequency display figures.

## Function dialog, `Source/ParamDialog.cpp`

- pitchRangeUp: sets how far pitch bend can raise the pitch, in semitones. From Pitch bend's
  RANGE, with a real divergence already noted in `docs/Plan.md`: the manual describes one
  range, 0 to 12, while the software splits it into independent up and down ranges, each to
  48.
- pitchRangeDn: sets how far pitch bend can lower the pitch, in semitones. Same divergence as
  pitchRangeUp.
- pitchStep: sets pitch bend to move in fixed semitone steps instead of smoothly, from 0,
  smooth, to 12, whole-octave steps. From Pitch bend's STEP.
- whlRange: sets how strongly the modulation wheel affects pitch, amplitude, or envelope bias,
  from 0, no effect, to 99, maximum. From the modulation wheel's RANGE.
- ftRange: sets how strongly the foot controller affects pitch, amplitude, or envelope bias,
  from 0, no effect, to 99, maximum. From the foot controller's RANGE.
- brRange: sets how strongly the breath controller affects pitch, amplitude, or envelope bias,
  from 0, no effect, to 99, maximum. From the breath controller's RANGE.
- atRange: sets how strongly after touch affects pitch, amplitude, or envelope bias, from 0,
  no effect, to 99, maximum. From after touch's RANGE.
- whlPitch, ftPitch, brPitch, atPitch: turns on or off whether the modulation wheel, foot
  controller, breath controller, or after touch affects pitch. From each controller's PITCH
  setting.
- whlAmp, ftAmp, brAmp, atAmp: turns on or off whether the modulation wheel, foot controller,
  breath controller, or after touch affects amplitude. From each controller's AMPLITUDE
  setting.
- whlEg, ftEg, brEg, atEg: turns on or off whether the modulation wheel, foot controller,
  breath controller, or after touch affects envelope bias, adding volume or brightness
  variation. From each controller's EG BIAS setting.
- portamentoTm: sets the speed of the portamento or glissando slide between notes, from 0, no
  slide, to 99, slowest slide. From Portamento's Time.
- glissando: turns glissando on or off. When on, notes slide in semitone steps instead of
  smoothly. From Portamento's Glissando.
- sysexChl: sets the MIDI channel the plugin sends and receives sysex data on, from 1 to 16.
  From MIDI's receiving channel and the MIDI CHANNEL specification.
- sysexIn: chooses which MIDI device sysex data comes in from. No manual match; the manual
  assumes a physical MIDI cable, not a software device list.
- sysexOut: chooses which MIDI device sysex data goes out to. No manual match, same reason as
  sysexIn.
- engineReso: chooses the synthesis engine, modern, Mark I, or OPL series. No manual match, a
  Dexed-only option not on the original DX7.
- showKeyboard: shows or hides the on-screen keyboard. No manual match, a plugin display
  option.
- scalingFactor: sets the size of the plugin window as a percentage. No manual match, a plugin
  display option.
- transposeScale: chooses whether a 12-semitone transpose follows the keyboard or follows the
  length of a custom tuning scale. No manual match, part of Dexed's microtuning feature.
- transposeHelp: opens a help window explaining the transpose scale setting. No manual match.
- mpePBRange: sets the pitch bend range used for MPE, in semitones. No manual match, part of
  Dexed's MPE feature.
- mpeEnabled: turns MPE, per-note expressive control, on or off. No manual match, part of
  Dexed's MPE feature.
- sclButton: loads a scale file for microtuning. No manual match, part of Dexed's microtuning
  feature.
- kbmButton: loads a keyboard mapping file for microtuning. No manual match, part of Dexed's
  microtuning feature.
- showTunButton: shows the current tuning table. No manual match, part of Dexed's microtuning
  feature.
- resetTuningButton: resets tuning back to standard equal temperament. No manual match, part
  of Dexed's microtuning feature.
