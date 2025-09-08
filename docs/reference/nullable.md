# Nullability (Kotlin-style)

This document describes how nullable types work in Ovum, including their restrictions and available operations.

## Creating Nullable Types

Append `?` to make a type **nullable**: `Int?`, `String?`, `Point?`. Nullable types are passed by reference and can hold either a value or `null`.

```ovum
val nullableInt: Int? = null
val nullableString: String? = "Hello"
val nullablePoint: Point? = Point(10, 20)
```

## Method Call Restrictions

**Important**: You cannot directly call methods on nullable types using `.` - you must use the safe call operator `?.` or non-null assertion `!!`.

```ovum
val nullableString: String? = "Hello"
// val length: Int = nullableString.Length()  // ERROR: Cannot call method directly on nullable
val safeLength: Int = nullableString?.Length() ?: 0  // Correct: Use safe call
val forcedLength: Int = nullableString!!.Length()    // Correct: Use non-null assertion
```

## Null Handling Operators

### Safe Call (`?.`)

`expr?.Method()` calls only if `expr != null`; otherwise yields `null` (if the method returns a reference type) or a sensible default for chaining to Elvis.

```ovum
val name: String? = null
val length: Int = name?.Length() ?: 0  // Returns 0 if name is null
val upper: String? = name?.ToUpper()   // Returns null if name is null
```

### Elvis Operator (`?:`)

`a ?: b` evaluates to `a` if non-null, else `b`.

```ovum
val nullableInt: Int? = null
val defaultValue: Int = nullableInt ?: 42  // Uses 42 if nullableInt is null

val nullableString: String? = null
val result: String = nullableString ?: "default"  // Uses "default" if nullableString is null
```

### Non-null Assertion (`!!`)

`x!!` throws an unhandleable error if `x == null`. Use with caution - only when you're certain the value is not null.

```ovum
val nullableInt: Int? = 42
val mustExist: Int = nullableInt!!  // Safe - nullableInt is not null

// val crash: Int = (null as Int?)!!  // ERROR: Will abort the program
```

## Type Casting

### Cast to Bool

Any value can be explicitly cast to `Bool`:

* **Primitives**: zero → `false`, non-zero → `true`
* **Non-primitives**: `true` iff the reference is a valid (non-null, live) object

```ovum
val nullableInt: Int? = null
val isNull: Bool = (nullableInt as Bool)  // false (null is falsy)

val someInt: Int? = 42
val isNotNull: Bool = (someInt as Bool)   // true (non-null is truthy)
```

## Chaining Operations

You can chain safe calls and Elvis operators for complex null handling:

```ovum
val person: Person? = getPerson()
val nameLength: Int = person?.Name?.Length() ?: 0

// Equivalent to:
val nameLength: Int = if (person != null && person.Name != null) {
    person.Name.Length()
} else {
    0
}
```

## Nullable Type Methods

All nullable types support the same operators but cannot directly call methods:

```ovum
val nullableString: String? = "Hello"
val nullableInt: Int? = 42

// Safe operations
val safeLength: Int = nullableString?.Length() ?: 0
val safeToString: String = nullableInt?.ToString() ?: "null"

// Unsafe operations (will crash if null)
val forcedLength: Int = nullableString!!.Length()
val forcedToString: String = nullableInt!!.ToString()
```

## Best Practices

1. **Prefer safe calls** over non-null assertions when possible
2. **Use Elvis operator** to provide sensible defaults
3. **Avoid non-null assertions** unless you're certain the value exists
4. **Chain operations** for cleaner null handling code
5. **Consider using `if` statements** for complex null checks instead of deeply nested safe calls
