#include "board/boards/nucleo_l476rg.hpp"

#ifndef CPPCHECK

// ----------------------------------------------------------------------------
// UART HARDWARE INITIALIZATION (MSP)
// This is called automatically by HAL_UART_Init()
// ----------------------------------------------------------------------------
extern "C" void HAL_UART_MspInit(UART_HandleTypeDef* huart) {
    if (reinterpret_cast<std::uintptr_t>(huart->Instance) != board::pins::kConsoleUartBase) return;

    board::pins::ConsoleTx::ConfigureAlternate(GPIO_AF7_USART2, hal::OutputType::PushPull, hal::Speed::VeryHigh,
                                               hal::Pull::None);
    board::pins::ConsoleRx::ConfigureAlternate(GPIO_AF7_USART2, hal::OutputType::PushPull, hal::Speed::VeryHigh,
                                               hal::Pull::None);
    __HAL_RCC_USART2_CLK_ENABLE();
}

#endif  // CPPCHECK
