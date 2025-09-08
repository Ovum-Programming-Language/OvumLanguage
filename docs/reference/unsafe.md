# Unsafe Operations

Allowed **only** inside `unsafe { ... }`:

* Declaring/writing **global `var`** variables and **`static var`** fields.
* Casting const → mutable.
* Using **`Pointer`**, address-of and dereference.
* **Manual destructor** calls.
* Any **`sys::Interope`** invocation.
* Casting any value to **(const or mutable) `ByteArray`**.
