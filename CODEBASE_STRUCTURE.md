# Базовая структура репозитория

Ниже описана структура репозитория

```
./
├─ CMakeLists.txt                 # Корневой CMake-проект, подключает подпроекты
├─ PrintOS.cmake                  # Вспомогательные CMake-функции/макросы
├─ BUILD.md                       # Инструкции по сборке
├─ README.md                      # Обзор и быстрый старт
├─ CONTRIBUTING.md                # Правила для участников
├─ CODE_OF_CONDUCT.md             # Кодекс поведения
├─ CODING_GUIDELINES.md           # Рекомендации по стилю кода
├─ CODEBASE_STRUCTURE.md          # Этот документ
├─ LICENSE                        # Лицензия
├─ install.sh                     # Скрипт установки/подготовки окружения
├─ .clang-format                  # Настройки форматирования C/C++
├─ .clang-tidy                    # Настройки статического анализа
├─ .gitignore                     # Правила игнорирования Git
├─ .github/
│  └─ workflows/
│     └─ ci_tests.yml             # CI: сборка и прогон тестов
├─ bin/
│  ├─ CMakeLists.txt
│  └─ main.cpp                    # Точка входа исполняемого файла
├─ lib/
│  ├─ CMakeLists.txt
│  ├─ mylib/
│  │  ├─ CMakeLists.txt
│  │  ├─ MyClass.hpp              # Заголовок основной библиотеки
│  │  └─ MyClass.cpp              # Реализация основной библиотеки
│  └─ ui/
│     ├─ CMakeLists.txt
│     ├─ ui_functions.hpp         # Интерфейс UI/CLI-утилит
│     └─ ui_functions.cpp         # Реализация UI/CLI-утилит
└─ tests/
   ├─ CMakeLists.txt
   ├─ main_test.cpp               # Точка входа тестового раннера
   ├─ ProjectIntegrationTestSuite.hpp
   ├─ ProjectIntegrationTestSuite.cpp
   ├─ test_functions.hpp          # Вспомогательные функции для тестов
   ├─ test_functions.cpp
   └─ unit_tests.cpp              # Модульные тесты
```

## Назначение директорий

- bin: исполняемые цели приложения (CLI/демо), точка входа `main.cpp`.
- lib: исходники библиотек проекта (`mylib`, `ui`) и их `CMakeLists.txt`.
- tests: тестовые цели и исходники (модульные и интеграционные).
- .github/workflows: конфигурация CI для сборки и тестов.

