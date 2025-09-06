# Style guide

В этом документе будут собраны все примеры правильного форматирования кода нашего проекта.

В основном следует придерживаться [Google Code Style](https://google.github.io/styleguide/cppguide.html). 
При противоречии стоит придерживаться этого документа.
Есть файл [ClangFormat](.clang-format), который можно использовать для автоматического форматирования кода.

### 1. Максимальная длина строки кода — 80 символов.

### 2. Используем пробелы, не табы. Настройте в своей среде разработки на 2 пробела.

### 3. Пишем названия переменных, используя snake_case, а для названий функций — PascalCase:

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

**НО!** Когда речь идёт о названии классов/перечислений/структур/etc, мы используем PascalCase.

**При этом!** Если мы создаем объект класса, мы используем snake_case. 
Если это приватное или защищенное поле — ставим нижнее подчеркивание в конце.
Константы пишем в PascalCase, но с префиксом `k`.

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

### 4. Используем тип фиксированной длины. Например, `int32_t` вместо `int`, `uint64_t` вместо `unsigned long`.

### 5. Мы допускаем использование auto только:
* При создании через явный вызов конструктора или ином явном указании типа
* При tuple unpacking

**В ИНОМ СЛУЧАЕ — ИЗБЕГАЕМ**

### 6. Стиль скобок: 
При создании новых классов/функций/структур/перечислений/лямбд/etc, определение и скобка на одной строке:

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

### 7. Переводы строк:

* После каждой функции, класса, структуры, перечисления, лямбды, etc.
* До и после каждого блока кода внутри функции — всего, что обрамлено фигурными скобками, например, `for` или `if`.
* Между переменными, если они не связаны между собой.
* Между `#include` и кодом.
* Перед `return` в функции.
* Перед квалификаторами доступа в классе, кроме первого.

*GodObject.hpp*
```c++
class GodObject { // Обратите внимание на порядок квалификаторов доступа
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

*GodObject.cpp*
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

### 8. Образование названий:

* ***Функции*** называем по их действию — например, если функция что-то "ищет": ```float FindMatrixDeterminant```

Иными словами, следует использовать соответствующий глагол.

Например, если мы делаем игру — допустим, змейку. У нас есть пользователь и счёт. 
В данном случае мы можем описать пользователя с помощью класса и сделать как минимум 2 публичные функции:

```c++
float Player::CalculateScore() {
  ...  
}
```

Название должно быть, насколько это возможно, **понятным**, **коротким**, и **интуитивным** для любого человека, работающего с проектом. 
Следует держать баланс между понятностью и длиной названия: не стоит давать названия более 20 символов и 4 слов. 
Аббревиатуры следует применять только если они общеприняты (например, название формата данных), и при использовании писать строчными буквами.

* Называем переменные **нормально, то есть осмысленно**:

```c++
int a = 12; // ПЛОХО!

int drops_count = 12; // Более ли менее понятно, о чём идёт речь
```

Никаких i, j, k, a, fizz, buzz, etc...

Глобальные переменные не используем. 
Если переменная используется в нескольких функциях, то она должна быть передана в качестве аргумента.


* Называем классы с помощью существительных. К примеру:

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

* Структуры называем так же, как и классы:

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

* Концепты называем прилагательными:

```c++
template <typename T>
concept Numeric = std::is_arithmetic_v<T>;
```

### 9. Делим логику (определение), записанную в `.cpp` и объявление функции записанное в `.hpp` соответственно
Например:

*main.cpp*
```c++
#include "my_func.hpp"

int main() {
  Square(10);
  return 0;
}
```
* Используем include guards!

*my_func.hpp*
```c++
#ifndef MYFUNC_HPP
#define MYFUNC_HPP

void Square(int32_t);

#endif // MYFUNC_HPP
```

* Про `include`: 
  * Все внешние библиотеки подключаем через `#include <...>`, а все наши файлы через `#include "..."`.
  * Все `include` пишем в начале файла.
  * Все `include` пишем в алфавитном порядке.
  * Все `include` пишем в следующем порядке, каждая категория разделяется пустой строкой: 
    * Сначала стандартная библиотека 
    * Затем внешние библиотеки
    * Затем другие наши библиотеки
    * Затем заголовочные файлы текущей библиотеки

*my_func.cpp*
```c++
#include <iostream> 

#include <exteral_lib/SomeFile.hpp>

#include <lib/another_lib/AnotherFile.hpp>

#include "my_func.hpp"

int Square(int32_t a) {
  return a * a;
}
```

### 10. Организация хранения исходного кода.

Для хранения данных, как-либо связанных с определёнными объектами, используются классы. 
В классах описывается модель объекта, поля, хранящие данные, и методы для взаимодействия с данным объектом. 
При этом сначала описывается структура класса в .hpp файле, а затем описывается реализация методов в .cpp файле.
Исключений нет, даже конструкторы считаются.
Структуры используем только как DTO (Data Transfer Object).

> В отличие от Google Code Style, мы называем заголовочные файлы так же, как и классы, которые они описывают.
> При этом файлы, которые содержат реализацию методов класса, называем так же, как и заголовочные файлы, но с расширением .cpp.
> А вот файлы, где лежат только функции, называем в snake_case по общему назначению.

***Правило:*** один класс — один заголовочный файл.

*IdealGas.hpp*
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

*IdealGas.cpp*
```c++
#include "IdealGas.hpp"

IdealGas::IdealGas(double t, double n, double v, double p) : t_(t), n_(n), v_(v), p_(p) {}

double IdealGas::GetP() {
  return p_;
}

// Реализация прочих методов класса
```
При создании экземпляров классов используются smart pointers, где это возможно (чтобы избежать утечек памяти):

```c++
#include <memory>

int main() {
  std::unique_ptr<IdealGas> ideal_gas;
  ideal_gas = std::make_unique<IdealGas>(10.0, 10.0, 10.0, 10.0);
  // Использование ideal_gas
  ideal_gas = std::make_unique<IdealGas>(20.0, 20.0, 20.0, 20.0);
  
  return 0;
}
```

> Стараемся использовать modern C++, например, почти всегда предпочитаем `std::array<char, N>` вместо `char[N]`.

#### О лямбда-функциях

Используем лямбда-функции, когда это необходимо, но не злоупотребляем ими.
К примеру, когда надо захватить некий контекст, или когда надо сформировать очень краткую функцию, 
которую надо куда-то передать.

### 11. Организация передачи данных.
   
Для передачи в функции используются ссылки и указатели на экземпляры классов, описывающих объекты, с которыми происходит взаимодействие:
* Правильно:
```c++
void SetIdealGas(std::unique_ptr<IdealGas> ideal_gas);
```
* НЕПРАВИЛЬНО
```c++
void SetIdealGase(double P, double V, double N, double T, double R, double x, double y, double z, double n, double m, double t);
```
