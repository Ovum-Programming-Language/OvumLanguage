# System Library & Interop

* `sys::Print(msg: String): Void`
* `sys::Time(): Int`
* `sys::Sleep(ms: Int): Void`
* `sys::Exit(code: Int): Never` *(terminates the process)*
* **Interop (FFI)**:

  * `sys::Interope(dllName: String, functionName: String, input: ByteArray, output: ByteArray): Int`
  * **All interop calls are `unsafe`.**

> Names use **PascalCase** (e.g., `Print`, `Time`, `Sleep`, `Exit`, `Interope`). Namespace remains `sys`.