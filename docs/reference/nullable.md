# Nullability (Kotlin-style)

This document describes how nullable types work in Ovum, including their restrictions and available operations.

## Creating Nullable Types

Append `?` to make a **reference type** nullable: `Int?`, `String?`, `Point?`. **Fundamental types** (`int`, `float`, `bool`, `char`, `byte`, `pointer`) cannot be made nullable.

```ovum
val nullableInt: Int? = null
val nullableString: String? = "Hello"
val nullablePoint: Point? = Point(10, 20)

// val invalidNullable: int? = null  // ERROR: Fundamental types cannot be nullable
```

## Method Call Restrictions

**Important**: You cannot directly call methods on nullable types using `.` - you must use the safe call operator `?.`.

```ovum
val nullableString: String? = "Hello"
// val length: Int = nullableString.Length()  // ERROR: Cannot call method directly on nullable
val safeLength: Int = nullableString?.Length() ?: 0  // Correct: Use safe call
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


## Type Casting

### Cast to Bool

Any value can be explicitly cast to `bool`:

* **Fundamentals, primitive reference types**: zero → `false`, non-zero → `true`
* **Non-primitive reference types and nullable primitives**: `true` iff the reference is a valid (non-null, live) object

```ovum
val nullableInt: Int? = null
val isNull: bool = (nullableInt as bool)  // false (null is falsy)

val someInt: Int? = 42  // Implicit conversion from literal
val isNotNull: bool = (someInt as bool)   // true (non-null is truthy)

// Converting nullable primitives to fundamentals
val nullablePrimitive: Int? = 42  // Implicit conversion from literal
val fundamentalValue: int = (nullablePrimitive as Int) as int  // Two-step conversion
```

## Chaining Operations

You can chain safe calls and Elvis operators for complex null handling:

```ovum
val person: Person? = getPerson()
val nameLength: Int = person?.Name?.Length() ?: 0

// Equivalent to:
val nameLength: Int = if (person != null && person.Name != null) {
    val nonNullPerson: Person = person ?: Person("Unknown")  // Use Elvis operator
    val nonNullName: String = nonNullPerson.Name ?: "Unknown"  // Use Elvis operator
    nonNullName.Length()
} else {
    0
}
```

## Nullable Type Methods

All nullable types support the same operators but cannot directly call methods:

```ovum
val nullableString: String? = "Hello"
val nullableInt: Int? = 42  // Implicit conversion from literal

// Safe operations
val safeLength: int = (nullableString?.Length() ?: 0) as int  // Built-in returns Int
val safeToString: String = nullableInt?.ToString() ?: "null"
```

## Best Practices

1. **Always use safe calls** (`?.`) for nullable types
2. **Use Elvis operator** (`?:`) to provide sensible defaults
3. **Chain operations** for cleaner null handling code
4. **Consider using `if` statements** for complex null checks instead of deeply nested safe calls
5. **Use copy assignment** (`:=`) when you need independent copies of nullable objects
6. **Convert to fundamentals** when you need value semantics: `(nullablePrimitive as PrimitiveType) as fundamentalType`
