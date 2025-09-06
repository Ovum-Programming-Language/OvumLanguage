# Ovum Programming Language

> This README describes **Ovum**: syntax & semantics, formal EBNF grammar, lexer & parser rules, runtime & VM requirements, unsafe operations, and code examples (snippets + a complete program).

---

## 1) Overview

**Ovum** is a strongly **statically typed**, **single-threaded** language with modern, Kotlin-like syntax and a focus on safety, clarity, and performance.

Key design points:

* **Strong static typing** with **immutability by default** (`mut` required for mutation).
* **Pure functions** (no side effects, VM-level result caching).
* **Classes & interfaces**

  * **No class inheritance**; classes only **implement interfaces**.
  * **Every class implements `Object`** (root interface) implicitly; **all interfaces extend `Object`**. `Object` has **only a virtual destructor**.
  * Interface methods are **virtual by default**; class methods that implement them must be marked `override`.
* **Namespaces** with `::` resolution (e.g., `sys::Print`).
* **Built-in operators**; **no user-defined operators**.
* **Preprocessor**: `#import`, `#define`, `#ifdef`, `#ifndef`, `#else`, `#undef`.
* **Managed runtime**: VM with **JIT** and **GC**; **no manual memory management**.
* **Single-threaded runtime**: no concurrency primitives.
* **Passing semantics**: all user-defined and non-primitive types (including `String` and all arrays) are **passed by reference** (const by default).

### Standard Interfaces & Object Model

* **`Object`**: root interface with **only a virtual destructor**.

  * All classes implement `Object` by default.
  * All interfaces extend `Object`.
  * Enables safe, uniform storage of user-defined types (e.g., in `ObjectArray`).
* Additional standard interfaces:

  * **`StringConvertible`** — `ToString(): String`
  * **`Comparable`** — `IsLess(other: Object): Bool`

    * Required when user-defined types appear as parameters to **pure** functions (for stable ordering/keys).
    * Implementations must validate and safely downcast `other` before using it.
  * **`Hashable`** — `GetHash(): Int`

> **Naming**: **Classes, functions, and methods use PascalCase** (e.g., `Main`, `ToString`, `IsLess`). Keywords/modifiers are lowercase (`class`, `interface`, `mut`, `override`, `pure`, etc.).

---

## 2) Types

### Primitive Types

* `Int` (8 bytes)
* `Float` (8 bytes)
* `Bool`
* `Char`
* `Byte`
* `Pointer` *(only meaningful in `unsafe` code)*

### Non-Primitive / Reference Types

* `String` *(reference type; not primitive)*
* **Array classes (no templates / generics)**:

  * For primitives: `IntArray`, `FloatArray`, `BoolArray`, `CharArray`, `ByteArray`, `PointerArray`
  * For objects: **`ObjectArray`** (stores elements of type `Object`)
  * **Convenience**: `StringArray` (standard library class for arrays of `String`, commonly used for `Main` arguments)
* Containers `List`, `Map` may exist as reference types (non-primitive) in the stdlib (not templated in this version).

> All non-primitive values are passed by link (reference), **const by default**. To mutate through a parameter, mark the parameter `mut` and ensure the underlying fields are `mut`.

---

## 3) Syntax & Semantics (Description)

### Declarations

* **Functions**: `pure fun Name(params): ReturnType { ... }`

  * Pure functions: side-effect free; may be cached by the VM.
  * If parameters include user-defined reference types, those types must implement **`Comparable`** (`IsLess`).
* **Variables**: immutable by default. Use `mut` to permit reassignment/mutation.
* **Classes**: `class Name implements IFace1, IFace2 { ... }`

  * No class inheritance.
  * **Fields and methods must specify an access modifier** (`private` or `public`).
  * **Fields use `let` or `mut`**:

    * `public let X: Int` — immutable field
    * `private mut Count: Int` — mutable field
  * Implementing interface methods must be marked `override`.
  * `static` fields allowed; **`static mut` is unsafe** to write.
* **Interfaces**: `interface Name : Object { ... }`

  * **Methods are public**; access modifiers are not used in interfaces.
  * Signatures only (no fields, no bodies); methods are implicitly virtual.

### Namespaces & Preprocessor

* Namespace resolution with `::` (e.g., `sys::Print`).
* Preprocessor directives:

  * `#import "path.ovm"`
  * `#define NAME [value]`
  * `#ifdef NAME` / `#ifndef NAME` / `#else` / `#endif`
  * `#undef NAME`

