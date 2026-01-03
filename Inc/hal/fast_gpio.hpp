#pragma once

#include "hal/gpio_impl.hpp"
#include "hal/gpio_types.hpp"

// TODO: Need to change reads and writes to use registers directly for speed

namespace hal {
namespace detail {
void RegisterExtiCallback(uint8_t index, void (*cb)(void));
}

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

    static constexpr PortBase kPort = PORT;
    static constexpr PinMask kPin = PIN;

    /**
     * @brief Resets the GPIO pin to its default state.
     */
    static void DeInit() { HAL_GPIO_DeInit(detail::PortPtr(PORT), PIN); }

    /**
     * @brief Locks the GPIO pin configuration to prevent accidental modification.
     */
    static void Lock() { HAL_GPIO_LockPin(detail::PortPtr(PORT), PIN); }

    /**
     * @brief Registers a function to be called when this pin triggers an interrupt.
     * @param callback Function pointer (void function(void)).
     * @warning You must Enable the EXTI Interrupt in CubeMX NVIC settings!
     */
    static void AttachInterrupt(void (*callback)(void)) {
        constexpr uint8_t pin_index = __builtin_ctz(PIN);

        // Register the callback in our central array
        detail::RegisterExtiCallback(pin_index, callback);
    }

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
