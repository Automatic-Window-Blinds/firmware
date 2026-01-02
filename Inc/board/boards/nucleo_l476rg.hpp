#pragma once

#include <stm32l4xx_hal.h>

#include <cstdint>

#include "hal/fast_gpio.hpp"

namespace board::pins {

using ConsoleTx = hal::FastGpio<GPIOA_BASE, GPIO_PIN_2>;
using ConsoleRx = hal::FastGpio<GPIOA_BASE, GPIO_PIN_3>;

inline constexpr std::uintptr_t CONSOLE_UART_BASE = USART2_BASE;

using StatusLed = hal::FastGpio<GPIOA_BASE, GPIO_PIN_5>;

}  // namespace board::pins