### Casting & Type Tests

* **Upcasts** (e.g., `Derived` → `BaseInterface`/`Object`): **safe**.
* **Downcasts** (e.g., `Object`/interface → concrete class): **runtime-checked**.

  * Use explicit cast: `expr as TargetClass`.
  * **If the downcast fails, an unhandleable error is raised (program abort).**
  * **Before a downcast you must check**: `if (expr is TargetClass) { let v = expr as TargetClass; ... }`
* **ByteArray casts** (view of raw bytes):

  * **Unsafe cast of any value to (const) `ByteArray`**.
  * **Unsafe cast of any value to mutable `ByteArray`** (mutable view).
  * Both require `unsafe { ... }`, e.g., `unsafe { let bytes: ByteArray = value as ByteArray; }`
* **Explicit cast to `Bool`** is **safe**:

  * For primitives: `0`/`0.0`/`'\0'` → `false`; non-zero → `true`.
  * For non-primitives: `true` iff the reference is a **valid** (non-null, live) object.
* **`is` type test**: `expr is Type` → `Bool` (must be used prior to downcasting).

### Purity Rules (Summary)

* **Pure** functions cannot:

  * Perform I/O, call impure/system/`unsafe` code.
  * Mutate global or external state, return handles to newly allocated mutable state.
* For pure function parameters of user-defined class types, require **`Comparable`** (`IsLess(other: Object): Bool`) to enable stable ordering/keys.

### Unsafe Blocks

Operations only allowed in `unsafe { ... }`:

* Declaring/writing **global `mut`** variables and **`static mut`** fields.
* Casting const → mutable.
* Using **`Pointer`**, address-of and dereference.
* **Manual destructor** calls.
* Any **`sys::Interope`** (FFI) call.
* Casting any type to **(const or mutable) `ByteArray`**.

---

## 4) Formal Grammar (EBNF)

> Core EBNF; whitespace/comments omitted. Operator precedence is in §5.

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
ClassMember     ::= FieldDecl | StaticFieldDecl | MethodDecl ;

FieldDecl       ::= ( "private" | "public" ) ( "let" | "mut" ) Identifier ":" Type [ "=" Expression ] ";" ;
StaticFieldDecl ::= "static" ( "private" | "public" ) ( "let" | "mut" ) Identifier ":" Type [ "=" Expression ] ";" ;

MethodDecl      ::= ( "private" | "public" ) [ "override" ] [ "pure" ]
                    "fun" Identifier "(" [ ParamList ] ")" [ ":" Type ] ( Block | ";" ) ;

InterfaceDecl   ::= "interface" Identifier ":" "Object" InterfaceBody ;
InterfaceBody   ::= "{" { InterfaceMethod } "}" ;
InterfaceMethod ::= "fun" Identifier "(" [ ParamList ] ")" [ ":" Type ] ";" ;  // public & virtual by default

GlobalVarDecl   ::= [ "mut" ] Identifier ":" Type "=" Expression ";" ;

Type            ::= PrimitiveType
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
Assignment      ::= OrExpr [ "=" Assignment ] ;
OrExpr          ::= AndExpr { "||" AndExpr } ;
AndExpr         ::= EqualityExpr { "&&" EqualityExpr } ;
EqualityExpr    ::= RelExpr { ("==" | "!=") RelExpr } ;
RelExpr         ::= AddExpr { ("<" | "<=" | ">" | ">=") AddExpr } ;
AddExpr         ::= MulExpr { ("+" | "-") MulExpr } ;
MulExpr         ::= UnaryExpr { ("*" | "/" | "%") UnaryExpr } ;

UnaryExpr       ::= ("!" | "-" | "&" | "*") UnaryExpr
                 | Postfix ;

Postfix         ::= Primary { PostfixOp } ;
PostfixOp       ::= "." Identifier
                 | "." Identifier "(" [ ArgList ] ")"
                 | "(" [ ArgList ] ")"
                 | "as" Type                 // explicit cast
                 | "is" Type                 // type test → Bool
                 ;

Primary         ::= Identifier
                 | Literal
                 | "(" Expression ")"
                 | NamespaceRef ;

NamespaceRef    ::= Identifier "::" Identifier ;
ArgList         ::= Expression { "," Expression } ;

