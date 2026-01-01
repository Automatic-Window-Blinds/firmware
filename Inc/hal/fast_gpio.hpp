#pragma once

#include "hal/gpio_impl.hpp"
#include "hal/gpio_types.hpp"

// TODO: Need to change reads and writes to use registers directly for speed

namespace hal {

/**
 * @class FastGpio
 * @brief Template-based GPIO pin control with compile-time dispatch.
 * @tparam PORT  The GPIO port (e.g., GPIOA_BASE, GPIOB_BASE).
 * @tparam PIN   The GPIO pin mask (e.g., GPIO_PIN_5).
 *
 * Uses C++ templates and static methods for zero-overhead abstraction.
 * All operations are inlined; cannot be instantiated.
 */
template <PortBase PORT, PinMask PIN>
class FastGpio {
public:
    // Prevent instantiation
    FastGpio() = delete;

    /**
     * @brief Configures the pin as a digital output.
     * @param type  Output driver type (default: PushPull).
     * @param speed Switching speed (default: Low).
     * @param pull  Pull resistor configuration (default: None).
     */
    static void ConfigureOutput(OutputType type = OutputType::PushPull, Speed speed = Speed::Low,
                                Pull pull = Pull::None) {
        detail::ConfigureGpio(PORT, PIN, detail::OutputTypeToGpioMode(type), detail::PullToGpioPull(pull),
                              detail::SpeedToGpioSpeed(speed));
    }

    /**
     * @brief Configures the pin as a digital input.
     * @param pull Pull resistor configuration (default: None).
     */
    static void ConfigureInput(Pull pull = Pull::None) {
        detail::ConfigureGpio(PORT, PIN, GPIO_MODE_INPUT, detail::PullToGpioPull(pull), GPIO_SPEED_FREQ_LOW);
    }

    /**
     * @brief Configures the pin for analog input.
     * @details Disconnects digital logic from the pin, leaving it available for an ADC driver to read.
     */
    static void ConfigureAnalog() {
        detail::ConfigureGpio(PORT, PIN, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW);
    }

    /**
     * @brief Configures the pin for an alternate function (UART, SPI, etc.).
     * @param af_num Alternate function number (0-15, device-specific).
     * @param type   Output driver type (default: PushPull).
     * @param speed  Switching speed (default: Low).
     * @param pull   Pull resistor configuration (default: None).
     */
    static void ConfigureAlternate(uint8_t af_num, OutputType type = OutputType::PushPull, Speed speed = Speed::Low,
                                   Pull pull = Pull::None) {
        detail::ConfigureGpio(PORT, PIN, detail::OutputTypeToAfMode(type), detail::PullToGpioPull(pull),
                              detail::SpeedToGpioSpeed(speed), af_num);
    }

    /**
     * @brief Resets the GPIO pin to its default state.
     */
    static void DeInit() { HAL_GPIO_DeInit(detail::PortPtr(PORT), PIN); }

    /**
     * @brief Locks the GPIO pin configuration to prevent accidental modification.
     */
    static void Lock() { HAL_GPIO_LockPin(detail::PortPtr(PORT), PIN); }

    /**
     * @brief Writes an integer value to the pin (non-zero = HIGH, zero = LOW).
     * @param v Integer value.
     */
    [[gnu::always_inline]]
    static inline void Write(int v) {
        Write(v != 0);
    }

    /**
     * @brief Writes a boolean value to the pin.
     * @param high `true` = HIGH, `false` = LOW.
     */
    [[gnu::always_inline]]
    static inline void Write(bool high) {
        high ? Set() : Clear();
    }

    /**
     * @brief Writes a Level enumeration value to the pin.
     * @param level Level::High or Level::Low.
     */
    [[gnu::always_inline]]
    static inline void Write(Level level) {
        Write(level == Level::High);
    }

    /**
     * @brief Drives the pin HIGH.
     */
    [[gnu::always_inline]]
    static inline void Set() {
        HAL_GPIO_WritePin(detail::PortPtr(PORT), PIN, GPIO_PIN_SET);
    }

    /**
     * @brief Drives the pin LOW.
     */
    [[gnu::always_inline]]
    static inline void Clear() {
        HAL_GPIO_WritePin(detail::PortPtr(PORT), PIN, GPIO_PIN_RESET);
    }

    /**
     * @brief Toggles the pin state (HIGH ↔ LOW).
     */
    [[gnu::always_inline]]
    static inline void Toggle() {
        HAL_GPIO_TogglePin(detail::PortPtr(PORT), PIN);
    }

    /**
     * @brief Reads the pin state as a Level enumeration.
     * @return Level::High or Level::Low.
     */
    [[gnu::always_inline]]
    static inline Level ReadLevel() {
        GPIO_PinState state = HAL_GPIO_ReadPin(detail::PortPtr(PORT), PIN);
        return (state == GPIO_PIN_SET) ? Level::High : Level::Low;
    }

    /**
     * @brief Reads the pin state as an integer.
     * @return `1` if HIGH, `0` if LOW.
     */
    [[gnu::always_inline]]
    static inline int Read() {
        GPIO_PinState state = HAL_GPIO_ReadPin(detail::PortPtr(PORT), PIN);
        return static_cast<int>(state == GPIO_PIN_SET);
    }
};

}  // namespace hal
