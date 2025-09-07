# Ovum Programming Language

This README (for `.ovum` sources) describes **Ovum**: syntax & semantics, nullable types, formal EBNF grammar, lexer & parser rules, runtime & VM requirements, unsafe operations, casting, functional objects, and code examples (snippets + a complete program).

If you want to contribute to the project, please read the [CONTRIBUTING.md](./CONTRIBUTING.md) file.
If you want to build the project, please read the [BUILD.md](./BUILD.md) file.

---

## 1) Overview

**Ovum** is a new general-purpose programming language with a design emphasis on safety, purity, and clarity. It is a strongly **statically typed**, **single-threaded** language with Kotlin-like syntax and a focus on safety, clarity, and performance.

### Design Philosophy

Ovum's core design principles center around:

* **Memory Safety**: Automatic memory management through garbage collection eliminates whole classes of bugs like dangling pointers, memory leaks, and buffer overruns.
* **Immutability by Default**: Variables, object fields, and function parameters are constant by default, reducing unintended side effects and making code easier to reason about.
* **Pure Functions**: Functions with no side effects whose results can be cached for performance optimization through memoization.
* **Interface-Based Polymorphism**: No class inheritance; polymorphism achieved through interface implementation and composition, avoiding complex inheritance hierarchies.
* **Explicit Unsafe Operations**: Low-level or potentially unsafe operations are isolated within explicit `unsafe { ... }` blocks.
* **Less is More**: The language is designed to be simple and easy to learn, with a focus on clarity and readability. The less ways there are to do the same thing, the better.

### Key Design Points

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

