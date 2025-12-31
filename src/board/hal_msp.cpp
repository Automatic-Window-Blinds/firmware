#include "board/boards/nucleo_l476rg.hpp"

#ifndef CPPCHECK

// ----------------------------------------------------------------------------
// UART HARDWARE INITIALIZATION (MSP)
// This is called automatically by HAL_UART_Init()
// ----------------------------------------------------------------------------
extern "C" void HAL_UART_MspInit(UART_HandleTypeDef* huart) {
    if (huart->Instance != reinterpret_cast<USART_TypeDef*>(board::pins::CONSOLE_UART_BASE)) return;

    __HAL_RCC_USART2_CLK_ENABLE();

    board::pins::ConsoleTx::ConfigureAlternate(GPIO_AF7_USART2, hal::OutputType::PushPull, hal::Speed::Medium,
                                               hal::Pull::None);
    board::pins::ConsoleRx::ConfigureAlternate(GPIO_AF7_USART2, hal::OutputType::PushPull, hal::Speed::Medium,
                                               hal::Pull::None);
}

#endif  // CPPCHECK
