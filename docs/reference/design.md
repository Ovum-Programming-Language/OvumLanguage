# Overview

**Ovum** is a new general-purpose programming language with a design emphasis on safety, purity, and clarity. It is a strongly **statically typed**, **single-threaded** language with Kotlin-like syntax and a focus on safety, clarity, and performance.

## Design Philosophy

Ovum's core design principles center around:

* **Memory Safety**: Automatic memory management through garbage collection eliminates whole classes of bugs like dangling pointers, memory leaks, and buffer overruns.
* **Immutability by Default**: Variables, object fields, and function parameters are constant by default, reducing unintended side effects and making code easier to reason about.
* **Pure Functions**: Functions with no side effects whose results can be cached for performance optimization through memoization.
* **Interface-Based Polymorphism**: No class inheritance; polymorphism achieved through interface implementation and composition, avoiding complex inheritance hierarchies.
* **Explicit Unsafe Operations**: Low-level or potentially unsafe operations are isolated within explicit `unsafe { ... }` blocks.
* **Less is More**: The language is designed to be simple and easy to learn, with a focus on clarity and readability. The less ways there are to do the same thing, the better.

## Key Design Points

* **Strong static typing** with **immutability by default** (`var` required for mutation).
* **Nullable types** and Kotlin-style null-handling: `Type?`, safe calls `?.`, Elvis `?:`.
* **Pure functions** (no side effects, VM-level result caching).
* **Classes & interfaces**

  * **No class inheritance**; classes only **implement interfaces**.
  * **Root `Object`** is implicit for all classes and interfaces (no need to write `: Object`). It declares only a **virtual destructor**.
  * Interfaces are named with **leading `I`** (C# style): e.g., `IGreeter`, `IComparable`.
  * Interface methods are **public** and **virtual** by default.
  * Class methods implementing interface members must be marked `override`.
  * **Access modifiers are mandatory** for all fields and methods in classes (`public`/`private`).
  * **Fields use `val` (immutable) or `var` (mutable)**.
* **Namespaces** with `::` resolution (e.g., `sys::Print`).
* **Functional objects** (`call` member) for functional programming.
* **Type aliases** (`typealias` keyword) for better readability.
* **Built-in operators**; **no user-defined operators**.
* **Preprocessor**: `#import`, `#define`, `#ifdef`, `#ifndef`, `#else`, `#undef`.
* **Managed runtime**: VM with **JIT** and **GC**; **no manual memory management**.
* **Single-threaded runtime**: no concurrency primitives.
* **Passing semantics**: all user-defined and non-primitive types (including `String` and all arrays) are **passed by reference** (const by default).
* **File extension**: `.ovum`.
