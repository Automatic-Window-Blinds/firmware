#pragma once
#include <cstdint>

#include "hal/gpio.hpp"

namespace board::pins {

inline constexpr hal::AfPinConfig kConsoleTx = {
    .pin = {GPIOA_BASE, GPIO_PIN_2},
    .af_num = GPIO_AF7_USART2,
    .type = hal::OutputType::PushPull,
    .pull = hal::Pull::None,
    .speed = hal::Speed::VeryHigh,
};
inline constexpr hal::AfPinConfig kConsoleRx = {
    .pin = {GPIOA_BASE, GPIO_PIN_3},
    .af_num = GPIO_AF7_USART2,
    .type = hal::OutputType::PushPull,
    .pull = hal::Pull::None,
    .speed = hal::Speed::VeryHigh,
};

inline constexpr std::uintptr_t kConsoleUartBase = USART2_BASE;

}  // namespace board::pins
