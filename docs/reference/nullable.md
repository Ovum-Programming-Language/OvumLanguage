# Nullability (Kotlin-style)

* Append `?` to make a type **nullable**: `Int?`, `String?`, `Point?`.
* **Safe call**: `expr?.Method()` calls only if `expr != null`; otherwise yields `null` (if the method returns a reference type) or a sensible default for chaining to Elvis.
  Example: `name?.ToString()?.Length() ?: 0`
* **Elvis**: `a ?: b` evaluates to `a` if non-null, else `b`.
* **Non-null assertion**: `x!!` throws an unhandleable error if `x == null`.
* **Cast to Bool**: any value can be explicitly cast to `Bool`.

  * Primitives: zero → `false`, non-zero → `true`.
  * Non-primitives: `true` iff the reference is a valid (non-null, live) object.
