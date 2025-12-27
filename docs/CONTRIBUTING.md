# Contributing to the Firmware

This document outlines the process for setting up your environment and submitting code for PRs.

## Environment Setup

We use [PlatformIO](https://platformio.org/) to ensure everyone uses the same compiler and settings.

1. Install **Visual Studio Code**
2. Install the **PlatformIO IDE extension** for VS Code
3. Open the project directory
4. Wait for PlatformIO to finish initializing (installing the STM32 toolchain)

## Git Workflow

We follow a **Feature Branch** workflow with subsystem prefixes.

1. **Sync:** Always pull the latest `main` before starting
    ```bash
    git checkout main
    git pull origin main
    ```

2. **Branch:** Create a new branch using the format `subsystem/description`
    * **Subsystems (primary):** Work related to a specific system
      * `mcu/` — Microcontroller core functionality (e.g., `mcu/uart-driver`)
      * `motor/` — Motor control subsystem (e.g., `motor/speed-control`)
      * `sensor/` — Sensor integration (e.g., `sensor/temperature-calibration`)
      * *(Add new subsystems as your project grows)*
    * **Categories (fallback):** For work that doesn't fit a subsystem
      * `docs/` — Documentation changes (e.g., `docs/add-style-guide`)
      * `ci/` — CI/CD pipeline updates (e.g., `ci/add-tests`)
      * `build/` — Build system changes (e.g., `build/update-platformio-config`)
    
    Examples:
    ```bash
    git checkout -b mcu/uart-driver
    git checkout -b docs/add-style-guide
    ```

3. **Code & Commit:** Write your code
    * Run the **Build** button in PlatformIO frequently
    * Follow the [Style Guide](STYLE_GUIDE.md)

4. **Push & PR:** Push your branch to GitHub and open a Pull Request against `main`

## Example PR

For an example PR, see [PR #1](https://github.com/Automatic-Window-Blinds/firmware/pull/1). Feel free to follow this as a reference.

## Code Review Checklist

Before marking your PR as ready, please check:

* [ ] Code compiles without warnings
* [ ] `.clang-format` has been run (auto-formatting)
* [ ] Variable names follow `snake_case`
* [ ] Functions follow `PascalCase`
* [ ] Public functions documented with Doxygen comments (`/** ... */`)
* [ ] No trailing whitespace or unrelated formatting changes