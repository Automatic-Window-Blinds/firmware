#include "adc.h"
#include "board/boards/nucleo_l476rg.hpp"
#include "hal/uart.hpp"
#include "usart.h"
#include "util/logger.hpp"

// Currently we are targeting the Nucleo-L476RG board because that is all I have on hand.
// Once we get the actual board (Nucleo-L432KC), we can change the pin definitions.

uint32_t value_adc = 0;
uint32_t value_dac = 0;

extern "C" int entry(void) {
    hal::Uart console_uart(huart2);

    Logger& logger = Logger::GetInstance();
    logger.Init(&console_uart);
    logger.TestLogger();
    int count = 0;

    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&value_adc, 1);

    while (1) {
        board::pins::StatusLed::Toggle();
        logger.Logf("Toggled LED %d\r\n", count++);
        logger.Logf("ADC Value: %u\r\n", value_adc);
        HAL_Delay(500);
    }
}
