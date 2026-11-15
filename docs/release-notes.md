This release adds four accessibility improvements:

- The algorithm routing diagram now speaks its own description live, instead of only a
  static tooltip, and updates as the algorithm or an operator's on/off state changes.
  Command+Shift+A (Control+Shift+A on Windows) announces that same description from
  anywhere, without moving keyboard focus. Confirmed with VoiceOver in both Logic and the
  standalone app.
- The cartridge library's small status line now announces which cartridge file, if any, the
  current program is based on. Confirmed with VoiceOver in both Logic and the standalone
  app.
- Three panels, the Function Settings dialog, the microtuning display, and the program
  lists, now state an explicit group role, matching a fix already in place for Global
  Parameters and each Operator panel. This one only matters on Windows with NVDA, which has
  not yet been tested; it makes no audible difference on Mac.
- Tab and Shift+Tab now announce a panel's name when crossing into it, the same way the
  existing Command-key jump shortcuts already do. Confirmed with VoiceOver in both Logic
  and the standalone app.

As before, these announcements use a JUCE call that, on Windows, speaks directly through
Windows' own text-to-speech rather than through NVDA. NVDA itself won't yet announce any of
this; that routing is separate, unstarted work.

This release still includes the CLAP typed-value fix from the last release, which has not
yet been tested in a real CLAP host such as Reaper.

Keyboard shortcuts are listed in Documentation/Keybindings.md, also attached to this release
as a separate download.
