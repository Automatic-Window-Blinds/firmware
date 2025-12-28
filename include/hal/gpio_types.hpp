#pragma once

#include <cstdint>

#include "stm32l4xx_hal.h"

namespace hal {

using PortBase = std::uintptr_t;  // e.g., GPIOA_BASE
using PinMask = std::uint16_t;    // e.g., GPIO_PIN_5

enum class Level : std::uint8_t { Low = 0, High = 1 };

enum class Pull : std::uint8_t { None = GPIO_NOPULL, Up = GPIO_PULLUP, Down = GPIO_PULLDOWN };

enum class Speed : std::uint8_t {
    Low = GPIO_SPEED_FREQ_LOW,
    Medium = GPIO_SPEED_FREQ_MEDIUM,
    High = GPIO_SPEED_FREQ_HIGH,
    VeryHigh = GPIO_SPEED_FREQ_VERY_HIGH
};

enum class OutputType : std::uint8_t { PushPull = OUTPUT_PP, OpenDrain = OUTPUT_OD };

struct Pin {
    PortBase port;  // GPIOx_BASE
    PinMask mask;   // GPIO_PIN_x
};

}  // namespace hal
