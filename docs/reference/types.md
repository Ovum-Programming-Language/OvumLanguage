# Types

Ovum has a rich type system with primitive types and user-defined types. The type system is static and does not permit implicit type coercions (an `Int` won't automatically become a `Float` without an explicit cast, for example).

## Primitive Types

* `Int` (8 bytes) - 64-bit signed integer
* `Float` (8 bytes) - 64-bit floating-point number
* `Bool` - Boolean value (`true` or `false`)
* `Char` - single character
* `Byte` - 8-bit unsigned integer
* `Pointer` - raw memory address *(only meaningful in `unsafe` code)*

> Any primitive may have a **nullable** counterpart via `?` (e.g., `Int?`).

## Non-Primitive / Reference Types

* `String` *(reference type; not primitive)* - text data
* *Nullable* `Int?` (and all other primitive types) - `Int` or `null` - also passed by reference
* **Array classes (no templates / generics):**
  * For primitives: `IntArray`, `FloatArray`, `BoolArray`, `CharArray`, `ByteArray`, `PointerArray`
  * For objects: **`ObjectArray`**
  * **Convenience**: `StringArray` (array of `String`, used for `Main`)

## Passing Semantics

* **Primitive types**: passed by value (copied)
* **Reference types**: all user-defined and non-primitive types (including `String` and all arrays) are **passed by reference** (const by default)
* **Immutability**: By default, references are considered immutable (the function cannot rebind them or mutate the object's content unless allowed via `var`)

## Type System Characteristics

* **Static typing**: Every variable and expression has a type that is checked at compile time
* **No implicit conversions**: Explicit casting required between different types
* **Type safety**: Prevents type errors and provides better documentation of code intent
* **Nullable types**: Any type can be made nullable by appending `?`