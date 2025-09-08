# Code Examples

Here are some code examples to help you get started with Ovum.

## 1) Entry point (`StringArray`)

```ovum
// .ovum file
fun Main(args: StringArray): Int {
    val count: Int = args.Length()
    sys::Print("Args count: " + count.ToString())
    return 0
}
```

## 2) Variables, Nulls, Elvis, Safe Calls

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

## 3) Interfaces, Classes, Fields, Overrides

```ovum
interface IGreeter {
    fun Greet(name: String): String  // public + virtual by default
}

class FriendlyGreeter implements IGreeter {
    private val Prefix: String = "Hello"
    public  var Suffix: String = "!"

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

## 4) Standard Interfaces (`IStringConvertible`, `IComparable`, `IHashable`)

```ovum
interface IStringConvertible { fun ToString(): String }
interface IComparable        { fun IsLess(other: Object): Bool }
interface IHashable          { fun GetHash(): Int }

class Point implements IStringConvertible, IComparable, IHashable {
    public val X: Int
    public val Y: Int

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

## 5) Pure Functions with Caching

```ovum
pure fun Fib(n: Int): Int {
    if (n <= 1) return n
    return Fib(n - 1) + Fib(n - 2)
}
// For user-defined reference types as parameters, implement IComparable.
```

## 6) `is`, `as`, `!!`, and ByteArray Casts

```ovum
fun DemoCasts(obj: Object): Void {
    if (obj is Point) {
        val p: Point = (obj as Point)!!         // nullable cast + assert
        sys::Print(p.ToString())
    }

    // Bool cast
    val b1: Bool = (0 as Bool)                  // false
    val b2: Bool = (42 as Bool)                 // true
    val b3: Bool = (obj as Bool)                // always true
    val b4: Bool = ((obj as Point) as Bool)     // true if obj is a Point

    // Unsafe: raw byte views
    unsafe {
        val bytesConst: ByteArray = (obj as ByteArray)
        val bytesMut  : ByteArray = (obj as var ByteArray)
    }
}
```

## 7) Functional Objects (`call`) & Literals

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
    return (a ?: 0) + (b ?: 0)
}

fun Main(args: StringArray): Int {
    return AddNullable(2, DefinedFunctional(-1)(2))
}
```

## 8) Control Flow Examples

```ovum
fun DemoControlFlow(): Void {
    var i: Int = 0
    
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
    val numbers: IntArray = IntArray(3)
    numbers[0] = 10
    numbers[1] = 20
    numbers[2] = 30
    
    for (num in numbers) {
        sys::Print("Number: " + num.ToString())
    }
}
```

## 9) Memory Management and Unsafe Operations

```ovum
fun DemoUnsafeOperations(): Void {
    // Unsafe block for low-level operations
    unsafe {
        // Global mutable state (unsafe)
        static var globalCounter: Int = 0
        globalCounter = globalCounter + 1
        
        // Pointer operations (unsafe)
        val obj: Point = Point(10, 20)
        val ptr: Pointer = &obj  // address-of
        val deref: Object = *ptr  // dereference to Object, Pointer is not typed
        
        // ByteArray casting (unsafe)
        val bytes: ByteArray = (obj as ByteArray)
        val mutableBytes: ByteArray = (obj as var ByteArray)
        
        // Foreign function interface (unsafe)
        val input: ByteArray = "Hello".ToUtf8Bytes()
        val output: ByteArray = ByteArray(4)
        val result: Int = sys::Interope("libc.so", "strlen", input, output)
    }
}
```

## 10) Complete Program Example

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
    var result: Int = 0
    for (num in numbers) {
        result = result + calc.Calculate(num, 2)
    }
    return result
}

fun Main(args: StringArray): Int {
    val numbers: IntArray = IntArray(3)
    numbers[0] = 5
    numbers[1] = 10
    numbers[2] = 15
    
    val adder: ICalculator = Adder()
    val multiplier: ICalculator = Multiplier()
    
    val sumResult: Int = ProcessNumbers(adder, numbers)
    val productResult: Int = ProcessNumbers(multiplier, numbers)
    
    sys::Print("Sum result: " + sumResult.ToString())
    sys::Print("Product result: " + productResult.ToString())
    
    return 0
}
```
