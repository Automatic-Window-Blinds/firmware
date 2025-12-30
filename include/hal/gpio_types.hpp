#pragma once

#include <stm32l4xx_hal.h>

#include <cstdint>

namespace hal {

/**
 * @brief  Represents the base address of a GPIO Port.
 * @note   Cast this to GPIO_TypeDef* before use.
 */
using PortBase = std::uintptr_t;

/**
 * @brief  Bitmask representing a specific pin number (0-15).
 * @example GPIO_PIN_5
 */
using PinMask = std::uint16_t;

/**
 * @brief  Logical state of a GPIO pin.
 */
enum class Level : std::uint8_t {
    Low = 0,  ///< Logic Low (GND)
    High = 1  ///< Logic High (VCC)
};

/**
 * @brief  Internal pull-up/pull-down resistor configuration.
 */
enum class Pull : std::uint8_t {
    None,  ///< Floating (High Impedance)
    Up,    ///< Weak Pull-Up
    Down   ///< Weak Pull-Down
};

/**
 * @brief  Output slew rate control (Max frequencies at 3.3V, 50pF load).
 * @note   Higher speeds increase power consumption and EMI noise.
 * See STM32L432KC Datasheet DS11451 Table 61.
 */
enum class Speed : std::uint8_t {
    Low,      ///< Low Speed (up to 5 MHz) - Best for GPIO, LEDs, Buttons
    Medium,   ///< Medium Speed (up to 25 MHz) - I2C, low-speed SPI/UART
    High,     ///< High Speed (up to 50 MHz) - Fast SPI, Clock lines
    VeryHigh  ///< Very High Speed (up to 80 MHz) - High-speed master clocks
};

enum class OutputType : std::uint8_t {
    PushPull,  ///< Active drive for both High and Low states.
    OpenDrain  ///< Active Low drive, High-Z for High state (requires pull-up).
};

/**
 * @brief  Standard hardware abstraction for a single GPIO pin.
 */
struct Pin {
    PortBase port;  ///< Peripheral base address (e.g. GPIOA_BASE)
    PinMask mask;   ///< Pin bitmask (e.g. GPIO_PIN_5)
};

}  // namespace hal