Literal         ::= NumberLiteral | StringLiteral | CharLiteral | "true" | "false" ;
```

> Notes
>
> * **No templates**; array types are concrete classes (`IntArray`, `ObjectArray`, etc.).
> * `String`/all arrays are **reference** (non-primitive) types.
> * `is` and `as` are part of expressions (type test & cast).

---

## 5) Lexer & Parser Rules

### Tokens

* **Identifiers**: `[A-Za-z_][A-Za-z0-9_]*` (case-sensitive). Style: **PascalCase** for classes/functions/methods.
* **Literals**: integers, floats, chars (`'A'`, escapes), strings (`"..."`, escapes).
* **Operators/Punct**: `+ - * / %`, `== != < <= > >=`, `&& || !`, `=`, `.`, `::`, `,`, `;`, `:`, `(` `)` `{` `}`.

### Comments & Whitespace

* Line: `// ...`
* Block: `/* ... */` (non-nested)
* Whitespace separates tokens; not otherwise significant.

### Precedence & Associativity (high → low)

1. Unary: `!` `-` `&` `*` (right; `&`/`*` only in `unsafe`)
2. Multiplicative: `*` `/` `%` (left)
3. Additive: `+` `-` (left)
4. Relational: `<` `<=` `>` `>=` (left)
5. Equality: `==` `!=` (left)
6. Logical AND: `&&` (left)
7. Logical OR: `||` (left)
8. Assignment: `=` (right)

> **No user-defined operators**.

---

## 6) Runtime, VM & Platform Support

* **Execution**: Source → bytecode → **Ovum VM**.
* **JIT**: Hot paths compiled to native for performance.
* **GC**: Automatic memory reclamation; **no manual memory management**.
* **Single-threaded**: Execution model and VM are single-threaded.
* **Architectures**: **amd64** and **arm64** supported.
* **Numeric widths**: `Int` **8 bytes**, `Float` **8 bytes**.

---

## 7) System Library & Interop

* `sys::Print(msg: String): Void`
* `sys::Time(): Int`
* `sys::Sleep(ms: Int): Void`
* `sys::Exit(code: Int): Never` *(terminates the process)*
* **Interop (FFI)**:

  * `sys::Interope(dllName: String, functionName: String, input: ByteArray, output: ByteArray): Int`
  * **All interop calls are `unsafe`.**

> Names use **PascalCase** (e.g., `Print`, `Time`, `Sleep`, `Exit`, `Interope`). Namespace remains `sys`.

---

## 8) Unsafe Operations (Recap)

Allowed **only** inside `unsafe { ... }`:

* Declaring/writing **global `mut`** variables and **`static mut`** fields.
* Casting const → mutable.
* Using **`Pointer`**, address-of and dereference.
* **Manual destructor** calls.
* Any **`sys::Interope`** invocation.
* Casting any value to **(const or mutable) `ByteArray`**.

---

## 9) Code Examples

### 9.1 Entry Point (`StringArray`)

```ovum
// Standard library provides StringArray (array of String).
fun Main(args: StringArray): Int {
    if (args.Length() == 0) {
        sys::Print("Hello, Ovum!");
        return 0;
    }

    sys::Print("Args count: " + args.Length().ToString());
    // Example access (APIs are illustrative):
    // let first: String = args.Get(0);
    return 0;
}
```

### 9.2 Variables & Immutability

```ovum
fun DemoVariables(): Void {
    value: Int = 10;         // immutable local
    mut counter: Int = 0;    // mutable local

    counter = counter + value;
    sys::Print("Counter = " + counter.ToString());
}
```

### 9.3 Interfaces, Classes, Fields, and Overrides

```ovum
interface Greeter : Object {
    fun Greet(name: String): String;  // public by default
}

class FriendlyGreeter implements Greeter {
    private let Prefix: String = "Hello";
    public  mut Suffix: String = "!";

    // Constructor pattern (returns this for chaining)
    public fun FriendlyGreeter(prefix: String, suffix: String): FriendlyGreeter {
        this.Prefix = prefix;
        this.Suffix = suffix;
        return this;
    }

    public override fun Greet(name: String): String {
        return Prefix + ", " + name + Suffix;
    }

    public fun CasualGreet(name: String): String {
        return "Hi, " + name;
    }
}

fun DemoGreeter(): Void {
    let g: Greeter = FriendlyGreeter("Good morning", "!");
    sys::Print(g.Greet("Ovum"));
}
```

