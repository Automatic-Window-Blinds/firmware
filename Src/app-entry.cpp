#include "adc.h"
#include "board/boards/nucleo_l476rg.hpp"
#include "hal/adc.hpp"
#include "hal/uart.hpp"
#include "usart.h"
#include "util/logger.hpp"

// Currently we are targeting the Nucleo-L476RG board because that is all I have on hand.
// Once we get the actual board (Nucleo-L432KC), we can change the pin definitions.

hal::Adc<std::uint16_t> adc1(hadc1);
std::uint16_t data[16];

extern "C" int entry(void) {
    hal::Uart console_uart(huart2);

    Logger& logger = Logger::GetInstance();
    logger.Init(&console_uart);
    logger.TestLogger();
    int count = 0;

    if (!adc1.Start(data, 16)) {
        logger.LogLine("ADC Start Failed!");
        return -1;
    }

    while (1) {
        board::pins::StatusLed::Toggle();
        logger.Logf("Toggled LED %d\r\n", count++);

        auto adc_value = adc1.Read();

        if (adc_value.has_value()) {
            logger.Logf("Val: %u\r\n", adc_value.value());
        } else {
            logger.LogLine("ADC Read Error");
        }

        auto adc_avg = adc1.ReadAverage();

        if (adc_avg.has_value()) {
            logger.Logf("Avg: %u\r\n", adc_avg.value());
        } else {
            logger.LogLine("ADC Avg Error");
        }

        HAL_Delay(500);
    }
}
