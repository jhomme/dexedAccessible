# Dexed Accessible: what we're building

## What this is
Dexed Accessible is a fork of asb2m10's Dexed, a DX7 FM synth plugin. DX7 means the Yamaha
DX7, a 1980s synthesizer famous for FM, frequency modulation, sound. FM synthesis builds
sounds by using one tone to shift the pitch of another, very fast, rather than shaping a
single waveform. Dexed recreates that engine as a plugin, meaning a piece of software that
loads inside a music program instead of running on its own. It supports the common plugin
formats, VST3, AU, and CLAP, plus a standalone app that runs without a host program.

## Who this is for
The user, jhomme, is blind and uses VoiceOver, the Mac screen reader, with a keyboard only. He
can't reliably use a mouse. He wants to use Dexed the same way a sighted musician would, moving
between knobs and displays and hearing what each one is and what it's set to.

## Why we're doing this
Dexed's screen, like most synth plugin screens, is built from custom drawn controls. A sighted
user sees knobs, sliders, and diagrams. VoiceOver only knows about standard controls unless the
code tells it otherwise, so right now large parts of the screen are invisible to VoiceOver, or
announce as blank, unnamed elements. The goal is to add that missing information so VoiceOver
can describe every control Dexed has: its name, its role, such as slider or button, and its
current value, and to announce changes as they happen rather than only once.

## What's in scope
- Making the existing DX7 feature set in Dexed accessible: every knob, switch, display, and
  diagram.
- Keeping full compatibility with the original DX7. We are not adding new synthesis
  features, float parameters, or extra waveforms here. That kind of work belongs in a
  separate fork.

## What's out of scope
- New synthesis engine features or capability beyond the original DX7.
- Windows accessibility, NVDA, or JAWS support. This fork's accessibility work targets Mac and
  VoiceOver, since that's the platform jhomme uses. The user's general working rules call for
  NVDA and JAWS support on Windows projects, but this particular project is Mac only.

## Future enhancements
Ideas found while looking at open accessibility work in the upstream repo, asb2m10/dexed. Not
committed to Plan.md yet. Worth revisiting once the current plan's work is done.

- Fix the discoverability problem described in upstream issue 515, "accessibility: Loading DX7
  patches." A screen reader user couldn't find the cartridge load button, since it's labeled
  just "Cart" and sits far from the patch selector in keyboard tab order. The fix would be a
  clearer label and moving it closer to the patch selector.
  https://github.com/asb2m10/dexed/issues/515
- Consider porting upstream pull request 181, "Add option to keep keyboard focussed on the MIDI
  keyboard." Lets you browse patches with the arrow keys while the computer keyboard stays
  mapped to play notes, so you can hear each patch as you move through the list. Not merged
  upstream. https://github.com/asb2m10/dexed/pull/181
- Consider porting upstream pull request 409, "added octave up and down buttons." Adds buttons
  to shift the on-screen keyboard up or down an octave, reachable from a plain computer
  keyboard. Not merged upstream, and the author notes a known bug where shifting octaves while
  holding notes can leave notes stuck. https://github.com/asb2m10/dexed/pull/409
