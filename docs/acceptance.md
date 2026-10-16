# Acceptance

How we'll know each piece of work is done. "Jim tests" means jhomme checks it himself with
VoiceOver on and a keyboard only. Nothing gets marked accessible until he's confirmed it.

## JUCE 8.0.15 upgrade and crash fix
Done when the plugin and standalone build and run without crashing on slider or control
interaction, in both the standalone app and inside a real host.
- Jim tests: in Logic, with VoiceOver on, move every slider and control that previously
  crashed, in both the VST3 and AU builds. Confirmed working, 2026-10-14.

## DexedTests regression suite
Done when `ctest` runs clean after a build, covering parameter registration.
- Jim tests: nothing to test by ear here. This is a code level safety net, not a behavior
  change.

## ComboBox accessibility workaround removal
Done when every combo box, including program and cartridge selectors, still announces its
name, its role, and its current value correctly after the workaround is removed.
- Jim tests: in Logic, with VoiceOver on, open every combo box and confirm the announcement is
  unchanged from before. Confirmed working, 2026-10-14.

## EnvDisplay, PitchEnvDisplay, and LcdDisplay accessibility
Done when VoiceOver, landing on one of these displays, announces a name and a description of
its current values, such as the envelope's attack, decay, sustain, and release settings, or the
LCD's current message. Also done when changing the value with a knob triggers a new
announcement right away, without needing to move away and back.
- Jim tests: with VoiceOver on, focus each display, confirm it announces current values, then
  turn a connected knob and confirm VoiceOver announces the change without needing to refocus.
  Confirmed working in the standalone app, 2026-10-14.

## AlgoDisplay accessibility
Done when the ALGO dial keeps announcing just its number automatically, with no extra
chatter when changing algorithms, and when VoiceOver's help command, VoiceOver plus shift
plus H, speaks a full description of the current algorithm on demand: which operators
modulate which, which are carriers, where the feedback loop is, and which operators are
muted. Also done when that description stays correct after muting or unmuting an operator,
without needing to leave and return to the dial.
- Jim tests: with VoiceOver on, in the standalone app, confirm the dial only announces its
  number while changing algorithms. Set algorithm 1, press the help command, and confirm it
  describes operator 6 with feedback down through operator 5, 4, and 3, a carrier, and
  operator 2 down to operator 1, a carrier. Mute an operator, return to the dial, and confirm
  the description reflects the mute. Confirmed working, 2026-10-15.

## Portamento time dial title
Done when VoiceOver announces "Portamento Time" landing on that dial in the parameter
dialog, instead of reporting it as unlabeled.
- Jim tests: with VoiceOver on, tab to the portamento time dial in the parameter dialog and
  confirm it announces a name. Confirmed working, 2026-10-15.

## Algorithm dial title
Done when VoiceOver announces "Algorithm" landing on the algorithm number dial in the main
window, instead of reporting it as unlabeled.
- Jim tests: with VoiceOver on, tab to the algorithm dial and confirm it announces a name.
  Confirmed working, 2026-10-15.
