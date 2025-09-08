# Standard Interfaces & Object Model

* **`Object`**: implicit root; has only a **virtual destructor**.
  Enables safe, uniform storage (e.g., in `ObjectArray`).
* Standard interfaces (all implicitly extend `Object`):

  * **`IStringConvertible`** — `ToString(): String`
  * **`IComparable`** — `IsLess(other: Object): Bool`

    * Required for user-defined types used as parameters to **pure** functions (stable ordering/keys).
  * **`IHashable`** — `GetHash(): Int`

## Casting & Type Tests

* **Upcasts** (to `Object` or an implemented interface): **safe**, non-nullable result.
* **Downcasts** (to a concrete class or more specific interface): **nullable** result.

  * `obj as Foo` has type `Foo?`; it yields `null` if `obj` is not a `Foo`.
  * Use `!!` to assert non-null (aborts if `null`) or `?:` to handle null.
  * **Guideline**: Prefer `if (obj is Foo) { val f = (obj as Foo)!!; ... }`.
* **Type test**: `expr is Type` → `Bool` (use before downcasting).
* **Byte view casts**:

  * **Unsafe** cast of any value to **(const) `ByteArray`**.
  * **Unsafe** cast of any value to **mutable `ByteArray`**.
  * Require `unsafe { ... }`.

> **Naming**: **Classes, functions, methods use PascalCase** (e.g., `Main`, `ToString`, `IsLess`). Keywords/modifiers remain lowercase (`class`, `interface`, `var`, `override`, `pure`, `unsafe`, etc.).