### 9.4 Standard Interfaces (`StringConvertible`, `Comparable`, `Hashable`)

```ovum
interface StringConvertible : Object {
    fun ToString(): String;
}

interface Comparable : Object {
    fun IsLess(other: Object): Bool;
}

interface Hashable : Object {
    fun GetHash(): Int;
}

class Point implements StringConvertible, Comparable, Hashable {
    public let X: Int
    public let Y: Int

    public fun Point(x: Int, y: Int): Point { this.X = x; this.Y = y; return this; }

    public override fun ToString(): String {
        return "(" + X.ToString() + ", " + Y.ToString() + ")";
    }

    public override fun IsLess(other: Object): Bool {
        if (!(other is Point)) { return false; }               // required check before downcast
        let p: Point = other as Point;                         // safe after 'is'
        if (this.X != p.X) return this.X < p.X;
        return this.Y < p.Y;
    }

    public override fun GetHash(): Int {
        return (X * 1315423911) ^ (Y * 2654435761);
    }
}
```

### 9.5 Pure Functions with Caching

```ovum
pure fun Fib(n: Int): Int {
    if (n <= 1) return n;
    return Fib(n - 1) + Fib(n - 2);
}
// For user-defined reference types as parameters, ensure they implement Comparable.
```

### 9.6 `is`, `as`, and ByteArray Casts

```ovum
fun DemoCasts(obj: Object): Void {
    // Upcast (implicit) and downcast (explicit)
    if (obj is Point) {
        let p: Point = obj as Point;   // safe after 'is'
        sys::Print(p.ToString());
    } else {
        // obj as Point;   // would abort program (unhandleable) if uncommented
    }

    // Explicit Bool cast
    let b1: Bool = 0 as Bool;       // false
    let b2: Bool = 42 as Bool;      // true
    let b3: Bool = obj as Bool;     // true if obj is a valid, live reference

    // Unsafe: view any value as bytes
    unsafe {
        let cview: ByteArray = obj as ByteArray;        // const byte view
        let mview: ByteArray = (obj as mut ByteArray);  // mutable byte view
        // ... use cview/mview ...
    }
}
```

### 9.7 Unsafe + Interop Example

```ovum
fun DemoInterope(): Void {
    let text: String = "Hello";
    let inBytes: ByteArray = text.ToUtf8NullTerminated();
    let outBytes: ByteArray = ByteArray(8); // reserve space if C func writes a 64-bit result

    unsafe {
        let status: Int = sys::Interope("libc.so", "puts", inBytes, outBytes);
        // Handle status / parse outBytes...
    }
}
```

### 9.8 Full Example Program

```ovum
#define GREET_FEATURE

interface Runner : Object { fun Run(): Int; }

class App implements Runner, StringConvertible {
    private let Name: String

    public fun App(name: String): App { this.Name = name; return this; }

    public override fun ToString(): String { return "App(" + Name + ")"; }

    public override fun Run(): Int {
        #ifdef GREET_FEATURE
        sys::Print("Welcome, " + Name + "!");
        #else
        sys::Print("Running...");
        #endif
        return 0;
    }
}

pure fun Add(a: Int, b: Int): Int { return a + b; }

// Entry point with StringArray
fun Main(args: StringArray): Int {
    let app: Runner = App("Ovum");
    sys::Print("App info: " + (app as App).ToString());

    sys::Print("2 + 3 = " + Add(2, 3).ToString());
    return app.Run();
}
```

---

## 10) Build & Run (Conceptual)

1. **Compile** `.ovum` sources → Ovum bytecode (with preprocessing) using Ovum runner.
2. **Execute** on the Ovum VM (single-threaded):

   * Bytecode interpretation → JIT-optimized native where hot.
   * GC manages memory automatically.
   * Entry function: `Main(args: StringArray): Int`.

---

## 11) Notes & Best Practices

* Prefer **immutable** data; use `mut` only when necessary.
* Implement **`StringConvertible`** for diagnostics (`ToString`).
* When using **pure functions** with custom types, implement **`Comparable`**.
* Avoid global `mut`; if necessary, **isolate in `unsafe`** with rationale.
* Keep names **PascalCase** for classes/functions/methods; keep **keywords lowercase**.
* `String` and all array classes are **reference (non-primitive)** types.

---

*© Ovum Project. Licensed under GNU GPLv3 (see separate LICENSE).*
