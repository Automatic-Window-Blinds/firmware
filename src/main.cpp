#include "hal/fast_gpio.hpp"
#include "hal/gpio.hpp"
#include "hal/gpio_types.hpp"
#include "hal/uart.hpp"
#include "stm32l4xx_hal.h"
#include "util/logger.hpp"

// Currently we are targeting the Nucleo-L476RG board because that is all I have on hand.
// Once we get the actual board (Nucleo-L432KC), we can change the pin definitions.

hal::UartConfig console_config = {
    .instance = USART2,
    .baud_rate = 115200,
    .word_length = UART_WORDLENGTH_8B,
    .stop_bits = UART_STOPBITS_1,
    .parity = UART_PARITY_NONE,
    .mode = UART_MODE_TX_RX,
    .hw_flow_ctrl = UART_HWCONTROL_NONE,
    .over_sampling = UART_OVERSAMPLING_16,
};

// The Nucleo-L476RG Green LED is connected to PA5
using StatusLed = hal::FastGpio<GPIOA_BASE, GPIO_PIN_5>;

int main(void) {
    // This must be the first function called. It sets up the Flash prefetch,
    // configures the SysTick to generate an interrupt every 1ms, and sets NVIC.
    HAL_Init();

    StatusLed::ConfigureOutput(hal::OutputType::PushPull, hal::Speed::Low);

    hal::Uart console_uart(console_config);
    bool console_init = console_uart.Init();
    if (!console_init) {
        // UART Initialization Failed
        while (1) {
            // Stay here
        }
    }

    Logger& logger = Logger::GetInstance();
    logger.Init(&console_uart);
    logger.TestLogger();
    int count = 0;

    while (1) {
        StatusLed::Toggle();
        HAL_Delay(1000);
        logger.Logf("Toggled LED %d\r\n", count++);
    }
}
