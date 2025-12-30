#pragma once

#include <cstdint>
#include <optional>

#include "stm32l4xx_hal.h"

namespace hal {

using PortBase = std::uintptr_t;  // e.g., GPIOA_BASE
using PinMask = std::uint16_t;    // e.g., GPIO_PIN_5

enum class Level : std::uint8_t { Low = 0, High = 1 };

enum class Pull : std::uint8_t { None, Up, Down };
enum class Speed : std::uint8_t { Low, Medium, High, VeryHigh };

enum class OutputType : std::uint8_t { PushPull, OpenDrain };

struct Pin {
    PortBase port;  // GPIOx_BASE
    PinMask mask;   // GPIO_PIN_x
};

}  // namespace hal
