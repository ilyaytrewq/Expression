# Expression

Библиотека и утилита для символьного дифференцирования и вычисления арифметических выражений на C++.

## Возможности

- Определение класса `Expression` для конструирования выражений из чисел, переменных и функций.
- Поддержка операций `+`, `-`, `*`, `/`, `^` и функций `sin`, `cos`, `ln`, `exp`.
- Шаблонный класс для работы с вещественными и комплексными числами.
- Символьное дифференцирование по заданной переменной.
- Вычисление выражения при подстановке значений переменных.
- Утилита `differentiator` для командной строки.
- Набор модульных тестов на Google Test.

## Структура репозитория

```text
Expression/
├── include/                   # Заголовочные файлы
│   └── Expression.hpp
├── test/                      # Тесты Google Test
│   └── test.cpp
├── differentiator.cpp         # CLI-утилита
├── CMakeLists.txt             # Основной CMake
├── CMakePresets.json          # CMake Presets (build, test, clean)
└── README.md                  # Этот файл
```

## Требования

- CMake минимум 3.19
- Компилятор с поддержкой C++20
- Интернет для загрузки GoogleTest через FetchContent

## Быстрый старт

1. **Клонировать репозиторий**

   ```bash
   git clone https://github.com/<your-user>/Expression.git
   cd Expression
   ```

2. **Создать каталог сборки и сконфигурировать с помощью CMake Presets**

   ```bash
   cmake --preset=default
   ```

   Это создаст поддиректорию `build/` и настроит сборку с генератором Unix Makefiles.

3. **Собрать проект**

   Сборка всех целей (библиотека, тесты, утилита):
   ```bash
   cmake --build --preset=build
   ```

4. **Запустить тесты**

   ```bash
   cmake --build --preset=test
   ```

## Использование утилиты `differentiator`

- **Вычисление выражения**

  ```bash
  ./build/differentiator --eval "x * y + sin(x)" x=2 y=3
  ```

- **Символьное дифференцирование**

  ```bash
  ./build/differentiator --diff "y * sin(x)" --by x
  ```
