// A small diagnostic tool, not part of the Dexed plugin.
//
// It watches one running process for macOS accessibility notifications, the same
// system-level messages VoiceOver and Apple's Accessibility Inspector rely on, and
// prints each one as a plain text line. This lets us tell apart "our app never sent
// the notification" from "the system got it but VoiceOver chose not to speak it,"
// something that is otherwise invisible from inside the app itself.
//
// Build:
//   swiftc ax-watch.swift -o ax-watch
//
// Run, after granting this tool Accessibility access in System Settings, Privacy and
// Security, Accessibility:
//   ./ax-watch <pid> [notification-name ...]
//
// With no notification names given, it watches the three that matter most for
// screen reader work: a value changing, keyboard focus moving to a different
// control, and keyboard focus moving to a different window.

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

func attr(_ element: AXUIElement, _ name: String) -> String {
    var value: AnyObject?
    let result = AXUIElementCopyAttributeValue(element, name as CFString, &value)
    if result != .success || value == nil { return "" }
    if let s = value as? String { return s }
    if let n = value as? NSNumber { return n.stringValue }
    return String(describing: value!)
}

guard CommandLine.arguments.count > 1, let targetPid = pid_t(CommandLine.arguments[1]) else {
    print("Usage: ax-watch <pid> [notification-name ...]")
    print("Example: ax-watch 1234")
    print("Example: ax-watch 1234 AXValueChanged AXFocusedUIElementChanged")
    exit(1)
}

let defaultNotifications = [
    kAXValueChangedNotification,
    kAXFocusedUIElementChangedNotification,
    kAXFocusedWindowChangedNotification
]

let notifications = CommandLine.arguments.count > 2
    ? Array(CommandLine.arguments[2...])
    : defaultNotifications

let appElement = AXUIElementCreateApplication(targetPid)

let callback: AXObserverCallback = { _, element, notification, _ in
    let role = attr(element, kAXRoleAttribute)
    let title = attr(element, kAXTitleAttribute)
    let desc = attr(element, kAXDescriptionAttribute)
    let value = attr(element, kAXValueAttribute)
    let time = ISO8601DateFormatter().string(from: Date())
    print("\(time)  \(notification as String)  role=\(role)  title=\(title)  desc=\(desc)  value=\(value)")
    fflush(stdout)
}

var observer: AXObserver?
let createResult = AXObserverCreate(targetPid, callback, &observer)
guard createResult == .success, let obs = observer else {
    print("Could not create an observer for pid \(targetPid), result \(createResult.rawValue).")
    print("This usually means Accessibility access has not been granted to this tool yet.")
    exit(1)
}

for name in notifications {
    let addResult = AXObserverAddNotification(obs, appElement, name as CFString, nil)
    if addResult != .success {
        print("Could not watch \"\(name)\", result \(addResult.rawValue).")
    }
}

CFRunLoopAddSource(CFRunLoopGetCurrent(), AXObserverGetRunLoopSource(obs), .defaultMode)
print("Watching pid \(targetPid) for: \(notifications.joined(separator: ", "))")
print("Press Control-C to stop.")
CFRunLoopRun()
