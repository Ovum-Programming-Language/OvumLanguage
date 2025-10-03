# Ovum Programming Language

Ovum is a strongly statically typed, single-threaded language focused on safety, clarity, and performance. It uses Kotlin-like syntax, immutability by default, a GC + JIT VM, interface-based polymorphism, and support for pure functions and functional objects.

- Contribute: see [`CONTRIBUTING.md`](CONTRIBUTING.md).
- Project docs: [`docs/README.md`](docs/README.md).
- Full language reference: [`docs/reference/README.md`](docs/reference/README.md).

---

## [Overview](docs/reference/design.md)

- Memory safety via GC; no manual memory management.
- Immutable by default; explicit mutation with `var`.
- Pure functions (`pure`) are side-effect free and may be cached on VM level.
- Interface-based polymorphism; no class inheritance.
- Explicit `unsafe { ... }` for low-level operations.
- Minimal, predictable syntax; no user-defined operators.
- No generics/templates, only ad-hoc and subclass polymorphism.

---

## [Syntax at a Glance](docs/reference/syntax.md)

- Files: `.ovum`. Names: PascalCase for types/methods.
- Functions: `fun Name(a: T): U { ... }`; pure: `pure fun ...`.
- Classes: `class Name implements IFace { ... }`; interfaces: `interface IFace { ... }`.
- Fields: `val` (immutable) or `var` (mutable); access modifiers are mandatory.
- Namespaces: `::` (e.g., `sys::Print`). Basic preprocessor: `#import`, `#define`, `#ifdef`…

---

## [Types](docs/reference/types.md) and [Nullability](docs/reference/nullable.md)

- **Fundamental types**: `int`, `float`, `byte`, `char`, `bool`, `pointer` (value types, not nullable, not Objects)
- **Primitive reference types**: `Int`, `Float`, `Byte`, `Char`, `Bool`, `Pointer` (reference wrappers, nullable, Objects)
- **Implicit conversion**: Literals convert to primitives (`val count: Int = 0`)
- **Nullable types**: Append `?` to reference types only (e.g., `Int?`, `String?`)
- **Safe call `?.`**, **Elvis `?:`** for null handling
- **Type test `is`**, **cast `as`** (downcast yields nullable type)
- **Copy assignment `:=`** for deep copying reference types

---

## [Control Flow](docs/reference/control_flow.md)

- `if/else`, `while`, `for (x in xs)`.
- `return`, `break`, `continue`; no `goto`.

---

## [Expressions and Operators](docs/reference/expressions_and_operators.md)

- Arithmetic: `+ - * / %`
- Comparison: `== != < <= > >=`
- Boolean: `&& || ! xor` (short‑circuit `&&`/`||`).
- Assignment: `=` (reference assignment), `:=` (copy assignment)
- Member/calls: `. ()` and safe `?.`
- Null handling: `?. ?:`
- Type ops: `as`, `is`
- Namespace: `::`

---

## [Object Model](docs/reference/object_model.md)

- Classes implement interfaces; no class inheritance.
- Methods declare access; support `override` and `pure`.
- Optional destructor; called by GC (manual calls are unsafe).

### Functional Objects (`call`)

- Classes or interfaces can define a special `call` member to be callable.
- Function literals can coerce to an interface exposing a compatible `call`.

---

## [Runtime and Tooling](docs/reference/runtime.md)

- Pipeline: `.ovum` → bytecode → Ovum VM.
- GC for memory safety; JIT compiles hot paths.
- Single‑threaded execution model.
- Architectures: amd64, arm64. Numeric widths: `int` 8 bytes, `float` 8 bytes.
- Entry point: `Main(args: StringArray): Int`.

Build & Run (conceptual): write `.ovum`, compile (parse, type‑check, enforce const/pure), run on VM (JIT + GC).

---

## [System Library and Interop](docs/reference/system_library.md)

