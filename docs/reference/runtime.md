# Memory Management and Runtime

One of Ovum's core principles is memory safety. Memory is managed by the runtime's garbage collector (GC), which automatically frees objects that are no longer in use, eliminating whole classes of bugs like dangling pointers, memory leaks, and buffer overruns.

## Automatic Memory Management

* **No Manual Memory Management**: There are no language constructs for pointer arithmetic, manual memory allocation (e.g., no `malloc`/`free` or `new`/`delete` outside of what the language runtime provides), nor explicit memory deallocation by the programmer.
* **Garbage Collection**: The runtime includes a garbage collector that runs periodically (or when allocation thresholds are exceeded) to reclaim memory. It finds objects that are no longer reachable from any live variables or object fields and frees them.
* **Modern GC Algorithm**: Ovum's GC is likely a modern algorithm (possibly generational, parallel, or incremental) to minimize pause times, but these are internal implementation details.

## Just-In-Time Compilation

* **JIT Compilation**: The VM includes a Just-In-Time compiler (JIT) that can compile frequently executed code paths to native machine code for speed.
* **Hot Path Optimization**: Initially, Ovum bytecode might be interpreted, but as functions or loops become "hot" (executed often), the JIT will optimize them.
* **Hybrid Approach**: This gives the flexibility of an interpreter (fast startup, platform independence of bytecode) with the performance of compiled code in long-running processes.

## Runtime, VM & Platform Support

* **Execution**: Source (`.ovum`) → bytecode → **Ovum VM**.
* **JIT**: Hot paths compiled to native for performance.
* **GC**: Automatic memory reclamation; **no manual memory management**.
* **Single-threaded**: Execution model and VM are single-threaded.
* **Architectures**: **amd64** and **arm64** supported.
* **Numeric widths**: `Int` **8 bytes**, `Float` **8 bytes**.

The Ovum compiler translates Ovum source code into Ovum bytecode or an intermediate representation, which is executed on the Ovum Virtual Machine (OVM). The OVM provides a sandboxed, platform-independent environment for Ovum programs.

## Development Workflow

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

## Platform Requirements

* **Ovum VM**: Required on the target platform (distributed as standalone application or embedded)
* **Architecture Support**: JIT compiler generates code specific to host CPU architecture for performance
* **Portability**: Bytecode is portable across platforms; only the VM's JIT component is platform-specific
* **Dependencies**: Any necessary native libraries if the program uses `sys::Interope` to call external code

## Execution Characteristics

* **Single-threaded**: Execution model and VM are single-threaded
* **No Concurrency Primitives**: No built-in threading or concurrency features
* **Structured Programming**: All control flow follows structured programming principles (no `goto`)
* **Entry Point**: Programs start with `Main(args: StringArray): Int` function
