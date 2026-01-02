#include "board/boards/nucleo_l476rg.hpp"
#include "hal/uart.hpp"
#include "usart.h"
#include "util/logger.hpp"

// Currently we are targeting the Nucleo-L476RG board because that is all I have on hand.
// Once we get the actual board (Nucleo-L432KC), we can change the pin definitions.

extern "C" int entry(void) {
    hal::Uart console_uart(huart2);

    Logger& logger = Logger::GetInstance();
    logger.Init(&console_uart);
    logger.TestLogger();
    int count = 0;

    while (1) {
        board::pins::StatusLed::Toggle();
        HAL_Delay(1000);
        logger.Logf("Toggled LED %d\r\n", count++);
    }
}
