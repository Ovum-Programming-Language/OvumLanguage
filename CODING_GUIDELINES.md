# Style guide

This document collects all examples of correct code formatting for our project.

Primarily follow the Google Code Style: https://google.github.io/styleguide/cppguide.html.
If there is any conflict, follow this document.
There is a ClangFormat file (.clang-format) you can use for automatic formatting.

### 1. Maximum line length — 80 characters.

### 2. Use spaces, not tabs. Configure your editor for 2 spaces.

### 3. Use snake_case for variable names, and PascalCase for function names:

```c++
int32_t MakingSomeStuff() {
  ...
}

int main() {
  int32_t some_value = 144;
  std::string person_name = "Alice";
  std::string base = "we_will_rock_you";
}
```

BUT! When it comes to class/enum/struct/etc names, use PascalCase.

At the same time, when creating an object of a class, use snake_case.
If it’s a private or protected field, append a trailing underscore.
Constants use PascalCase with a `k` prefix.

```c++
class SomeClass {
public:
  SomeClass();
  void DoSomeStuff();
  int32_t some_value;
  static const int32_t kSomeFixedValue = 10;
private:
  int32_t some_value_;
};

enum Role {
  kOfficer = 0,
  kPostman,
  kWaiter,
  kBooster
}

struct UsersData {
  std::string name[3];
  uint32_t age;
  Role role;
}
```

### 4. Use fixed-width types. For example, `int32_t` instead of `int`, `uint64_t` instead of `unsigned long`.

### 5. Allow `auto` only:
- When the type is explicit (e.g., via an explicit constructor call or otherwise clearly specified)
- For tuple unpacking

IN ALL OTHER CASES — AVOID

### 6. Bracing style:
When declaring new classes/functions/structs/enums/lambdas/etc, put the declaration and opening brace on the same line:

```c++
struct TypicalExperimentData {
  short num_of_experiment;
  int32_t weights[10];
  float temperatures[5];
};

void DoSomeGreatStuff() {
  if (true) {
    std::cout << "Hello, world!";
  } else {
    std::cout << "What a wonderful world!";
  }
}

int main() {
  return 0;
}
```

### 7. Blank lines:

- After each function, class, struct, enum, lambda, etc.
- Before and after each code block inside a function — anything delimited by braces, e.g., `for` or `if`.
- Between variables if they’re unrelated.
- Between `#include` and the code.
- Before `return` in a function.
- Before access specifiers in a class, except the first one.

GodObject.hpp
```c++
class GodObject { // Note the order of access specifiers
public: 
  GodObject();
  ~GodObject();
  int32_t GetSomeValue();
  int32_t CalculateSomething();
  int32_t field;
  
protected:
  int32_t some_value_;
  
private:
  int32_t another_value_;
}
```

GodObject.cpp
```c++
#include "GodObject.hpp"

GodObject::GodObject() : some_value_(0), another_value_(0) {}

GodObject::~GodObject() {}

int32_t GodObject::GetSomeValue() {
  return some_value_;
}

int32_t GodObject::CalculateSomething() {
  int32_t sum = 0;
  
  for (int32_t i = 0; i < 10; ++i) {
    sum += i;
  }
  
  return sum * another_value_;
}
```

### 8. Naming:

- Functions are named by what they do — for example, if a function “finds” something:
  ```c++
  float FindMatrixDeterminant
  ```

In other words, use an appropriate verb.

For example, if we’re making a game — say, Snake. We have the user and the score.
In this case we can describe the user with a class and create at least two public functions:

```c++
float Player::CalculateScore() {
  ...  
}
```

Names should be, as much as possible, **clear**, **short**, and **intuitive** for anyone working on the project.
Maintain a balance between clarity and length: avoid names longer than 20 characters and 4 words.
Use abbreviations only if they are widely accepted (for example, a data format name), and when used, write them in lowercase.

- Name variables **meaningfully**:

```c++
int a = 12; // BAD!

int drops_count = 12; // More or less clear what this is
```

