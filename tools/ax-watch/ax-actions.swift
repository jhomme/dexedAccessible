// A small diagnostic tool, not part of the Dexed plugin.
//
// It asks one process a single read-only question: whatever accessibility element
// currently has VoiceOver's focus, what actions does it say it supports. It prints the
// list and exits. It cannot perform any of those actions itself.
//
// Build:
//   swiftc ax-actions.swift -o ax-actions
//
// Run, with VoiceOver's cursor already sitting on the control you want to ask about:
//   ./ax-actions <pid>

import Foundation
import ApplicationServices

setbuf(stdout, nil)

func attr(_ element: AXUIElement, _ name: String) -> String {
    var value: AnyObject?
    let result = AXUIElementCopyAttributeValue(element, name as CFString, &value)
    if result != .success || value == nil { return "" }
    if let s = value as? String { return s }
    if let n = value as? NSNumber { return n.stringValue }
    return String(describing: value!)
}

guard CommandLine.arguments.count > 1, let targetPid = pid_t(CommandLine.arguments[1]) else {
    print("Usage: ax-actions <pid>")
    exit(1)
}

let appElement = AXUIElementCreateApplication(targetPid)

var focused: AnyObject?
let focusResult = AXUIElementCopyAttributeValue(appElement, kAXFocusedUIElementAttribute as CFString, &focused)
guard focusResult == .success, let focusedElement = focused else {
    print("Could not read the focused element for pid \(targetPid), result \(focusResult.rawValue).")
    exit(1)
}

let element = focusedElement as! AXUIElement

print("role: \(attr(element, kAXRoleAttribute))")
print("title: \(attr(element, kAXTitleAttribute))")
print("description: \(attr(element, kAXDescriptionAttribute))")
print("value: \(attr(element, kAXValueAttribute))")

var actionNames: CFArray?
let actionsResult = AXUIElementCopyActionNames(element, &actionNames)
if actionsResult == .success, let names = actionNames as? [String] {
    print("actions: \(names.joined(separator: ", "))")
} else {
    print("Could not read action names, result \(actionsResult.rawValue).")
}
