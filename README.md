# Ovum Programming Language

> This README (for `.ovum` sources) describes **Ovum**: syntax & semantics, nullable types, formal EBNF grammar, lexer & parser rules, runtime & VM requirements, unsafe operations, casting, functional objects, and code examples (snippets + a complete program).

---

## 1) Overview

**Ovum** is a strongly **statically typed**, **single-threaded** language with Kotlin-like syntax and a focus on safety, clarity, and performance.

Key design points:

* **Strong static typing** with **immutability by default** (`mut` required for mutation).
* **Nullable types** and Kotlin-style null-handling: `Type?`, safe calls `?.`, Elvis `?:`, non-null assertion `!!`.
* **Pure functions** (no side effects, VM-level result caching).
* **Classes & interfaces**

  * **No class inheritance**; classes only **implement interfaces**.
  * **Root `Object`** is implicit for all classes and interfaces (no need to write `: Object`). It declares only a **virtual destructor**.
  * Interfaces are named with **leading `I`** (C# style): e.g., `IGreeter`, `IComparable`.
  * Interface methods are **public** and **virtual** by default.
  * Class methods implementing interface members must be marked `override`.
  * **Access modifiers are mandatory** for all fields and methods in classes (`public`/`private`).
  * **Fields use `let` (immutable) or `mut` (mutable)**.
* **Namespaces** with `::` resolution (e.g., `sys::Print`).
* **Built-in operators**; **no user-defined operators**.
* **Preprocessor**: `#import`, `#define`, `#ifdef`, `#ifndef`, `#else`, `#undef`.
* **Managed runtime**: VM with **JIT** and **GC**; **no manual memory management**.
* **Single-threaded runtime**: no concurrency primitives.
* **Passing semantics**: all user-defined and non-primitive types (including `String` and all arrays) are **passed by reference** (const by default).
* **File extension**: `.ovum`.

### Standard Interfaces & Object Model

* **`Object`**: implicit root; has only a **virtual destructor**.
  Enables safe, uniform storage (e.g., in `ObjectArray`).
* Standard interfaces (all implicitly extend `Object`):

  * **`IStringConvertible`** — `ToString(): String`
  * **`IComparable`** — `IsLess(other: Object): Bool`

    * Required for user-defined types used as parameters to **pure** functions (stable ordering/keys).
  * **`IHashable`** — `GetHash(): Int`

> **Naming**: **Classes, functions, methods use PascalCase** (e.g., `Main`, `ToString`, `IsLess`). Keywords/modifiers remain lowercase (`class`, `interface`, `mut`, `override`, `pure`, `unsafe`, etc.).

---

## 2) Types

### Primitive Types

* `Int` (8 bytes)
* `Float` (8 bytes)
* `Bool`
* `Char`
* `Byte`
* `Pointer` *(only meaningful in `unsafe` code)*

> Any primitive may have a **nullable** counterpart via `?` (e.g., `Int?`).

### Non-Primitive / Reference Types

* `String` *(reference type; not primitive)*
* **Array classes (no templates / generics):**

  * For primitives: `IntArray`, `FloatArray`, `BoolArray`, `CharArray`, `ByteArray`, `PointerArray`
  * For objects: **`ObjectArray`**
  * **Convenience**: `StringArray` (array of `String`, used for `Main`)

> All non-primitive values are passed by link (reference), **const by default**.

---

## 3) Nullability & Boolean Logic

### Nullability (Kotlin-style)

* Append `?` to make a type **nullable**: `Int?`, `String?`, `Point?`.
* **Safe call**: `expr?.Method()` calls only if `expr != null`; otherwise yields `null` (if the method returns a reference type) or a sensible default for chaining to Elvis.
  Example: `name?.ToString()?.Length() ?: 0`
* **Elvis**: `a ?: b` evaluates to `a` if non-null, else `b`.
* **Non-null assertion**: `x!!` throws an unhandleable error if `x == null`.
* **Cast to Bool**: any value can be explicitly cast to `Bool`.

  * Primitives: zero → `false`, non-zero → `true`.
  * Non-primitives: `true` iff the reference is a valid (non-null, live) object.

### Boolean Logic

* **Short-circuit**: `&&`, `||` (like Kotlin/Java).
* **Negation**: `!cond`.
* **XOR**: `xor` infix on `Bool` (e.g., `a xor b`).
  (Also `==`/`!=` for equality/inequality of Booleans.)

---

## 4) Casting & Type Tests

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

---

## 5) Syntax & Semantics (Description)

### Functions

* Declared with `fun`, PascalCase names: `fun Compute(a: Int): Int { ... }`
* **Pure** functions: `pure fun Hash(o: Object): Int { ... }`

  * Side-effect free; VM may cache results.
  * If parameters include user-defined reference types, those types must implement **`IComparable`**.

### Classes

* `class Name implements IFace1, IFace2 { ... }`

  * **No class inheritance**.
  * **Access modifiers are mandatory** on fields and methods.
  * **Fields** use `let` (immutable) or `mut` (mutable):

    * `private let Prefix: String`
    * `public  mut Count: Int`
  * **Methods** must declare access and can be `override`/`pure`:

    * `public override fun Run(): Int { ... }`
  * **Static** fields supported; **writing `static mut` is unsafe**.
  * **Destructor**: optional, overrides the implicit virtual destructor from `Object`.

    * Syntax: `public destructor(): Void { ... }` (no parameters, no return).
    * Called automatically by GC finalization; **manual calls are unsafe**.

### Interfaces

* `interface IGreeter { fun Greet(name: String): String; }`

  * Methods are **public** and **virtual** by default.
  * No fields, no bodies.

### Namespaces & Preprocessor

* Namespace resolution with `::` (e.g., `sys::Print`).
* Preprocessor: `#import`, `#define`, `#ifdef`, `#ifndef`, `#else`, `#undef`.

### Functional Objects (`call`)

* Classes can declare a **special `call`** member that makes instances **callable** like functions.
* If a class **declares `call`**, it **may have other members**.
* If a class **does not declare `call`**, it **must not have any non-`call` members** (i.e., it must be purely functional).
* A function literal `fun(...) : T { ... }` can be **coerced** to a class type that exposes a compatible `call(...) : T`.

Example:

```ovum
class CustomFunctional {
    public call(a: Int?, b: Int?): Int
}

class DefinedFunctional {
    public mut Multiplier: Int

    public fun DefinedFunctional(multiplier: Int): DefinedFunctional {
        this.Multiplier = multiplier
        return this
    }

    // Defines the callable behavior; pure body allowed
    public call(secondMultiplier: Int): Int = pure fun(secondMultiplier: Int): Int {
        return Multiplier * secondMultiplier
    }
}

let AddNullable: CustomFunctional = fun(a: Int?, b: Int?): Int {
    return (a ?: 0) + (b ?: 0)
}

fun Main(args: StringArray): Int {
    // Constructor call then functional call via `call`
    return AddNullable(2, DefinedFunctional(-1)(2))
}
```

---

## 6) Formal Grammar (EBNF)

> Core EBNF; whitespace/comments omitted. Operator precedence in §7.

```ebnf
Program         ::= { Import | Conditional | GlobalDef } ;

Import          ::= "#import" StringLiteral ;
Define          ::= "#define" Identifier [ NumberLiteral ] ;
Undef           ::= "#undef" Identifier ;
Conditional     ::= "#ifdef" Identifier { GlobalDef | Import | Conditional }
                    [ "#else" { GlobalDef | Import | Conditional } ] "#endif"
                 | "#ifndef" Identifier { GlobalDef | Import | Conditional }
                    [ "#else" { GlobalDef | Import | Conditional } ] "#endif" ;

GlobalDef       ::= FunctionDecl | ClassDecl | InterfaceDecl | GlobalVarDecl ;

FunctionDecl    ::= [ "pure" ] "fun" Identifier "(" [ ParamList ] ")" [ ":" Type ] Block ;
ParamList       ::= Parameter { "," Parameter } ;
Parameter       ::= [ "mut" ] Identifier ":" Type ;

ClassDecl       ::= "class" Identifier [ "implements" TypeList ] ClassBody ;
TypeList        ::= Type { "," Type } ;
ClassBody       ::= "{" { ClassMember } "}" ;
ClassMember     ::= FieldDecl | StaticFieldDecl | MethodDecl | DestructorDecl ;

FieldDecl       ::= ( "private" | "public" ) ( "let" | "mut" ) Identifier ":" Type [ "=" Expression ] ";" ;
StaticFieldDecl ::= "static" ( "private" | "public" ) ( "let" | "mut" ) Identifier ":" Type [ "=" Expression ] ";" ;

MethodDecl      ::= ( "private" | "public" ) [ "override" ] [ "pure" ]
                    "fun" Identifier "(" [ ParamList ] ")" [ ":" Type ] ( Block | ";" ) ;

DestructorDecl  ::= ( "private" | "public" ) "destructor" "(" ")" ":" "Void" Block ;

InterfaceDecl   ::= "interface" Identifier InterfaceBody ;  // implicitly extends Object
InterfaceBody   ::= "{" { InterfaceMethod } "}" ;
InterfaceMethod ::= "fun" Identifier "(" [ ParamList ] ")" [ ":" Type ] ";" ;  // public & virtual

GlobalVarDecl   ::= [ "mut" ] Identifier ":" Type "=" Expression ";" ;

Type            ::= NullableType | NonNullType ;
NullableType    ::= NonNullType "?" ;
NonNullType     ::= PrimitiveType
                 | "String"
                 | "IntArray" | "FloatArray" | "BoolArray" | "CharArray" | "ByteArray" | "PointerArray"
                 | "ObjectArray"
                 | "StringArray"
                 | Identifier ;              // class/interface names (non-primitive)

PrimitiveType   ::= "Int" | "Float" | "Bool" | "Char" | "Byte" | "Pointer" ;

Block           ::= "{" { Statement } "}" ;
Statement       ::= VarDeclStmt | ExprStmt | ReturnStmt | IfStmt | WhileStmt | ForStmt | UnsafeStmt | Block ;

VarDeclStmt     ::= [ "mut" ] Identifier ":" Type "=" Expression ";" ;
ExprStmt        ::= Expression ";" ;
ReturnStmt      ::= "return" [ Expression ] ";" ;
IfStmt          ::= "if" "(" Expression ")" Statement [ "else" Statement ] ;
WhileStmt       ::= "while" "(" Expression ")" Statement ;
ForStmt         ::= "for" "(" Identifier "in" Expression ")" Statement ;
UnsafeStmt      ::= "unsafe" Block ;

Expression      ::= Assignment ;
Assignment      ::= ElvisExpr [ "=" Assignment ] ;

ElvisExpr       ::= OrExpr [ "?:" ElvisExpr ] ;  // right-assoc

OrExpr          ::= AndExpr { "||" AndExpr } ;
AndExpr         ::= XorExpr { "&&" XorExpr } ;
XorExpr         ::= EqualityExpr { "xor" EqualityExpr } ;

EqualityExpr    ::= RelExpr { ("==" | "!=") RelExpr } ;
RelExpr         ::= AddExpr { ("<" | "<=" | ">" | ">=") AddExpr } ;
AddExpr         ::= MulExpr { ("+" | "-") MulExpr } ;
MulExpr         ::= UnaryExpr { ("*" | "/" | "%") UnaryExpr } ;

UnaryExpr       ::= ("!" | "-" | "&" | "*") UnaryExpr
                 | Postfix ;

Postfix         ::= Primary { PostfixOp } ;
PostfixOp       ::= "." Identifier
                 | "." Identifier "(" [ ArgList ] ")"
                 | "(" [ ArgList ] ")"          // function call or callable object
                 | "as" Type                    // explicit cast; downcast yields nullable type
                 | "is" Type                    // type test → Bool
                 | "!!"                         // non-null assertion
                 | "?." Identifier [ "(" [ ArgList ] ")" ]  // safe call chain
                 ;

Primary         ::= Identifier
                 | Literal
                 | "(" Expression ")"
                 | NamespaceRef
                 | FunctionLiteral ;

FunctionLiteral ::= "fun" "(" [ ParamList ] ")" [ ":" Type ] Block ;

NamespaceRef    ::= Identifier "::" Identifier ;
ArgList         ::= Expression { "," Expression } ;

Literal         ::= NumberLiteral | StringLiteral | CharLiteral | "true" | "false" ;
```

---

## 7) Lexer & Parser Rules

### Tokens

* **Identifiers**: `[A-Za-z_][A-Za-z0-9_]*` (case-sensitive). Style: **PascalCase** for classes/functions/methods.
* **Literals**: integers, floats, chars (`'A'`, escapes), strings (`"..."`, escapes), Booleans.
* **Operators/Punct**:

  * Arithmetic: `+ - * / %`
  * Comparison: `== != < <= > >=`
  * Boolean: `&& || ! xor`
  * Nulls: `?. ?: !!`
  * Cast/Test: `as is`
  * Call/Access: `. ( ) :: , ; : { } [ ]`
  * Assignment: `=`

### Comments & Whitespace

* Line: `// ...`
* Block: `/* ... */` (non-nested)
* Whitespace separates tokens; not otherwise significant.

### Precedence (high → low)

1. Postfix: member calls `.`, calls `()`, safe call `?.`, non-null `!!`, casts `as`, test `is`
2. Unary: `!` `-` `&` `*` (right; `&`/`*` only in `unsafe`)
3. Multiplicative: `*` `/` `%` (left)
4. Additive: `+` `-` (left)
5. Relational: `<` `<=` `>` `>=` (left)
6. Equality: `==` `!=` (left)
7. XOR: `xor` (left)
8. Logical AND: `&&` (left)
9. Logical OR: `||` (left)
10. Elvis: `?:` (right)
11. Assignment: `=` (right)

> **No user-defined operators**.

---

## 8) Runtime, VM & Platform Support

* **Execution**: Source (`.ovum`) → bytecode → **Ovum VM**.
* **JIT**: Hot paths compiled to native for performance.
* **GC**: Automatic memory reclamation; **no manual memory management**.
* **Single-threaded**: Execution model and VM are single-threaded.
* **Architectures**: **amd64** and **arm64** supported.
* **Numeric widths**: `Int` **8 bytes**, `Float` **8 bytes**.

---

## 9) System Library & Interop

* `sys::Print(msg: String): Void`
* `sys::Time(): Int`
* `sys::Sleep(ms: Int): Void`
* `sys::Exit(code: Int): Never` *(terminates the process)*
* **Interop (FFI)**:

  * `sys::Interope(dllName: String, functionName: String, input: ByteArray, output: ByteArray): Int`
  * **All interop calls are `unsafe`.**

> Names use **PascalCase** (e.g., `Print`, `Time`, `Sleep`, `Exit`, `Interope`). Namespace remains `sys`.

---

## 10) Unsafe Operations (Recap)

Allowed **only** inside `unsafe { ... }`:

* Declaring/writing **global `mut`** variables and **`static mut`** fields.
* Casting const → mutable.
* Using **`Pointer`**, address-of and dereference.
* **Manual destructor** calls.
* Any **`sys::Interope`** invocation.
* Casting any value to **(const or mutable) `ByteArray`**.

---

## 11) Code Examples

### 11.1 Entry point (`StringArray`)

```ovum
// .ovum file
fun Main(args: StringArray): Int {
    val count: Int = args.Length() ?: 0
    sys::Print("Args count: " + count.ToString())
    return 0
}
```

### 11.2 Variables, Nulls, Elvis, Safe Calls

```ovum
fun DemoNulls(): Void {
    val a: Int? = null
    val b: Int? = 5

    val sum: Int = (a ?: 0) + (b ?: 0)  // Elvis
    sys::Print("Sum = " + sum.ToString())

    val name: String? = null
    sys::Print("Name length = " + (name?.Length() ?: 0).ToString())

    val mustNotBeNull: Int = (b!!)      // ok
    // val crash: Int = (a!!)           // aborts (unhandleable)
}
```

### 11.3 Interfaces, Classes, Fields, Overrides

```ovum
interface IGreeter {
    fun Greet(name: String): String  // public + virtual by default
}

class FriendlyGreeter implements IGreeter {
    private let Prefix: String = "Hello"
    public  mut Suffix: String = "!"

    public fun FriendlyGreeter(prefix: String, suffix: String): FriendlyGreeter {
        this.Prefix = prefix
        this.Suffix = suffix
        return this
    }

    public override fun Greet(name: String): String {
        return Prefix + ", " + name + Suffix
    }

    // Optional destructor (finalization logic)
    public destructor(): Void {
        // release non-memory resources if any (files, handles, etc.)
    }
}
```

### 11.4 Standard Interfaces (`IStringConvertible`, `IComparable`, `IHashable`)

```ovum
interface IStringConvertible { fun ToString(): String }
interface IComparable        { fun IsLess(other: Object): Bool }
interface IHashable          { fun GetHash(): Int }

class Point implements IStringConvertible, IComparable, IHashable {
    public let X: Int
    public let Y: Int

    public fun Point(x: Int, y: Int): Point { this.X = x; this.Y = y; return this; }

    public override fun ToString(): String {
        return "(" + X.ToString() + ", " + Y.ToString() + ")"
    }

    public override fun IsLess(other: Object): Bool {
        if (!(other is Point)) return false
        val p: Point = (other as Point)!!   // safe after is + !!
        if (this.X != p.X) return this.X < p.X
        return this.Y < p.Y
    }

    public override fun GetHash(): Int {
        return (X * 1315423911) ^ (Y * 2654435761)
    }
}
```

### 11.5 Pure Functions with Caching

```ovum
pure fun Fib(n: Int): Int {
    if (n <= 1) return n
    return Fib(n - 1) + Fib(n - 2)
}
// For user-defined reference types as parameters, implement IComparable.
```

### 11.6 `is`, `as`, `!!`, and ByteArray Casts

```ovum
fun DemoCasts(obj: Object): Void {
    if (obj is Point) {
        val p: Point = (obj as Point)!!   // nullable cast + assert
        sys::Print(p.ToString())
    }

    // Bool cast
    val b1: Bool = (0 as Bool)      // false
    val b2: Bool = (42 as Bool)     // true
    val b3: Bool = (obj as Bool)    // true if non-null

    // Unsafe: raw byte views
    unsafe {
        val bytesConst: ByteArray = (obj as ByteArray)
        val bytesMut  : ByteArray = (obj as mut ByteArray)
    }
}
```

### 11.7 Functional Objects (`call`) & Literals

```ovum
class CustomFunctional {
    public call(a: Int?, b: Int?): Int
}

class DefinedFunctional {
    public mut Multiplier: Int

    public fun DefinedFunctional(multiplier: Int): DefinedFunctional {
        this.Multiplier = multiplier
        return this
    }

    public call(secondMultiplier: Int): Int = pure fun(secondMultiplier: Int): Int {
        return Multiplier * secondMultiplier
    }
}

let AddNullable: CustomFunctional = fun(a: Int?, b: Int?): Int {
    return (a ?: 0) + (b ?: 0)
}

fun Main(args: StringArray): Int {
    return AddNullable(2, DefinedFunctional(-1)(2))
}
```

---

## 12) Build & Run (Conceptual)

1. **Compile** `.ovum` sources → Ovum bytecode (preprocessor applied).
2. **Execute** on the Ovum VM (single-threaded):

   * Bytecode interpretation with JIT for hot paths.
   * GC manages memory automatically.
   * Entry function: `Main(args: StringArray): Int`.

---

## 13) Notes & Best Practices

* Prefer **immutable** data; use `mut` only when necessary.
* Implement **`IStringConvertible`** for diagnostics (`ToString`).
* For **pure** functions with custom types, implement **`IComparable`**.
* Avoid global `mut`; if necessary, **isolate in `unsafe`** with rationale.
* Keep names **PascalCase** for classes/functions/methods; keep **keywords lowercase**.
* `String` and all array classes are **reference (non-primitive)** types.
