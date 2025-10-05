# Object Model 

Ovum uses an interface-based object model with no class inheritance. All types derive from `Object`, and polymorphism is achieved through interface implementation.

## Object Hierarchy

### Root Type: `Object`

* **`Object`** - implicit root of all reference types
  * Contains only a virtual destructor
  * Enables safe, uniform storage (e.g., in `ObjectArray`)
  * All user-defined classes implicitly extend `Object`
  * Cannot be instantiated directly

```ovum
// Object is the root of all reference types
val obj: Object = Point(10, 20)  // Upcast to Object
val point: Point? = (obj as Point)  // Downcast to Point (nullable)
```

## Standard Interfaces

All standard interfaces implicitly extend `Object` and provide common functionality:

### `IStringConvertible`

Provides string representation capability:

```ovum
interface IStringConvertible {
    fun ToString(): String
}

class Person implements IStringConvertible {
    public val Name: String
    public val Age: Int
    
    public fun Person(name: String, age: Int): Person {
        this.Name = name
        this.Age = age
        return this
    }
    
    public override fun ToString(): String {
        return Name + " (" + Age.ToString() + ")"
    }
}
```

### `IComparable`

Provides ordering capability for sorting and comparison:

```ovum
interface IComparable {
    fun IsLess(other: Object): Bool
}

class Point implements IComparable {
    public val X: Int
    public val Y: Int
    
    public fun Point(x: Int, y: Int): Point {
        this.X = x
        this.Y = y
        return this
    }
    
    public override fun IsLess(other: Object): Bool {
        if (!(other is Point)) return false
        val p: Point = (other as Point) ?: Point(0, 0)
        if (this.X != p.X) return this.X < p.X
        return this.Y < p.Y
    }
}
```

**Required for pure function parameters** (provides stable ordering/keys).

### `IHashable`

Provides hash code generation for use in hash tables:

```ovum
interface IHashable {
    fun GetHash(): Int
}

class Point implements IHashable {
    public val X: Int
    public val Y: Int
    
    public override fun GetHash(): Int {
        return (X * 1315423911) ^ (Y * 2654435761)
    }
}
```

## Class Definitions

### Basic Class Syntax

```ovum
class ClassName implements Interface1, Interface2 {
    // Fields
    // Constructor
    // Methods
    // Destructor (optional)
}
```

### Fields

Fields can be immutable (`val`) or mutable (`var`):

```ovum
class BankAccount {
    public val AccountNumber: String      // Immutable field
    public var Balance: Float            // Mutable field
    private val CreatedDate: Int         // Private field
}
```

### Access Modifiers

* **`public`** - accessible from anywhere
* **`private`** - accessible only within the same class

```ovum
class DataContainer {
    public val PublicData: String = "Public"
    private val PrivateData: String = "Private"
    
    public fun GetPrivateData(): String {
        return PrivateData  // OK: accessing private member from within class
    }
}
```

### Constructors

Constructors initialize new instances:

```ovum
class Rectangle {
    public val Width: Float
    public val Height: Float
    
    public fun Rectangle(width: Float, height: Float): Rectangle {
        this.Width = width
        this.Height = height
        return this
    }
    
    // Multiple constructors (overloading)
    public fun Rectangle(size: Float): Rectangle {
        this.Width = size
        this.Height = size
        return this
    }
}
```

### Methods

Methods can be regular, pure, or override:

```ovum
class Calculator implements IStringConvertible {
    public fun Add(a: Int, b: Int): Int {
        return a + b
    }
    
    public pure fun Multiply(a: Int, b: Int): Int {
        return a * b
    }
    
    public override fun ToString(): String {
        return "Calculator"
    }
}
```

### Destructors

Optional destructors are called by the garbage collector:

```ovum
class FileHandler {
    private val FilePath: String
    
    public destructor(): Void {
        // Release file handles, network connections, etc.
        // Manual calls are unsafe
    }
}
```

## Interface Definitions

### Basic Interface Syntax

```ovum
interface InterfaceName extends BaseInterface {
    // Method declarations
    // Property declarations
}
```

### Method Declarations

```ovum
interface IShape {
    fun GetArea(): Float
    fun GetPerimeter(): Float
    fun Draw(): Void
}

interface IColorable {
    fun SetColor(color: String): Void
    fun GetColor(): String
}
```

### Property Declarations

Interfaces can declare properties that implementing classes must provide:

