// A small diagnostic tool, not part of the Dexed plugin.
//
// It watches the whole system for real keyboard events, key down and key up, and for
// real mouse events, button down, button up, and dragging, printing each one as a plain
// text line. It only listens. It cannot block, change, or inject a keystroke or a click,
// the same read-only limit ax-watch has for accessibility notifications.
//
// Build:
//   swiftc event-watch.swift -o event-watch
//
// Run, after granting this tool both Accessibility and Input Monitoring access in
// System Settings, Privacy and Security:
//   ./event-watch

import Foundation
import ApplicationServices

setbuf(stdout, nil)

let trustOptions = [kAXTrustedCheckOptionPrompt.takeRetainedValue() as String: true] as CFDictionary
if !AXIsProcessTrustedWithOptions(trustOptions) {
    print("Accessibility access has not been granted yet.")
    print("A system prompt may have just appeared asking to grant it.")
    print("If not, open System Settings, Privacy and Security, Accessibility, and turn on access")
    print("for the app this was run from, then run this again.")
    exit(1)
}

func modifierSummary(_ flags: CGEventFlags) -> String {
    var parts: [String] = []
    if flags.contains(.maskShift) { parts.append("shift") }
    if flags.contains(.maskControl) { parts.append("control") }
    if flags.contains(.maskAlternate) { parts.append("option") }
    if flags.contains(.maskCommand) { parts.append("command") }
    return parts.isEmpty ? "none" : parts.joined(separator: "+")
}

let eventMask = (1 << CGEventType.keyDown.rawValue)
    | (1 << CGEventType.keyUp.rawValue)
    | (1 << CGEventType.leftMouseDown.rawValue)
    | (1 << CGEventType.leftMouseUp.rawValue)
    | (1 << CGEventType.leftMouseDragged.rawValue)

guard let tap = CGEvent.tapCreate(
    tap: .cgSessionEventTap,
    place: .headInsertEventTap,
    options: .listenOnly,
    eventsOfInterest: CGEventMask(eventMask),
    callback: { _, type, event, _ in
        let flags = event.flags
        let time = ISO8601DateFormatter().string(from: Date())

        switch type {
        case .keyDown, .keyUp:
            let keyCode = event.getIntegerValueField(.keyboardEventKeycode)
            let kind = type == .keyDown ? "key down" : "key up"
            print("\(time)  \(kind)  code \(keyCode)  modifiers \(modifierSummary(flags))")
        case .leftMouseDown, .leftMouseUp, .leftMouseDragged:
            let location = event.location
            let kind = type == .leftMouseDown ? "mouse down"
                : type == .leftMouseUp ? "mouse up"
                : "mouse dragged"
            print("\(time)  \(kind)  at \(Int(location.x)),\(Int(location.y))  modifiers \(modifierSummary(flags))")
        default:
            break
        }

        return Unmanaged.passRetained(event)
    },
    userInfo: nil
) else {
    print("Could not start watching the keyboard, this usually means Input Monitoring access")
    print("has not been granted to this tool yet, in System Settings, Privacy and Security,")
    print("Input Monitoring.")
    exit(1)
}

let runLoopSource = CFMachPortCreateRunLoopSource(nil, tap, 0)
CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, .commonModes)
CGEvent.tapEnable(tap: tap, enable: true)

print("Watching every key press and mouse click on the system. Press Control-C to stop.")
CFRunLoopRun()
