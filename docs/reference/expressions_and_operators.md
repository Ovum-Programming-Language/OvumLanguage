# Expressions and Operators

Expressions in Ovum include literal values, variable references, function calls, method calls, field accesses, and combinations of these with operators. Operator syntax and precedence are designed to be familiar to C/Java/Kotlin developers.

## Arithmetic Operators

* `+` (addition) - operates on numeric types and may be overloaded internally for string concatenation
* `-` (subtraction) - operates on numeric types
* `*` (multiplication) - operates on numeric types
* `/` (division) - operates on numeric types
* `%` (modulo) - operates on numeric types

## Comparison Operators

* `==` (equality) - most types can be compared for equality
* `!=` (inequality) - opposite of equality
* `<`, `<=`, `>`, `>=` (ordering) - only valid on types that have a defined ordering (numeric types or classes implementing `IComparable`)

## Logical Operators

* `&&` (logical AND) - short-circuit evaluation
* `||` (logical OR) - short-circuit evaluation
* `!` (negation) - unary operator
* `xor` (exclusive OR) - infix operator on `Bool`

## Assignment Operator

* `=` (assignment) - assigns a value to a mutable variable or field. The left-hand side must be a mutable variable or field.

## Member Access

* `object.field` - access a field of an object
* `object.method()` - call a method on an object
* `object?.field` - safe field access (returns null if object is null)
* `object?.method()` - safe method call (returns null if object is null)

## Type Operations

* `expr as Type` - explicit cast (downcast yields nullable type)
* `expr is Type` - type test (returns `Bool`)

## Null Handling

* `expr?.member` - safe call (calls only if expr is not null)
* `expr ?: default` - Elvis operator (returns expr if not null, otherwise default)
* `expr!!` - non-null assertion (throws error if expr is null)

## Namespace Resolution

* `Namespace::Name` - refers to a definition from a specific namespace (e.g., `sys::Print`)

## No User-Defined Operators

Programmers cannot create new operator symbols or overload the existing ones for user-defined types. The set of operators and their meanings are fixed by the language. This keeps the language syntax clear and consistent and avoids operator overloading misuse.
