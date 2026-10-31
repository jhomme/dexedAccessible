# Tooltip descriptions

This is the tooltip-ready version of `docs/control-manual-mapping.md`. That doc stays as is,
full explanations, manual sections, and value ranges included, since it's useful groundwork
for the track two user guide. This doc is shorter and plainer, meant to go straight into the
code as each control's `setTitle()` name and tooltip text.

Rules followed here, from `docs/Plan.md`: no definition uses the word it's defining, no
manual section is named, no value range or option list is spelled out, since VoiceOver users
can explore those directly on the control, no control is described in terms of the original
hardware's physical knob or switch, and no date is mentioned for when a change was made. Also,
"preset" is used throughout instead of the DX7 manual's "voice," since "voice" already has a
different meaning on the DX7, a single playing note, as in "16-voice polyphony."

Each entry gives the control's internal code name, for finding it again in
`Source/GlobalEditor.cpp`, `Source/OperatorEditor.cpp`, or `Source/ParamDialog.cpp`, a
suggested full-word title to replace the raw internal name VoiceOver currently falls back to,
and the one-sentence tooltip text.

## Global panel

- algo: title "Algorithm." Chooses how the 6 operators connect to each other as carriers and
  modulators.
- feedback: title "Feedback." Controls how much of the chosen operator's output loops back
  into itself, adding harmonics and a noisier edge.
- lfoSpeed: title "LFO Speed." Sets how fast the low frequency oscillator cycles.
- lfoDelay: title "LFO Delay." Sets how long the low frequency oscillator waits after a key is
  pressed before it starts affecting the sound.
- lfoPitchDepth: title "LFO Pitch Modulation Depth." Sets how strongly the low frequency
  oscillator wavers the pitch for vibrato.
- lfoAmDepth: title "LFO Amplitude Modulation Depth." Sets how strongly the low frequency
  oscillator wavers the volume for tremolo.
- lfoType: title "LFO Waveform." Chooses the shape of wave the low frequency oscillator
  produces.
- lfoSync: title "LFO Key Sync." Turns on or off whether the low frequency oscillator restarts
  at the same point every time a key is pressed.
- oscSync: title "Oscillator Sync." Turns on or off whether every operator starts from the
  same point in its cycle for every note.
- pitchModSens: title "Pitch Modulation Sensitivity." Controls how strongly the low frequency
  oscillator and controllers can waver this preset's pitch.
- transpose: title "Transpose." Shifts the whole keyboard's pitch up or down.
- pitchRate1 through pitchRate4: title "Pitch EG Rate 1" through "4." Controls how quickly the
  pitch envelope moves toward its matching level.
- pitchLevel1 through pitchLevel4: title "Pitch EG Level 1" through "4." Sets the pitch
  envelope's target at that stage, above, below, or at standard pitch.
- output: title "Output Level." Sets the overall loudness of the sound coming out of the
  plugin.
- tune: title "Master Tune." Fine-tunes the overall pitch of the instrument to match other
  instruments.
- cutoff: title "Filter Cutoff." Sets the cutoff point of the resonant filter, a Dexed addition
  not on the original DX7.
- reso: title "Filter Resonance." Sets how much the resonant filter emphasizes frequencies
  near its cutoff point, a Dexed addition not on the original DX7.
- monoMode: title "Mono Mode." Switches between playing multiple notes at once and playing
  only one note at a time.
- initButton: title "Initialize Preset." Resets the current preset to its basic starting
  settings, ready for building a new sound.
- parmButton: title "Function Settings." Opens the dialog holding pitch bend, portamento,
  MIDI, and controller settings.
- cartButton: title "Cartridge." Opens the list of internal and cartridge presets for
  selection.
- storeButton: title "Store." Saves the current edited preset into memory.
- programSelector: title "Preset Selector." Lists the available presets for selection.
- algoDisplay: title "Algorithm Diagram." Shows how the 6 operators connect to each other and
  which ones are turned on.
- pitchEnvDisplay: title "Pitch Envelope Graph." Already has a real accessibility handler that
  describes the current envelope values in words. No change needed here.
- lcdDisplay: title "Status Display." Shows the plugin's current status message.
- vuOutput: title "Output Meter." Shows the overall output level.
- aboutButton: title "About." Opens information about the plugin's version and credits.

## Operator panel, applies to all 6 operators

- opLevel: title "Output Level." Sets how loud or strong this operator is.
- opCoarse: title "Frequency Coarse." Sets this operator's frequency as a broad multiple of
  the base pitch.
- opFine: title "Frequency Fine." Fine-tunes this operator's frequency within its coarse
  setting.
