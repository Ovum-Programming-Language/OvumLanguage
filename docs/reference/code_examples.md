# Code Examples

Here are some code examples to help you get started with Ovum.

## 1) Entry point (`StringArray`)

```ovum
// .ovum file
fun Main(args: StringArray): Int {
    val count: Int = args.Length()  // Built-in returns Int
    sys::Print("Args count: " + count.ToString())
    return 0
}
```

## 2) Variables, Nulls, Elvis, Safe Calls

```ovum
fun DemoNulls(): Void {
    val a: Int? = null
    val b: Int? = 5

    val aVal: int = a ?: 0
    val bVal: int = b ?: 0
    val sum: int = aVal + bVal
    sys::Print("Sum = " + Int(sum).ToString())

    val name: String? = null
    val length: int = (name?.Length() ?: 0) as int  // Built-in returns Int
    sys::Print("Name length = " + Int(length).ToString())
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
    public val X: int
    public val Y: int

    public fun Point(x: int, y: int): Point { this.X = x; this.Y = y; return this; }

    public override fun ToString(): String {
        return "(" + Int(X).ToString() + ", " + Int(Y).ToString() + ")"
    }

    public override fun IsLess(other: Object): Bool {
        if (!(other is Point)) return false
        val p: Point? = other as Point
        if (p != null) {
            val nonNullP: Point = p ?: Point(0, 0)  // Use Elvis operator
            if (this.X != nonNullP.X) return this.X < nonNullP.X
            return this.Y < nonNullP.Y
        }
        return false
    }

    public override fun GetHash(): Int {
        val hash: int = (X * 1315423911) ^ (Y * 2654435761)
        return Int(hash)
    }
}
```

## 5) Pure Functions with Caching

```ovum
pure fun Fib(n: int): int {
    if (n <= 1) return n
    val fib1: int = Fib(n - 1)
    val fib2: int = Fib(n - 2)
    return fib1 + fib2
}
// For user-defined reference types as parameters, implement IComparable.
```

## 6) `is`, `as`, and ByteArray Casts

```ovum
fun DemoCasts(obj: Object): Void {
    if (obj is Point) {
        val p: Point? = obj as Point
        if (p != null) {
            val nonNullP: Point = p ?: Point(0, 0)  // Use Elvis operator
            sys::Print(nonNullP.ToString())
        }
    }

    // bool cast
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
    var i: int = 0
    
    // While loop with break and continue
    while (i < 10) {
        if (i == 3) {
            i = i + 1
            continue  // Skip to next iteration
        }
        if (i == 7) {
            break  // Exit loop
        }
        sys::Print("i = " + Int(i).ToString())
        i = i + 1
    }
    
    // For loop over array
    val numbers: IntArray = IntArray(3)
    numbers[0] := 10
    numbers[1] := 20
    numbers[2] := 30
    
    for (num in numbers) {
        sys::Print("Number: " + num.ToString())
    }
}
```

## 9) Unsafe Operations

```ovum
fun DemoUnsafeOperations(): Void {
    // Unsafe block for low-level operations
    unsafe {
        // Global mutable state (unsafe)
        static var globalCounter: int = 0
        globalCounter = globalCounter + 1
        
        // Pointer operations (unsafe)
        val obj: Point = Point(10, 20)
        val ptr: pointer = &obj  // address-of
        val deref: Object = *ptr  // dereference to Object, pointer is not typed
        
        // ByteArray casting (unsafe)
        val bytes: ByteArray = (obj as ByteArray)
        val mutableBytes: ByteArray = (obj as var ByteArray)
        
        // Foreign function interface (unsafe)
        val input: ByteArray = "Hello".ToUtf8Bytes()
        val output: ByteArray = ByteArray(8)
        val result: int = sys::Interope("libc.so", "strlen", input, output)
    }
}
```

## 10) Type Aliases

```ovum
// Define type aliases for better readability
typealias UserId = Int
typealias UserName = String
typealias UserList = ObjectArray

class User {
    public val Id: UserId
    public val Name: UserName
    
    public fun User(id: UserId, name: UserName): User {
        this.Id = id
        this.Name = name
        return this
    }
}

fun ProcessUsers(users: UserList): Void {
    for (i in 0..users.Length()) {
        val user: User? = users[i] as User
        if (user != null) {
            val nonNullUser: User = user ?: User(0, "Unknown")  // Use Elvis operator
            sys::Print("User " + nonNullUser.Id.ToString() + ": " + nonNullUser.Name)
        }
    }
}
```


## 11) Memory Management and Destructors

```ovum
class DatabaseConnection {
    private val ConnectionId: int
    private val IsConnected: bool
    
    public fun DatabaseConnection(id: int): DatabaseConnection {
        this.ConnectionId = id
        this.IsConnected = true
        // Establish database connection
        return this
    }
    
    public fun Query(sql: String): String {
        if (!IsConnected) return "Not connected"
        // Execute query
        return "Query result"
    }
    
    // Destructor called automatically by GC
    public destructor(): Void {
        if (IsConnected) {
            // Close database connection
            sys::Print("Closing connection " + Int(ConnectionId).ToString())
        }
    }
}

fun DemoMemoryManagement(): Void {
    val db: DatabaseConnection = DatabaseConnection(1)
    val result: String = db.Query("SELECT * FROM users")
    sys::Print("Query result: " + result)
    
    // db will be garbage collected automatically
    // destructor will be called by GC
}
```

## 12) Complete Program Example

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
        val calcResult: Int = calc.Calculate(num, 2)
        result = result + calcResult
    }
    return result
}

fun Main(args: StringArray): int {
    val numbers: IntArray = IntArray(3)
    numbers[0] := 5
    numbers[1] := 10
    numbers[2] := 15
    
    val adder: ICalculator = Adder()
    val multiplier: ICalculator = Multiplier()
    
    val sumResult: int = ProcessNumbers(adder, numbers)
    val productResult: int = ProcessNumbers(multiplier, numbers)
    
    sys::Print("Sum result: " + Int(sumResult).ToString())
    sys::Print("Product result: " + Int(productResult).ToString())
    
    return 0
}
```