Ovum has a rich type system with primitive types and user-defined types. The type system is static and does not permit implicit type coercions (an `Int` won't automatically become a `Float` without an explicit cast, for example).

### Primitive Types

* `Int` (8 bytes) - 64-bit signed integer
* `Float` (8 bytes) - 64-bit floating-point number
* `Bool` - Boolean value (`true` or `false`)
* `Char` - single character
* `Byte` - 8-bit unsigned integer
* `Pointer` - raw memory address *(only meaningful in `unsafe` code)*

> Any primitive may have a **nullable** counterpart via `?` (e.g., `Int?`).

### Non-Primitive / Reference Types

* `String` *(reference type; not primitive)* - text data
* **Array classes (no templates / generics):**

  * For primitives: `IntArray`, `FloatArray`, `BoolArray`, `CharArray`, `ByteArray`, `PointerArray`
  * For objects: **`ObjectArray`**
  * **Convenience**: `StringArray` (array of `String`, used for `Main`)

### Passing Semantics

* **Primitive types**: passed by value (copied)
* **Reference types**: all user-defined and non-primitive types (including `String` and all arrays) are **passed by reference** (const by default)
* **Immutability**: By default, references are considered immutable (the function cannot rebind them or mutate the object's content unless allowed via `mut`)

### Type System Characteristics

* **Static typing**: Every variable and expression has a type that is checked at compile time
* **No implicit conversions**: Explicit casting required between different types
* **Type safety**: Prevents type errors and provides better documentation of code intent
* **Nullable types**: Any type can be made nullable by appending `?`

---

## 3) Lexical Structure

Ovum's source code uses a lexical syntax familiar to C-style and Kotlin-style languages:

### Identifiers

Names for variables, functions, classes, etc., consist of letters, digits, and underscores, and must not begin with a digit. For example: `myVar`, `compute_sum`, `GraphNode`. Identifiers are case-sensitive.

**Naming Convention**: Classes, functions, methods use **PascalCase** (e.g., `Main`, `ToString`, `IsLess`). Keywords/modifiers remain lowercase (`class`, `interface`, `mut`, `override`, `pure`, `unsafe`, etc.).

### Keywords

Ovum reserves certain words like `fun`, `class`, `interface`, `mut`, `override`, `pure`, `if`, `else`, `for`, `while`, `return`, `unsafe`, `let`, `static`, `public`, `private`, `implements`, `as`, `is`, `null`, `true`, `false`, etc. These cannot be used as identifiers.

### Literals

* **Numeric literals**: integers (e.g., `42`, `-17`) and floating-point numbers (e.g., `3.14`, `-2.5`)
* **Boolean literals**: `true`, `false`
* **Character literals**: single characters in single quotes (e.g., `'A'`, `'\n'`, `'\t'`)
* **String literals**: text in double quotes (e.g., `"hello"`, `"world"`) with escape sequences like `"\n"` for newline, `"\t"` for tab, `"\\"` for backslash, `"\""` for quote

### Operators and Punctuation

* **Arithmetic**: `+`, `-`, `*`, `/`, `%`
* **Comparison**: `==`, `!=`, `<`, `<=`, `>`, `>=`
* **Boolean logic**: `&&` (logical AND), `||` (logical OR), `!` (negation), `xor` (exclusive OR)
* **Assignment**: `=`
* **Null handling**: `?.` (safe call), `?:` (Elvis), `!!` (non-null assertion)
* **Type operations**: `as` (cast), `is` (type test)
* **Punctuation**: `,` (comma), `;` (semicolon), `:` (colon), `()` (parentheses), `{}` (braces), `[]` (brackets)
* **Namespace resolution**: `::`

### Comments

* **Line comments**: start with `//` and continue to the end of the line
* **Block comments**: start with `/*` and end with `*/`, can span multiple lines. Nested comments are not allowed

### Whitespace

Spaces, tabs, and newlines are generally ignored outside of separating tokens. Indentation is not significant (Ovum is not whitespace-sensitive except that newlines can terminate statements if no semicolon is present).

---

## 4) Nullability & Boolean Logic

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

## 5) Control Flow

Ovum supports standard control flow constructs following structured programming principles:

### Conditional Execution

**If/Else**: Conditional execution with syntax `if (condition) { ... } else { ... }`. The condition must be a Boolean expression (`Bool`). Braces are required for blocks, but for single statements the braces can be omitted (though using braces is encouraged for clarity).

```ovum
if (x > 0) {
    sys::Print("Positive")
} else if (x < 0) {
    sys::Print("Negative")
} else {
    sys::Print("Zero")
}
```

### Loops

**While Loop**: `while (condition) { ... }` repeats the body while the condition is true.

```ovum
mut i: Int = 0
while (i < 10) {
    sys::Print(i.ToString())
    i = i + 1
}
```

**For Loop**: `for (item in collection) { ... }` iterates over elements of a collection (arrays, etc.).

```ovum
for (item in items) {
    sys::Print(item.ToString())
}
```

### Flow Control

**Return**: `return expression;` exits the current function with the given value (or `return;` with no value to exit a void function). In pure functions, a return simply provides the result; in impure, it may terminate early as usual.

**Break/Continue**: `break` exits a loop immediately, `continue` skips to the next iteration of the loop.

```ovum
mut i: Int = 0
while (i < 10) {
    if (i == 5) {
        break  // Exit loop
    }
    if (i == 3) {
        i = i + 1
        continue  // Skip to next iteration
    }
    sys::Print(i.ToString())
    i = i + 1
}
```

All control flow follows structured programming principles (no `goto`).

---

## 6) Syntax & Semantics (Description)

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

## 7) Expressions and Operators

Expressions in Ovum include literal values, variable references, function calls, method calls, field accesses, and combinations of these with operators. Operator syntax and precedence are designed to be familiar to C/Java/Kotlin developers.

### Arithmetic Operators

* `+` (addition) - operates on numeric types and may be overloaded internally for string concatenation
* `-` (subtraction) - operates on numeric types
* `*` (multiplication) - operates on numeric types
* `/` (division) - operates on numeric types
* `%` (modulo) - operates on numeric types

### Comparison Operators

* `==` (equality) - most types can be compared for equality
* `!=` (inequality) - opposite of equality
* `<`, `<=`, `>`, `>=` (ordering) - only valid on types that have a defined ordering (numeric types or classes implementing `IComparable`)

### Logical Operators

* `&&` (logical AND) - short-circuit evaluation
* `||` (logical OR) - short-circuit evaluation
* `!` (negation) - unary operator
* `xor` (exclusive OR) - infix operator on `Bool`

### Assignment Operator

* `=` (assignment) - assigns a value to a mutable variable or field. The left-hand side must be a mutable variable or field.

### Member Access

* `object.field` - access a field of an object
* `object.method()` - call a method on an object
* `object?.field` - safe field access (returns null if object is null)
* `object?.method()` - safe method call (returns null if object is null)

### Type Operations

* `expr as Type` - explicit cast (downcast yields nullable type)
* `expr is Type` - type test (returns `Bool`)

### Null Handling

* `expr?.member` - safe call (calls only if expr is not null)
* `expr ?: default` - Elvis operator (returns expr if not null, otherwise default)
* `expr!!` - non-null assertion (throws error if expr is null)

### Namespace Resolution

* `Namespace::Name` - refers to a definition from a specific namespace (e.g., `sys::Print`)

### No User-Defined Operators

Programmers cannot create new operator symbols or overload the existing ones for user-defined types. The set of operators and their meanings are fixed by the language. This keeps the language syntax clear and consistent and avoids operator overloading misuse.

---

## 8) Lexer & Parser Rules

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

## 8) Memory Management and Runtime

One of Ovum's core principles is memory safety. Memory is managed by the runtime's garbage collector (GC), which automatically frees objects that are no longer in use, eliminating whole classes of bugs like dangling pointers, memory leaks, and buffer overruns.

### Automatic Memory Management

* **No Manual Memory Management**: There are no language constructs for pointer arithmetic, manual memory allocation (e.g., no `malloc`/`free` or `new`/`delete` outside of what the language runtime provides), nor explicit memory deallocation by the programmer.
* **Garbage Collection**: The runtime includes a garbage collector that runs periodically (or when allocation thresholds are exceeded) to reclaim memory. It finds objects that are no longer reachable from any live variables or object fields and frees them.
* **Modern GC Algorithm**: Ovum's GC is likely a modern algorithm (possibly generational, parallel, or incremental) to minimize pause times, but these are internal implementation details.

### Just-In-Time Compilation

* **JIT Compilation**: The VM includes a Just-In-Time compiler (JIT) that can compile frequently executed code paths to native machine code for speed.
* **Hot Path Optimization**: Initially, Ovum bytecode might be interpreted, but as functions or loops become "hot" (executed often), the JIT will optimize them.
* **Hybrid Approach**: This gives the flexibility of an interpreter (fast startup, platform independence of bytecode) with the performance of compiled code in long-running processes.

### Runtime, VM & Platform Support

* **Execution**: Source (`.ovum`) → bytecode → **Ovum VM**.
* **JIT**: Hot paths compiled to native for performance.
* **GC**: Automatic memory reclamation; **no manual memory management**.
* **Single-threaded**: Execution model and VM are single-threaded.
* **Architectures**: **amd64** and **arm64** supported.
* **Numeric widths**: `Int` **8 bytes**, `Float` **8 bytes**.

---

## 9) Runtime and Execution Model

The Ovum compiler translates Ovum source code into Ovum bytecode or an intermediate representation, which is executed on the Ovum Virtual Machine (OVM). The OVM provides a sandboxed, platform-independent environment for Ovum programs.

### Development Workflow

1. **Write Ovum source code** in `.ovum` files
2. **Compile** using the Ovum compiler, which will:
   - Parse using the grammar rules
   - Type-check (ensure types match and all variables are defined)
   - Enforce const/pure rules
   - Produce bytecode or an executable
3. **Execute** using the Ovum VM, which will:
   - Load the bytecode
   - Resolve any imports (linking together modules)
   - Start executing (usually beginning with `Main(args: StringArray): Int`)
   - Apply JIT optimization to hot code paths
   - Manage memory with garbage collection

### Platform Requirements

* **Ovum VM**: Required on the target platform (distributed as standalone application or embedded)
* **Architecture Support**: JIT compiler generates code specific to host CPU architecture for performance
* **Portability**: Bytecode is portable across platforms; only the VM's JIT component is platform-specific
* **Dependencies**: Any necessary native libraries if the program uses `sys::Interope` to call external code

### Execution Characteristics

* **Single-threaded**: Execution model and VM are single-threaded
* **No Concurrency Primitives**: No built-in threading or concurrency features
* **Structured Programming**: All control flow follows structured programming principles (no `goto`)
* **Entry Point**: Programs start with `Main(args: StringArray): Int` function

---

## 10) System Library & Interop

* `sys::Print(msg: String): Void`
* `sys::Time(): Int`
* `sys::Sleep(ms: Int): Void`
* `sys::Exit(code: Int): Never` *(terminates the process)*
* **Interop (FFI)**:

  * `sys::Interope(dllName: String, functionName: String, input: ByteArray, output: ByteArray): Int`
  * **All interop calls are `unsafe`.**

> Names use **PascalCase** (e.g., `Print`, `Time`, `Sleep`, `Exit`, `Interope`). Namespace remains `sys`.

---

## 11) Unsafe Operations (Recap)

Allowed **only** inside `unsafe { ... }`:

* Declaring/writing **global `mut`** variables and **`static mut`** fields.
* Casting const → mutable.
* Using **`Pointer`**, address-of and dereference.
* **Manual destructor** calls.
* Any **`sys::Interope`** invocation.
* Casting any value to **(const or mutable) `ByteArray`**.

---

## 12) Code Examples

### 12.1 Entry point (`StringArray`)

```ovum
// .ovum file
fun Main(args: StringArray): Int {
    val count: Int = args.Length() ?: 0
    sys::Print("Args count: " + count.ToString())
    return 0
}
```

### 12.2 Variables, Nulls, Elvis, Safe Calls

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

### 12.3 Interfaces, Classes, Fields, Overrides

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

### 12.4 Standard Interfaces (`IStringConvertible`, `IComparable`, `IHashable`)

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

### 12.5 Pure Functions with Caching

```ovum
pure fun Fib(n: Int): Int {
    if (n <= 1) return n
    return Fib(n - 1) + Fib(n - 2)
}
// For user-defined reference types as parameters, implement IComparable.
```

### 12.6 `is`, `as`, `!!`, and ByteArray Casts

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

### 12.7 Functional Objects (`call`) & Literals

```ovum
interface CustomFunctional {
    call(a: Int?, b: Int?): Int
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

### 12.8 Control Flow Examples

```ovum
fun DemoControlFlow(): Void {
    mut i: Int = 0
    
    // While loop with break and continue
    while (i < 10) {
        if (i == 3) {
            i = i + 1
            continue  // Skip to next iteration
        }
        if (i == 7) {
            break  // Exit loop
        }
        sys::Print("i = " + i.ToString())
        i = i + 1
    }
    
    // For loop over array
    let numbers: IntArray = IntArray(3)
    numbers[0] = 10
    numbers[1] = 20
    numbers[2] = 30
    
    for (num in numbers) {
        sys::Print("Number: " + num.ToString())
    }
}
```

### 12.9 Memory Management and Unsafe Operations

```ovum
fun DemoUnsafeOperations(): Void {
    // Unsafe block for low-level operations
    unsafe {
        // Global mutable state (unsafe)
        static mut globalCounter: Int = 0
        globalCounter = globalCounter + 1
        
        // Pointer operations (unsafe)
        let obj: Point = Point(10, 20)
        let ptr: Pointer = &obj  // address-of
        let deref: Point = *ptr  // dereference
        
        // ByteArray casting (unsafe)
        let bytes: ByteArray = (obj as ByteArray)
        let mutableBytes: ByteArray = (obj as mut ByteArray)
        
        // Foreign function interface (unsafe)
        let input: ByteArray = "Hello".ToUtf8Bytes()
        let output: ByteArray = ByteArray(4)
        let result: Int = sys::Interope("libc.so", "strlen", input, output)
    }
}
```

### 12.10 Complete Program Example

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

pure fun ProcessNumbers(calc: ICalculator, numbers: IntArray): Int {
    mut result: Int = 0
    for (num in numbers) {
        result = result + calc.Calculate(num, 2)
    }
    return result
}

fun Main(args: StringArray): Int {
    let numbers: IntArray = IntArray(3)
    numbers[0] = 5
    numbers[1] = 10
    numbers[2] = 15
    
    let adder: ICalculator = Adder()
    let multiplier: ICalculator = Multiplier()
    
    let sumResult: Int = ProcessNumbers(adder, numbers)
    let productResult: Int = ProcessNumbers(multiplier, numbers)
    
    sys::Print("Sum result: " + sumResult.ToString())
    sys::Print("Product result: " + productResult.ToString())
    
    return 0
}
```

---

## 13) Build & Run (Conceptual)

1. **Compile** `.ovum` sources → Ovum bytecode (preprocessor applied).
2. **Execute** on the Ovum VM (single-threaded):

   * Bytecode interpretation with JIT for hot paths.
   * GC manages memory automatically.
   * Entry function: `Main(args: StringArray): Int`.

---

## 14) Notes & Best Practices

* Prefer **immutable** data; use `mut` only when necessary.
* Implement **`IStringConvertible`** for diagnostics (`ToString`).
* For **pure** functions with custom types, implement **`IComparable`**.
* Avoid global `mut`; if necessary, **isolate in `unsafe`** with rationale.
* Keep names **PascalCase** for classes/functions/methods; keep **keywords lowercase**.
* `String` and all array classes are **reference (non-primitive)** types.