No i, j, k, a, fizz, buzz, etc.

Do not use global variables.
If a variable is used in multiple functions, it must be passed as an argument.


- Name classes using nouns. For example:

```c++
class Engine {
  ...
}

class TableInfo{
  ...
}

class AbstactMixer {
  ...
}
```

- Name structs the same way as classes:

```c++
struct List {
  int8_t item;
  float* pf;
  List* next;
};

struct TypicalExperimentData {
  int16_t num_of_experiment;
  int32_t weights[10];
  float temperatures[3];
};
```

- Name concepts with adjectives:

```c++
template <typename T>
concept Numeric = std::is_arithmetic_v<T>;
```

### 9. Separate the function declaration and its implementation between `.hpp` and `.cpp`, respectively
For example:

main.cpp
```c++
#include "my_func.hpp"

int main() {
  Square(10);
  return 0;
}
```

- Use include guards!

my_func.hpp
```c++
#ifndef MYFUNC_HPP
#define MYFUNC_HPP

void Square(int32_t);

#endif // MYFUNC_HPP
```

- About `include`:
  - Include external libraries via `#include <...>`, and our own files via `#include "..."`.
  - Put all `include`s at the top of the file.
  - Write all `include`s in alphabetical order.
  - Write all `include`s in the following sequence, with each category separated by a blank line:
    - First the standard library
    - Then external libraries
    - Then other libraries of ours
    - Then header files of the current library

my_func.cpp
```c++
#include <iostream> 

#include <exteral_lib/SomeFile.hpp>

#include <lib/another_lib/AnotherFile.hpp>

#include "my_func.hpp"

int Square(int32_t a) {
  return a * a;
}
```

### 10. Source code organization.

Use classes to store data associated with specific objects.
A class describes the object model, fields that store data, and methods to interact with the object.
First describe the class structure in the `.hpp` file, and then describe method implementations in the `.cpp` file.
There are no exceptions — constructors count too.
Use structs only as DTOs (Data Transfer Objects).

> Unlike Google Code Style, we name header files the same as the classes they describe.
> Files that contain class method implementations are named the same as the headers but with a `.cpp` extension.
> Files that contain only functions are named in snake_case by general purpose.

Rule: one class — one header file.

IdealGas.hpp
```c++
class IdealGas {
public:
  IdealGas(double t = 0.0, double n = 0.0, double v = 0.0, double p = 0.0);
  double GetP();
  double GetT();
  double GetV();
  double GetN();
  double CalculateR();

private:
  double t_;
  double n_; 
  double p_;
  double v_;
};
```

IdealGas.cpp
```c++
#include "IdealGas.hpp"

IdealGas::IdealGas(double t, double n, double v, double p) : t_(t), n_(n), v_(v), p_(p) {}

double IdealGas::GetP() {
  return p_;
}

// Implementation of the remaining class methods
```

When creating class instances, use smart pointers where possible (to avoid memory leaks):

```c++
#include <memory>

int main() {
  std::unique_ptr<IdealGas> ideal_gas;
  ideal_gas = std::make_unique<IdealGas>(10.0, 10.0, 10.0, 10.0);
  // Use ideal_gas
  ideal_gas = std::make_unique<IdealGas>(20.0, 20.0, 20.0, 20.0);
  
  return 0;
}
```

> Aim to use modern C++; for example, almost always prefer `std::array<char, N>` over `char[N]`.

#### On lambda functions

Use lambda functions when needed, but don’t overuse them.
For example, when you need to capture some context, or to create a very short function to pass somewhere.

### 11. Data passing organization.
   
To pass data to functions, use references and pointers to instances of the classes that describe the objects you interact with:
- Correct:
```c++
void SetIdealGas(std::unique_ptr<IdealGas> ideal_gas);
```
- INCORRECT
```c++
void SetIdealGase(double P, double V, double N, double T, double R, double x, double y, double z, double n, double m, double t);
```