- detune: title "Detune." Shifts this operator slightly sharp or flat.
- opMode: title "Oscillator Mode." Switches this operator between following the keyboard's
  pitch and staying at one fixed frequency.
- s_egl1 through s_egl4: title "EG Level 1" through "4." Sets this operator's envelope target
  at that stage.
- s_egv1 through s_egv4, internally named egr1 through egr4: title "EG Rate 1" through "4."
  Controls how quickly this operator's envelope moves toward its matching level.
- sclLeftLevel: title "Keyboard Scale Level Left Depth." Sets how much this operator's level
  changes for keys below the breakpoint.
- sclRightLevel: title "Keyboard Scale Level Right Depth." Sets how much this operator's level
  changes for keys above the breakpoint.
- sclLvlBrkPt: title "Scale Level Breakpoint." Sets the keyboard key that keyboard level
  scaling measures from.
- sclRateScaling: title "Keyboard Rate Scaling." Controls how much faster this operator's
  envelope runs for higher keys.
- kbdLeftCurve: title "Keyboard Scaling Curve Left." Chooses the shape of level change for
  keys below the breakpoint.
- kbdRightCurve: title "Keyboard Scaling Curve Right." Chooses the shape of level change for
  keys above the breakpoint.
- keyVelSens: title "Key Velocity Sensitivity." Controls how much this operator responds to
  how hard a key is struck.
- ampModSens: title "Amplitude Modulation Sensitivity." Controls how strongly the low
  frequency oscillator and controllers can waver this operator's volume.
- opSwitch: already titled "Operator switch" in the code. Turns this operator on or off.
- envDisplay: title "Envelope Graph." Shows this operator's envelope shape.
- vu: title "Operator Meter." Shows this operator's output level.
- khzDisplay: title "Frequency Readout." Shows this operator's current frequency as a number.

## Function dialog

Most of these already have a `setTitle()` call in `Source/ParamDialog.cpp`; only the tooltip
text is new.

- pitchRangeUp: Sets how far pitch bend can raise the pitch.
- pitchRangeDn: Sets how far pitch bend can lower the pitch.
- pitchStep: Sets pitch bend to move in fixed steps instead of smoothly.
- whlRange: Sets how strongly the modulation wheel affects pitch, volume, or tone over time.
- ftRange: Sets how strongly the foot controller affects pitch, volume, or tone over time.
- brRange: Sets how strongly the breath controller affects pitch, volume, or tone over time.
- atRange: Sets how strongly after touch affects pitch, volume, or tone over time.
- whlPitch: Turns on or off whether the modulation wheel affects pitch.
- ftPitch: Turns on or off whether the foot controller affects pitch.
- brPitch: Turns on or off whether the breath controller affects pitch.
- atPitch: Turns on or off whether after touch affects pitch.
- whlAmp: Turns on or off whether the modulation wheel affects volume.
- ftAmp: Turns on or off whether the foot controller affects volume.
- brAmp: Turns on or off whether the breath controller affects volume.
- atAmp: Turns on or off whether after touch affects volume.
- whlEg: title suggestion "Wheel Envelope Bias." Turns on or off whether the modulation wheel
  can add brightness or loudness variation over time.
- ftEg: title suggestion "Foot Envelope Bias." Turns on or off whether the foot controller can
  add brightness or loudness variation over time.
- brEg: title suggestion "Breath Envelope Bias." Turns on or off whether the breath controller
  can add brightness or loudness variation over time.
- atEg: title suggestion "After Touch Envelope Bias." Turns on or off whether after touch can
  add brightness or loudness variation over time.
- portamentoTm: Controls how slowly one note slides into the next.
- glissando: Turns on or off whether notes slide in fixed steps instead of smoothly.
- sysexChl: Sets which MIDI channel the plugin sends and receives preset data on.
- sysexIn: Chooses which MIDI device preset data comes in from.
- sysexOut: Chooses which MIDI device preset data goes out to.
- engineReso: Chooses which synthesis engine generates the sound, a Dexed-only choice not on
  the original DX7.
- showKeyboard: Shows or hides the on-screen keyboard.
- scalingFactor: Sets the size of the plugin window.
- transposeScale: Chooses whether a 12 semitone transpose follows the keyboard or follows a
  custom tuning scale's length.
- transposeHelp: Opens an explanation of the transpose scale setting.
- mpePBRange: Sets the pitch bend range used for per-note expressive control.
- mpeEnabled: Turns on or off per-note expressive pitch, pressure, and timbre control.
- sclButton: Loads a scale file for custom tuning.
- kbmButton: Loads a keyboard mapping file for custom tuning.
- showTunButton: Shows the current tuning table.
- resetTuningButton: Resets tuning back to standard equal temperament.