- `sys::Print(msg: String): Void`
- `sys::Time(): Int`
- `sys::Sleep(ms: Int): Void`
- `sys::Exit(code: Int): Never`
- FFI: `sys::Interope(dllName, functionName, input: ByteArray, output: ByteArray): Int` (unsafe)

---

## [Unsafe (recap)](docs/reference/unsafe.md)

Only inside `unsafe { ... }`:
- Global `var` and `static var` writes.
- Const/mutable casts; `pointer`, address‑of, dereference.
- Manual destructor calls.
- `sys::Interope`; casting any value to (const or mutable) `ByteArray`.

---

## [Code Examples](docs/reference/code_examples.md)

### Entry point (`StringArray`)

```ovum
// .ovum file
fun Main(args: StringArray): Int {
    val count: Int = args.Length()  // Built-in returns Int
    sys::Print("Args count: " + count.ToString())
    return 0  // Implicit conversion from literal
}
```

### Pure functions with caching

```ovum
pure fun Fib(n: int): int {
    if (n <= 1) return n
    val fib1: int = Fib(n - 1)
    val fib2: int = Fib(n - 2)
    return fib1 + fib2
}
```

### `is`, `as` and ByteArray casts

```ovum
fun DemoCasts(obj: Object): Void {
    if (obj is Point) {
        val p: Point? = obj as Point
        if (p != null) {
            val nonNullP: Point = p ?: Point(0, 0)  // Use Elvis operator
            sys::Print(nonNullP.ToString())
        }
    }

    // Bool cast
    val b1: Bool = 0 as bool  // false
    val b2: Bool = 42 as bool  // true
    val b3: Bool = obj as bool  // always true
    val b4: Bool = (obj as Point) as bool  // true if obj is a Point

    // Unsafe: raw byte views
    unsafe {
        val bytesConst: ByteArray = (obj as ByteArray)
        val bytesMut  : ByteArray = (obj as var ByteArray)
    }
}
```

### Functional objects (`call`) & literals

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

    public call(secondMultiplier: Int): Int = fun(secondMultiplier: Int): Int {
        return Multiplier * secondMultiplier
    }
}

val AddNullable: CustomFunctional = pure fun(a: Int?, b: Int?): Int {
    val aVal: int = a ?: 0  // Implicit conversion from Int? to int
    val bVal: int = b ?: 0
    return Int(aVal + bVal)  // Implicit conversion from int to Int
}

fun Main(args: StringArray): Int {
    // Constructor call then functional call via `call`
    return AddNullable(2, DefinedFunctional(-1)(2))  // Implicit conversion from literals
}
```

### Complete program example

```ovum
// Complete Ovum program demonstrating key features
interface ICalculator {
    fun Calculate(a: Int, b: Int): Int
}

class Adder implements ICalculator {
    public override fun Calculate(a: Int, b: Int): Int {
        return a + b
    }
}

class Multiplier implements ICalculator {
    public override fun Calculate(a: Int, b: Int): Int {
        return a * b
    }
}

pure fun ProcessNumbers(calc: ICalculator, numbers: IntArray): int {
    var result: int = 0
    for (num in numbers) {
        val calcResult: Int = calc.Calculate(num, 2)  // Implicit conversion from literal
        result = result + calcResult  // Implicit conversion
    }
    return result
}

fun Main(args: StringArray): int {
    val numbers: IntArray = IntArray(3)
    numbers[0] := 5  // Implicit conversion from literal
    numbers[1] := 10
    numbers[2] := 15
    
    val adder: ICalculator = Adder()
    val multiplier: ICalculator = Multiplier()
    
    val sumResult: int = ProcessNumbers(adder, numbers)
    val productResult: int = ProcessNumbers(multiplier, numbers)
    
    sys::Print("Sum result: " + Int(sumResult).ToString())
    sys::Print("Product result: " + Int(productResult).ToString())
    
    return 0  // Implicit conversion from literal
}
```
