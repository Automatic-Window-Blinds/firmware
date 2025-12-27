# Project Style Guide

This project follows the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html), with a few specific overrides tailored to our embedded workflow and tooling.
If a rule is not listed here, assume the Google Style Guide applies.

## File Naming & Structure

* **Filenames:** Use `snake_case` (lowercase with underscores)
* **Extensions:**
  * Source files: `.cpp`
  * Header files: `.hpp`
  * These explicitly identify C++ code to IDEs, distinguishing it from legacy C code (`.c` / `.h`)
* **Examples:**
  * ✅ `logger.cpp`, `uart_driver.hpp`
  * ❌ `Logger.cpp`, `UartDriver.h`

## Formatting (Handled by Clang-Format)

We use a `.clang-format` file to automate this. Do not fight the formatter.

* **Indentation:** 4 spaces (Google defaults to 2; we use 4 for better visual separation)
* **Tabs:** Never use tabs—they render differently on every screen; spaces are consistent
* **Line Length:** 120 characters
* **Braces:** Attached style (K&R / 1TBS)
* **Example:**
    ```cpp
    // Correct
    void Init() {
        if (valid) {
            // ...
        }
    }
    ```

## Naming Conventions

| Element | Convention | Example | Notes |
|---|---|---|---|
| **Variables** | `snake_case` | `baud_rate`, `packet_len` | Matches C-style and Standard Library. |
| **Functions** | `PascalCase` | `Init()`, `ReadSensor()` | Distinguishes actions from data. |
| **Classes / Structs** | `PascalCase` | `UartConfig`, `Logger` | Standard C++ convention. |
| **Private Members** | `snake_case_` | `transport_`, `is_init_` | Must have a trailing underscore. |
| **Constants** | `kPascalCase` | `kBufferSize`, `kMaxRetries` | `ALL_CAPS` acceptable for macros. |
| **Enums** | `PascalCase` | `UartMode` | Use `enum class`. |

## Modern C++ Guidelines

* **`nullptr`:** Always use `nullptr` instead of `NULL` or `0`
* **`constexpr`:** Prefer `constexpr` variables over `#define` macros for constants
* **`enum class`:** Use scoped enums (`enum class`) to prevent name collisions
* **Casts:** Use C++ casts (`static_cast<int>(x)`) instead of C-style casts (`(int)x`)

## Comments

* **Header Files:** Public functions must be documented in the `.hpp` file
  * Use Doxygen style: `/** ... */`
  * Include `@brief`, `@param`, `@return` tags
* **Source Files:** Implementation details can be commented inline using `//`