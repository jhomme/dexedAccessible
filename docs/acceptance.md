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

## Mono mode switch title
Done when VoiceOver announces "Mono Mode" landing on the mono or poly switch in the main
window, instead of reporting it as an unnamed button.
- Jim tests: with VoiceOver on, tab to the mono mode switch and confirm it announces a name.
  Confirmed working, 2026-10-15.

## Keyboard scroll button titles
Done when VoiceOver announces "Scroll Keyboard Down an Octave" and "Scroll Keyboard Up an
Octave" on the two small scroll buttons at each end of the on-screen keyboard, instead of
reporting them as unnamed buttons.
- Jim tests: with VoiceOver on, tab to each end of the on-screen keyboard and confirm each
  scroll button announces a name. Confirmed working, 2026-10-15.

## Keyboard keys group content
Done when VoiceOver's interact command on the "Keyboard keys" area announces its two scroll
buttons by name, instead of reporting the group as empty. The on-screen piano keys themselves
stay mouse or MIDI keyboard only, since they aren't separate objects; this is about stopping
the group from sounding broken when there is real content inside it.
- Jim tests: with VoiceOver on, land on the "Keyboard keys" group and use the interact
  command. Confirm it lists the two scroll buttons by name instead of saying the content is
  empty. Confirmed working, 2026-10-15.

Jim also tested this whole batch of fixes, the algorithm dial, the mono mode switch, the
keyboard scroll buttons, and the keyboard keys group content, in Logic, in both the VST3 and
AU builds, not just the standalone app. Confirmed working, 2026-10-15.

## Arrow-key stepping on parameters
Done when Home jumps a slider to its lowest value and End to its highest, on every slider,
and Shift+arrow (or Shift+Up/Down) moves the value by 5 in the matching direction, clamped at
that slider's own min and max.
- Jim tests: with VoiceOver on, in the standalone app, press Home, End, and Shift+arrow on
  several sliders and confirm the jumps. Confirmed working, 2026-10-15.
- Jim tests: the same checks in Logic, VST3 and AU. Confirmed working, 2026-10-17.

## LCD and slider double-announcement
Done when moving a slider's value with the keyboard announces the new value once, not twice.
The LCD readout still updates its visible text at the same time, just without announcing that
change itself.
- Jim tests: with VoiceOver on, arrow-key through `detune`, `opCoarse`, and `opFine`, and
  confirm each press speaks once. Confirmed working in standalone, 2026-10-21, and in Logic,
  VST3 and AU, 2026-10-21.

## Envelope and pitch envelope diagram double-announcement
Done when moving an envelope or pitch envelope rate or level slider announces the new value
once, not twice.
- Jim tests: with VoiceOver on, move the rate and level sliders on the envelope and pitch
  envelope diagrams and confirm each speaks once. Confirmed working, 2026-10-25, with one
  known exception: Pitch EG Level 1 still double-speaks in standalone for a reason that was
  never found. Not being pursued further; see deviations.md. Standalone double-speak in
  general matters less than it might, since Logic doesn't speak live slider changes at all
  right now regardless.

## Real titles for every control
Done when every control in the plugin announces its own plain-language name when VoiceOver
lands on it, not its raw internal DX7 parameter name, such as `lfoSpeed` or `egl1`.
- Jim tests: with VoiceOver on, tab through every control in the global section, each
  operator panel, and the parameter dialogs, and confirm each announces a real name.
  Confirmed working, 2026-10-31.

## Tooltip text for every control
Done when VoiceOver's help command, VoiceOver plus shift plus H, speaks a plain-language
description of what a control does, for every slider, button, and combo box, without
repeating the control's own name or describing the original hardware's knob or switch.
- Jim tests: with VoiceOver on, use the help command on controls across the global section,
  each operator panel, and the parameter dialogs, and confirm each gives a useful
  description. Confirmed working, 2026-10-31.

