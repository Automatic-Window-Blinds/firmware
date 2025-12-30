#include "board/boards/nucleo_l476rg.hpp"
#include "hal/gpio.hpp"

#ifndef CPPCHECK

// ----------------------------------------------------------------------------
// UART HARDWARE INITIALIZATION (MSP)
// This is called automatically by HAL_UART_Init()
// ----------------------------------------------------------------------------
extern "C" void HAL_UART_MspInit(UART_HandleTypeDef* huart) {
    if (reinterpret_cast<std::uintptr_t>(huart->Instance) != board::pins::kConsoleUartBase) return;

    __HAL_RCC_USART2_CLK_ENABLE();

    hal::ConfigureAlternatePin(board::pins::kConsoleTx);
    hal::ConfigureAlternatePin(board::pins::kConsoleRx);
}

#endif  // CPPCHECK
