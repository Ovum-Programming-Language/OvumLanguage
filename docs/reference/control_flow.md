# Control Flow

Ovum supports standard control flow constructs following structured programming principles:

## Conditional Execution

**If/Else**: Conditional execution with syntax `if (condition) { ... } else { ... }`. The condition must be a Boolean expression (`Bool`). Braces are required for blocks, but for single statements the braces can be omitted (though using braces is encouraged for clarity).

```ovum
if (x > 0) {
    sys::Print("Positive")
} else if (x < 0) {
    sys::Print("Negative")
} else {
    sys::Print("Zero")
}
```

## Loops

**While Loop**: `while (condition) { ... }` repeats the body while the condition is true.

```ovum
var i: Int = 0
while (i < 10) {
    sys::Print(i.ToString())
    i = i + 1
}
```

**For Loop**: `for (item in collection) { ... }` iterates over elements of a collection (arrays, etc.).

```ovum
for (item in items) {
    sys::Print(item.ToString())
}
```

## Flow Control

**Return**: `return expression;` exits the current function with the given value (or `return;` with no value to exit a void function). In pure functions, a return simply provides the result; in impure, it may terminate early as usual.

**Break/Continue**: `break` exits a loop immediately, `continue` skips to the next iteration of the loop.

```ovum
var i: Int = 0
while (i < 10) {
    if (i == 5) {
        break  // Exit loop
    }
    if (i == 3) {
        i = i + 1
        continue  // Skip to next iteration
    }
    sys::Print(i.ToString())
    i = i + 1
}
```

All control flow follows structured programming principles (no `goto`).