## Keyboard scaling curve button labels
Done when the four keyboard scaling curve buttons speak "Linear decrease," "Exponential
decrease," "Exponential increase," and "Linear increase," instead of the abbreviations
"-LN," "-EX," "+EX," "+LN." The LCD readout keeps its own short abbreviations, left alone on
purpose for players who already know the original DX7.
- Jim tests: with VoiceOver on, open the keyboard scaling curve menu for an operator and
  confirm each of the four choices announces its spelled-out name. Confirmed working,
  2026-10-31.

## Algorithm diagram and VU meters hidden from accessibility
Done when the algorithm diagram and all VU meters, the main output meter and the six
per-operator meters, no longer appear to VoiceOver as empty, unexplained stops, since none of
their live readings currently reach VoiceOver.
- Jim tests: with VoiceOver on, tab through the main window and each operator panel, and
  confirm VoiceOver skips the diagram and the meters entirely rather than landing on an empty
  control. Confirmed working, 2026-10-31.

## Cartridge manager keeps VoiceOver inside the dialog
Done when VoiceOver's arrow-key navigation can't leave the cartridge manager dialog and land
on the operators, the global section, or the on-screen keyboard behind it, while the dialog
is open.
- Jim tests: with VoiceOver on, open the cartridge manager, arrow past its last control, and
  confirm VoiceOver stays inside the dialog instead of reaching the main window behind it.
  Confirmed working, 2026-10-31.

## Function Settings dialog title and first focus
Done when opening Function Settings announces "Function Settings," not an unnamed empty
group, and VoiceOver lands on the pitch bend range up slider first, not that empty group.
Also done when the Scale, Keyboard Mapping, Show Tuning, and Reset Tuning buttons each
announce their name once, not with the word "Button" said twice.
- Jim tests: with VoiceOver on, open Function Settings and confirm the dialog's own name,
  that focus lands on the pitch bend range up slider, and that all four buttons announce
  cleanly. Confirmed working, 2026-10-31.

## Keyboard octave scroll button tooltips
Done when VoiceOver's help command explains what the keyboard's scroll-down-an-octave and
scroll-up-an-octave buttons do.
- Jim tests: with VoiceOver on, use the help command on both buttons and confirm each
  explains what it does. Confirmed working, 2026-10-31.

Jim's full VoiceOver pass over this whole 2026-10-31 round, in Logic, is still pending. See
bookmark.md.

## Plugin identity rename, DexedAccessible instead of Dexed
Done when the plugin presents itself as DexedAccessible everywhere a host or the operating
system shows plugin identity, with no leftover Dexed or Digital Suburban codes that could
clash with the real Dexed, and when Apple's own AU validator, `auval`, passes every check.
- Jim tests: with VoiceOver on, open a new track's instrument list in Logic and confirm
  DexedAccessible appears there, distinct from the original Dexed if it's also installed.
  Confirmed working, 2026-11-01.

## About box reflects the rename and its credits text is readable
Done when the About button's accessible name says "About DexedAccessible," the dialog's own
title matches, the dialog's text opens by naming this as the accessible fork, the first link
points at this fork's GitHub repository instead of the original Dexed site, a second link
points at the releases/latest page, and the whole credits block, version, license, and
attributions, can be read by VoiceOver rather than only appearing as painted graphics.
- Jim tests: with VoiceOver on, activate the About button, confirm its name and the dialog's
  title, tab through all three links, and confirm the credits text itself is announced.
  Confirmed working, 2026-11-01.

## Cartridge manager: Home/End jump and list-name announcement
Done when Home jumps to the first program slot and End jumps to the last, slot 32, in either
the browser list or the active list inside the cartridge manager. Also done when VoiceOver
announces which list, browser or active, a slot belongs to whenever focus arrives there from
somewhere else, such as pressing Tab, but not when just arrowing between slots already
inside the same list.
- Jim tests: with VoiceOver on, open the cartridge manager, tab into each list and confirm it
  announces the list name, then press Home and End in each list and confirm it jumps to the
  first and last slot. Confirmed working by Jim. This was tested before it got written up
  here; the exact test date wasn't recorded at the time.
