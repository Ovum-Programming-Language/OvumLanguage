# Built-in Reference Types

* `String` *(reference type; not primitive)* - text data
* *Nullable* `Int?` (and all other primitive types) - `Int` or `null` - also passed by reference
* **Array classes (no templates / generics):**
  * For primitives: `IntArray`, `FloatArray`, `BoolArray`, `CharArray`, `ByteArray`, `PointerArray`
  * For objects: **`ObjectArray`**
  * **Convenience**: `StringArray` (array of `String`, used for `Main`)