```ovum
interface IReadable {
    val IsReadable: Bool
    val Content: String
}

class Document implements IReadable {
    public val IsReadable: Bool = true
    public val Content: String
}
```

### Multiple Interface Implementation

Classes can implement multiple interfaces:

```ovum
class ColoredRectangle implements IShape, IColorable {
    public val Width: Float
    public val Height: Float
    public var Color: String
    
    public fun ColoredRectangle(width: Float, height: Float, color: String): ColoredRectangle {
        this.Width = width
        this.Height = height
        this.Color = color
        return this
    }
    
    public override fun GetArea(): Float {
        return Width * Height
    }
    
    public override fun GetPerimeter(): Float {
        return 2 * (Width + Height)
    }
    
    public override fun Draw(): Void {
        sys::Print("Drawing " + Color + " rectangle")
    }
    
    public override fun SetColor(color: String): Void {
        this.Color = color
    }
    
    public override fun GetColor(): String {
        return Color
    }
}
```

## Type Casting and Tests

**Upcasting** (to `Object` or interfaces): safe, non-nullable
**Downcasting** (to concrete classes): nullable result

```ovum
val point: Point = Point(10, 20)
val obj: Object = point                    // Upcast to Object
val comparable: IComparable = point        // Upcast to interface

// Downcasting with type test
if (obj is Point) {
    val p: Point = (obj as Point) ?: Point(0, 0)
    sys::Print("Point: " + p.ToString())
}

// Type test operator
if (shape is ColoredRectangle) {
    val rect: ColoredRectangle = (shape as ColoredRectangle) ?: ColoredRectangle(0, 0, "red")
    sys::Print("Rectangle color: " + rect.GetColor())
}
```

**Unsafe casting** requires `unsafe` blocks:
```ovum
unsafe {
    val obj: Object = Point(10, 20)
    val bytes: ByteArray = (obj as ByteArray)           // Raw byte view
    val mutableBytes: ByteArray = (obj as var ByteArray) // Mutable byte view
}
```

## Functional Objects

Classes and interfaces can define a special `call` member to make them callable:

```ovum
interface IAdder {
    call(a: Int, b: Int): Int
}

class Calculator implements IAdder {
    public var Multiplier: Int
    
    public call(a: Int, b: Int): Int {
        return (a + b) * Multiplier
    }
}

val calc: IAdder = Calculator(2)
val result: Int = calc(5, 3)  // Calls the call method: (5 + 3) * 2 = 16
```

**Function literals** can be assigned to interfaces with compatible `call` signatures:

```ovum
interface IBinaryOperation {
    call(a: Int, b: Int): Int
}

val add: IBinaryOperation = pure fun(a: Int, b: Int): Int {
    return a + b
}

val sum: Int = add(5, 3)  // 8
```

## Memory Management

Ovum uses garbage collection for automatic memory management:

```ovum
fun CreateObjects(): Void {
    val point1: Point = Point(10, 20)  // Allocated on heap
    val point2: Point = Point(30, 40)  // Allocated on heap
    
    // Objects are automatically collected when no longer referenced
}
```

**Destructors** are called by the garbage collector, not manually:

```ovum
class ResourceManager {
    public destructor(): Void {
        // Release resource - called automatically by GC
    }
}

// Manual destructor calls are unsafe and not recommended
```

## Best Practices

**Interface Design:**
- Keep interfaces focused on single concepts
- Use descriptive names
- Prefer many small interfaces over few large ones

**Class Design:**
- Implement standard interfaces (`ToString()`, `IsLess()`, `GetHash()`)
- Use appropriate access modifiers (private fields when possible)
- Prefer immutability (`val` over `var`)

**Type Safety:**
- Use type tests before casting (`is` before `as`)
- Prefer safe operations
- Handle nullable types properly

```ovum
// Good: focused interfaces
interface IReadable { fun Read(): String }
interface IWritable { fun Write(content: String): Void }

// Good: safe type handling
fun SafeProcessObject(obj: Object?): Void {
    val result: String = obj?.ToString() ?: "null"
    if (obj is Person) {
        val person: Person = (obj as Person) ?: Person("Unknown")
        sys::Print("Person: " + person.ToString())
    }
}
```

> **Naming Conventions**: Classes, functions, methods, and properties use **PascalCase** (e.g., `Main`, `ToString`, `IsLess`). Keywords and modifiers remain lowercase (`class`, `interface`, `var`, `override`, `pure`, `unsafe`, etc.).
