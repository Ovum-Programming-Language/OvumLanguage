# Unsafe Operations

Unsafe operations bypass Ovum's safety guarantees and are allowed **only** inside `unsafe { ... }` blocks. These operations can lead to undefined behavior, memory corruption, or crashes if used incorrectly.

## Global and Static Variables

Global mutable state is unsafe because it can be accessed from anywhere:

```ovum
unsafe {
    global var globalCounter: Int = 0
    globalCounter = globalCounter + 1
    
    static var staticValue: String = "initial"
    staticValue = "modified"
}
```

## Const to Mutable Casting

Converting immutable references to mutable bypasses immutability guarantees:

```ovum
unsafe {
    val immutable: String = "Hello"
    val mutable: var String = (immutable as var String)
    // mutable can now be modified, breaking immutability
}
```

## Pointer Operations

Raw memory operations are inherently unsafe:

```ovum
unsafe {
    val obj: Point = Point(10, 20)
    val ptr: Pointer = &obj          // Address-of operator
    val deref: Object = *ptr         // Dereference to Object
    
    // Pointer arithmetic and manipulation
    val nextPtr: Pointer = ptr + 8   // Assuming 8-byte alignment
}
```

## Manual Destructor Calls

Destructors should only be called by the garbage collector:

```ovum
unsafe {
    val resource: FileHandler = FileHandler("file.txt")
    resource.destructor()  // Unsafe: manual destructor call
}
```

## Foreign Function Interface

Calling external native code is unsafe:

```ovum
unsafe {
    val input: ByteArray = "Hello".ToUtf8Bytes()
    val output: ByteArray = ByteArray(4)
    val result: Int = sys::Interope("libc.so", "strlen", input, output)
}
```

## ByteArray Casting

Raw byte access bypasses type safety:

```ovum
unsafe {
    val point: Point = Point(10, 20)
    val bytes: ByteArray = (point as ByteArray)           // Const byte view
    val mutableBytes: ByteArray = (point as var ByteArray) // Mutable byte view
    
    // Direct memory manipulation
    mutableBytes[0] = 0xFF
}
```

## Safety Guidelines

When using unsafe operations:

1. **Minimize scope** - Keep unsafe blocks as small as possible
2. **Validate assumptions** - Ensure pointers and casts are valid
3. **Document invariants** - Clearly document what makes the code safe
4. **Test thoroughly** - Unsafe code requires extensive testing
5. **Consider alternatives** - Use safe APIs when possible

## Common Unsafe Patterns

**Memory layout inspection:**
```ovum
unsafe {
    val obj: MyClass = MyClass()
    val bytes: ByteArray = (obj as ByteArray)
    // Inspect object's memory layout
}
```

**Low-level data conversion:**
```ovum
unsafe {
    val intValue: Int = 42
    val bytes: ByteArray = (intValue as ByteArray)
    // Convert Int to raw bytes
}
```

**Performance-critical operations:**
```ovum
unsafe {
    val largeArray: IntArray = IntArray(1000000)
    val bytes: ByteArray = (largeArray as ByteArray)
    // Direct memory operations for performance
}
```
