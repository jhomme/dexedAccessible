This release adds Comma and Period as quick ways to move between the main sections of the
plugin, from issue #3:

- Comma jumps to the previous section (an Operator, Global Parameters, or the on-screen
  keyboard, when it's showing), Period to the next, cycling through all of them and
  wrapping around at either end, landing on that section's first control, the same as the
  existing Command+1-6/G/K jump shortcuts. Neither key needs a modifier. Pressing either
  one while the cartridge library is open does nothing but remind you to press Escape
  first, the same as those existing shortcuts.
- Testing this surfaced and fixed a real bug in the code those existing Command-key
  shortcuts also rely on, to remember which section to return focus to: stepping onto the
  on-screen keyboard, then stepping again, could get stuck there instead of continuing on.

Confirmed with VoiceOver: stepping forward and backward through every section, including
the keyboard, and the cartridge-library block.

This release still includes the CLAP typed-value fix from earlier releases, which has not
yet been confirmed tested in a real CLAP host such as Reaper.

Keyboard shortcuts, including these two, are listed in Documentation/Keybindings.md, also
attached to this release as a separate download.
