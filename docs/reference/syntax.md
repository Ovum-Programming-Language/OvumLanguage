# Syntax & Semantics (Description)

## Functions

* Declared with `fun`, PascalCase names: `fun Compute(a: Int): Int { ... }`
* **Pure** functions: `pure fun Hash(o: Object): Int { ... }`
  * Side-effect free; VM may cache results.
  * If parameters include user-defined reference types, those types must implement **`IComparable`**.

## Classes

* `class Name implements IFace1, IFace2 { ... }`

  * **No class inheritance**.
  * **Access modifiers are mandatory** on fields and methods.
  * **Fields** use `val` (immutable) or `var` (mutable):

    * `private val Prefix: String`
    * `public  var Count: Int`
  * **Methods** must declare access and can be `override`/`pure`:

    * `public override fun Run(): Int { ... }`
  * **Static** fields supported; **writing `static var` is unsafe**.
  * **Destructor**: optional, overrides the implicit virtual destructor from `Object`.

    * Syntax: `public destructor(): Void { ... }` (no parameters, no return).
    * Called automatically by GC finalization; **manual calls are unsafe**.

## Interfaces

* `interface IGreeter { fun Greet(name: String): String; }`

  * Methods are **public** and **virtual** by default.
  * No fields, no bodies.

## Type Aliases

* Create type aliases for better readability: `typealias UserId = Int`
* Can be used anywhere a type is expected: `fun ProcessUser(id: UserId): Void`

## Namespaces & Preprocessor

* Namespace resolution with `::` (e.g., `sys::Print`).
* Preprocessor: `#import`, `#define`, `#ifdef`, `#ifndef`, `#else`, `#undef`.

> **Note**: `#define` cannot be used to really define something, it is a way to control what code will be used.

## Functional Objects (`call`)

* Classes or interfaces can declare a **special `call`** member that makes instances **callable** like functions.
* Classes **define `call`**, it **may have other members**.
* Interfaces **contain undefined `call` member** (maybe not one, with all interface rules applying).
* A function literal `fun(...) : T { ... }` can be **coerced** to a interface type that exposes a compatible `call(...) : T` (and only this).

Example:

```ovum
interface CustomFunctional {
    call(a: Int?, b: Int?): Int
}

class DefinedFunctional {
    public var Multiplier: Int

    public fun DefinedFunctional(multiplier: Int): DefinedFunctional {
        this.Multiplier = multiplier
        return this
    }

    // Defines the callable behavior; pure body allowed
    public call(secondMultiplier: Int): Int = fun(secondMultiplier: Int): Int {
        return Multiplier * secondMultiplier
    }
}

val AddNullable: CustomFunctional = pure fun(a: Int?, b: Int?): Int {
    val aVal: int = a ?: 0  // Conversion from Int? to int
    val bVal: int = b ?: 0
    return aVal + bVal
}

fun Main(args: StringArray): Int {
    // Constructor call then functional call via `call`
    return AddNullable(2, DefinedFunctional(-1)(2))  // Implicit conversion from literals
}
```
