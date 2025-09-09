# Types

Ovum has a rich type system with primitive types and user-defined types. The type system is static and does not permit implicit type coercions (an `Int` won't automatically become a `Float` without an explicit cast, for example).

## Primitive Types

### Numeric Types

* **`Int`** (8 bytes) - 64-bit signed integer
  * Literals: `42`, `-17`, `0x1A` (hex), `0b1010` (binary)
  
* **`Float`** (8 bytes) - 64-bit floating-point number (IEEE 754 double precision)
  * Literals: `3.14`, `2.0e10`, `1.5E-3`, `.5`, `5.`
  * Special values: `Infinity`, `-Infinity`, `NaN`

* **`Byte`** (1 byte) - 8-bit unsigned integer
  * Literals: `255`, `0x00`, `0b11111111`

### Character and Boolean Types

* **`Char`** - single Unicode character (UTF-32)
  * Literals: `'A'`, `'中'`, `'\n'`, `'\t'`, `'\0'`

* **`Bool`** - Boolean value (`true`, `false`)
  * Any value can be explicitly cast to `Bool`

### Low-Level Types

* **`Pointer`** - raw memory address *(only meaningful in `unsafe` code)*
  * Used for FFI and low-level memory operations

> **Nullable Primitives**: Any primitive type can be made nullable by appending `?` (e.g., `Int?`, `Float?`, `Bool?`). Nullable primitives are reference types.

## Reference Types

### Built-in Reference Types

* **`String`** - immutable text data (UTF-8 encoded)
  * Literals: `"Hello"`, `"Multi-line\nstring"`, `""` (empty string)
  * Concatenation: `"Hello" + " " + "World"`

* **`Object`** - root of all reference types
  * Implicit base class for all user-defined types
  * Contains only a virtual destructor

### Array Types

Ovum provides specialized array classes for different element types (no generics/templates):

**Primitive Arrays:**
* `IntArray` - array of `Int` values
* `FloatArray` - array of `Float` values  
* `BoolArray` - array of `Bool` values
* `CharArray` - array of `Char` values
* `ByteArray` - array of `Byte` values
* `PointerArray` - array of `Pointer` values

**Object Arrays:**
* `ObjectArray` - array of any `Object`-derived types
* `StringArray` - convenience array of `String` (used for `Main` function arguments)

**Array Creation:**
```ovum
val numbers: IntArray = IntArray(10)        // Create array of size 10
val names: StringArray = StringArray(5)     // Create string array of size 5
val objects: ObjectArray = ObjectArray(3)   // Create object array of size 3
```

## Type Aliases

Create type aliases for better code readability:

```ovum
typealias UserId = Int
typealias UserName = String

fun ProcessUser(id: UserId, name: UserName): Void {
    // Implementation
}
```


## Type Casting

### Explicit Casting

Use the `as` operator for explicit casting:

```ovum
val intValue: Int = 42
val floatValue: Float = (intValue as Float)  // Int to Float
val stringValue: String = (intValue as String)  // Int to String

val floatNum: Float = 3.14
val intNum: Int = (floatNum as Int)  // Float to Int (truncates)
```

### Boolean Casting

Any value can be explicitly cast to `Bool`:

```ovum
val intVal: Int = 42
val boolVal: Bool = (intVal as Bool)  // true (non-zero)

val zeroInt: Int = 0
val falseBool: Bool = (zeroInt as Bool)  // false (zero)

val nullString: String? = null
val nullBool: Bool = (nullString as Bool)  // false (null)
```

**Rules:** Primitives: zero → `false`, non-zero → `true`. References: `null` → `false`, non-null → `true`

### Unsafe Casting

Some casts require `unsafe` blocks:

```ovum
unsafe {
    val obj: Object = Point(10, 20)
    val bytes: ByteArray = (obj as ByteArray)           // Raw byte view
    val mutableBytes: ByteArray = (obj as var ByteArray) // Mutable byte view
}
```

## Passing Semantics

**Primitive types** are passed by value (copied):
```ovum
fun ModifyInt(x: Int): Void {
    x = x + 1  // Only modifies the local copy
}
```

**Reference types** are passed by reference:
```ovum
fun ModifyArray(arr: IntArray): Void {
    arr[0] = 999  // Modifies the original array
}
```

**Immutability:** References are immutable by default - use `var` for mutable references:
```ovum
fun CannotReassign(str: String): Void {
    // str = "New value"  // ERROR: Cannot reassign immutable reference
}

fun CanReassign(var str: String): Void {
    str = "New value"  // OK: str is mutable
}
```

## Type System Characteristics

**Static typing:** Every variable and expression has a type checked at compile time
**No implicit conversions:** Explicit casting required between different types
**Type safety:** Prevents many common errors
**Nullable types:** Any type can be made nullable by appending `?`

```ovum
val x: Int = 42
val y: String = "Hello"
// val z: Int = x + y  // ERROR: Cannot add Int and String

val intVal: Int = 42
val floatVal: Float = 3.14
val result: Float = (intVal as Float) + floatVal  // OK: Explicit conversion

val nullableInt: Int? = null
val nullableString: String? = "Hello"
```

## Pure Function Constraints

Types used as parameters in pure functions must implement `IComparable` for stable ordering:

```ovum
pure fun ProcessData(data: IComparable): Int {
    // data must implement IComparable for stable ordering
    return data.GetHash()
}
```

## Runtime Behavior

Type information is preserved at runtime for reference types:

```ovum
fun ProcessObject(obj: Object): Void {
    if (obj is String) {
        val str: String = (obj as String)!!
        sys::Print("String length: " + str.Length().ToString())
    } else if (obj is IntArray) {
        val arr: IntArray = (obj as IntArray)!!
        sys::Print("Array size: " + arr.Length().ToString())
    }
}
```
