# Built-in Reference Types

This document describes the built-in reference types in Ovum, their methods, and the standard interfaces that all types implement.

## String Type

`String` is a reference type (not primitive) for text data. All strings are immutable by default and implement `IStringConvertible`, `IComparable`, and `IHashable`.

### String Methods

* `Length(): Int` - Returns the number of characters in the string
* `ToString(): String` - Returns the string itself (implements `IStringConvertible`)
* `IsLess(other: Object): Bool` - Lexicographic comparison (implements `IComparable`)
* `GetHash(): Int` - Returns hash code for the string (implements `IHashable`)
* `+` operator - String concatenation (e.g., `"Hello" + " " + "World"`)

### String Usage

```ovum
val greeting: String = "Hello, World!"
val length: Int = greeting.Length()  // Returns 13
val combined: String = "Hello" + ", " + "World!"

// String comparison
val a: String = "apple"
val b: String = "banana"
val isLess: Bool = a.IsLess(b)  // true (lexicographic order)

// String hashing
val hash: Int = greeting.GetHash()
```

## Nullable Types

> **Note**: For detailed information about nullable types, see [`nullable.md`](nullable.md). This section only covers basic information.

Any primitive type can be made nullable by appending `?` (e.g., `Int?`, `String?`). Nullable types are passed by reference and can hold either a value or `null`.

**Important**: You cannot directly call methods on nullable types using `.` - you must use the safe call operator `?.`.

```ovum
val nullableString: String? = "Hello"
// val length: Int = nullableString.Length()  // ERROR: Cannot call method directly on nullable
val safeLength: Int = nullableString?.Length() ?: 0  // Correct: Use safe call
```

## Array Types

Ovum provides specialized array classes for different data types. Arrays are reference types and support indexing, iteration, and length operations. All array types implement `IStringConvertible`, `IComparable`, and `IHashable`.

### Primitive Arrays

* `IntArray` - Array of 64-bit signed integers
* `FloatArray` - Array of 64-bit floating-point numbers  
* `BoolArray` - Array of Boolean values
* `CharArray` - Array of characters
* `ByteArray` - Array of 8-bit unsigned integers
* `PointerArray` - Array of raw memory addresses (unsafe)

### Object Arrays

* `ObjectArray` - Array of any object type (implements `Object`)
* `StringArray` - Convenience array of `String` objects (used for `Main` function)

## File Type

`File` is a reference type for file operations. Files are nullable by default (`File?`) since file operations can fail. The `File` type implements `IStringConvertible`, `IComparable`, and `IHashable`.

### File Methods

* `ReadAllBytes(): ByteArray?` - Reads all bytes from the file, returns `null` on error
* `ReadAllText(): String?` - Reads all text from the file as UTF-8, returns `null` on error
* `WriteAllBytes(data: ByteArray): Bool` - Writes all bytes to the file, returns `false` on error
* `WriteAllText(text: String): Bool` - Writes all text to the file as UTF-8, returns `false` on error
* `AppendText(text: String): Bool` - Appends text to the file, returns `false` on error
* `Close(): Void` - Closes the file handle
* `IsOpen(): Bool` - Returns `true` if the file is currently open
* `GetSize(): Int?` - Returns file size in bytes, or `null` if error
* `ToString(): String` - Returns file path (implements `IStringConvertible`)
* `IsLess(other: Object): Bool` - Compares file paths lexicographically (implements `IComparable`)
* `GetHash(): Int` - Returns hash of file path (implements `IHashable`)

### File Usage

```ovum
// File operations
val file: File? = sys::OpenFile("data.txt", sys::FileMode::Read)
if (file != null) {
    val content: String? = file.ReadAllText()
    if (content != null) {
        sys::Print("File content: " + content)
    }
    file.Close()
}

// Writing to file
val outputFile: File? = sys::OpenFile("output.txt", sys::FileMode::Write)
if (outputFile != null) {
    val success: Bool = outputFile.WriteAllText("Hello, World!")
    if (success) {
        sys::Print("File written successfully")
    }
    outputFile.Close()
}

// File comparison
val file1: File? = sys::OpenFile("a.txt", sys::FileMode::Read)
val file2: File? = sys::OpenFile("b.txt", sys::FileMode::Read)
if (file1 != null && file2 != null) {
    val isLess: Bool = file1.IsLess(file2)  // Compares file paths
}
```

### Array Methods

All array types support the following methods:

* `Length(): Int` - Returns the number of elements in the array
* `[index]: ElementType` - Indexing operator for element access
* `[index] = value` - Assignment operator for mutable arrays
* `ToString(): String` - String representation of the array (implements `IStringConvertible`)
* `IsLess(other: Object): Bool` - Lexicographic comparison of array elements (implements `IComparable`)
* `GetHash(): Int` - Hash code based on array contents (implements `IHashable`)

### Array Usage

```ovum
// Creating and using arrays
val numbers: IntArray = IntArray(3)
numbers[0] = 10
numbers[1] = 20
numbers[2] = 30

val count: Int = numbers.Length()  // Returns 3

// Iteration
for (num in numbers) {
    sys::Print(num.ToString())
}

// Array comparison
val arr1: IntArray = IntArray(2)
arr1[0] = 1
arr1[1] = 2

val arr2: IntArray = IntArray(2)
arr2[0] = 1
arr2[1] = 3

val isLess: Bool = arr1.IsLess(arr2)  // true (lexicographic comparison)

// Array hashing
val hash: Int = numbers.GetHash()

// String array (used in Main)
fun Main(args: StringArray): Int {
    for (arg in args) {
        sys::Print("Argument: " + arg)
    }
    return 0
}
```

## Built-in Interfaces

All types in Ovum implicitly implement certain standard interfaces that provide common functionality.

### Object (Root Interface)

The implicit root interface for all types. Provides:
* `destructor(): Void` - Virtual destructor called by GC during finalization

### IStringConvertible

Provides string conversion capability:
* `ToString(): String` - Converts the object to its string representation

All built-in types implement this interface:
```ovum
val num: Int = 42
val str: String = num.ToString()  // "42"

val flag: Bool = true
val flagStr: String = flag.ToString()  // "true"
```

### IComparable

Provides ordering capability for sorting and comparison:
* `IsLess(other: Object): Bool` - Returns true if this object is less than the other

Required for user-defined types used as parameters to pure functions (ensures stable ordering).

```ovum
val a: Int = 5
val b: Int = 10
val isLess: Bool = a.IsLess(b)  // true
```

### IHashable

Provides hashing capability for use in hash tables and caching:
* `GetHash(): Int` - Returns a hash code for the object

```ovum
val text: String = "Hello"
val hash: Int = text.GetHash()
```

## Type Hierarchy

```
Object (implicit root)
├── IStringConvertible
├── IComparable  
└── IHashable

Built-in Types:
├── String (implements all interfaces)
├── File (implements all interfaces)
├── IntArray, FloatArray, etc. (implements all interfaces)
├── Int, Float, Bool, Char, Byte (implements all interfaces)
└── Int?, String?, File?, etc. (nullable versions, implements all interfaces)
```
