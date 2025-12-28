#pragma once
#include <cstdint>

#include "hal/uart.hpp"

namespace board::pins {

inline constexpr hal::UartPins kConsoleUartPins{
    .tx = {{GPIOA_BASE, GPIO_PIN_2}, GPIO_AF7_USART2, hal::AfMode::AfPushPull, hal::Pull::None, hal::Speed::VeryHigh},
    .rx = {{GPIOA_BASE, GPIO_PIN_3}, GPIO_AF7_USART2, hal::AfMode::AfPushPull, hal::Pull::None, hal::Speed::VeryHigh},
};

inline constexpr std::uintptr_t kConsoleUartBase = USART2_BASE;

}  // namespace board::pins
