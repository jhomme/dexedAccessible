# Bookmark

## Where we are
- Built a small, reusable diagnostic tool, `tools/ax-watch`, a standalone command line
  program, not part of the plugin, that watches a chosen process's real macOS accessibility
  notifications from outside the app, the same way Apple's Accessibility Inspector does. It
  only reads: no ability to set a value, perform an action, or send a synthetic keystroke or
  click. Needed a one-time Accessibility permission grant to "The Chat Place," the app hosting
  this Claude Code session, which Jim approved and can revoke any time.
- Used it to answer the parked side issue from 2026-10-21: a plain arrow key and VoiceOver's
  own VO+Left/Right gesture send the identical accessibility notification either way in
  standalone. The two input methods are indistinguishable at the system level, so whatever
  makes VoiceOver stay quiet about a competing announcement during its own gesture is internal
  to VoiceOver, past what we can observe from outside.
- Used the same tool against Logic's `AUHostingServiceXPC_arrow` helper process, the one
  actually running Dexed's UI there. Found the system-level notifications are correct and
  complete during Logic's silent case too, ruling out our own code or the system's delivery of
  the notification as the cause. Comparing a plain arrow key, silent, against VoiceOver's own
  gesture, which spoke correctly, on the same loaded instance, points to VoiceOver only
  live-announcing notifications from whichever process the system treats as frontmost, Logic
  itself, not the helper process. Not fully proven; if right, this isn't fixable from inside
  Dexed.
- Flagged, not yet investigated: arrow keys briefly did nothing in Logic again partway through
  today's testing, the same symptom as the one-time 2026-10-21 episode, before starting to work
  on their own. Happened a second time now, still unexplained.
- `ax-watch` is meant to be reusable beyond this project, any Mac app where we're checking
  VoiceOver support. Plan is to turn it into a general skill once it's proven itself further.

## What's next
- Jim's theory: VoiceOver should step by 5 on VO+Right/Left and by 1 on VO+Shift+Right/Left,
  and VO+End/Home should jump to max/min, but Dexed and Musio, both JUCE apps, always step by
  1 on VO+Right/Left. Confirmed in JUCE's own code that it only ever uses one fixed step, with
  no Shift handling. Next session, test this properly across several sliders in Dexed, a
  native Logic instrument, and a non-Logic app like System Settings, with both VoiceOver
  gestures and real keystrokes, using `tools/ax-watch` together with Dexed's own `AxTrace` log.
  Full test plan in docs/Plan.md.
- Possible next check on the separate Logic-silence question: read which process VoiceOver
  currently treats as frontmost while sitting in the Dexed window inside Logic, a one-time
  lookup, not another live test, to confirm or rule out the frontmost-process theory directly.
- After the Logic-silence question: fix the already-understood double announcement on
  envelope and pitch envelope sliders, where the envelope diagram next to them also announces,
  every time, not just every other time, the same kind of fix as the LCD case.
- Later: revisit slider naming for clarity and consistency.
- See docs/log.md for the full history and reasoning behind all of the above.

## Open questions
- Does VoiceOver only live-announce notifications from the system's current frontmost process?
  That would explain Logic's silence as a side effect of Logic hosting the plugin in a separate
  helper process, not something fixable in Dexed. Not yet confirmed directly.
- Why did arrow keys briefly stop responding in Logic a second time, before working again on
  their own? Possibly related to Logic Magician, Jim's Keyboard Maestro macro set, being left
  on at the start of testing; not confirmed.
- The documented build command in CLAUDE.md, without `CMAKE_BUILD_TYPE=Release`, produces an
  x86_64-only binary on this Mac. Worked around with an extra CMake flag each time so far;
  needs a decision on what a plain local build should default to.
