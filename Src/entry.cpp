#include "adc.h"
#include "board/boards/nucleo_l476rg.hpp"
#include "dac.h"
#include "hal/adc.hpp"
#include "hal/uart.hpp"
#include "usart.h"
#include "util/logger.hpp"

// Currently we are targeting the Nucleo-L476RG board because that is all I have on hand.
// Once we get the actual board (Nucleo-L432KC), we can change the pin definitions.

// This test application outputs a ramp signal on DAC channel 1.
// For the nucleo-l476rg, place a jumper between A0 (PA0) and A2 (PA4) to
// connect the DAC output to the ADC input.

hal::Adc<std::uint16_t> adc1(hadc1);
std::uint16_t data[16];

std::uint16_t value_dac = 0;

void OnButtonPressed() {
    board::pins::StatusLed::Toggle();
}

extern "C" int Entry(void) {
    board::pins::UserButton::AttachInterrupt(OnButtonPressed);

    hal::Uart console_uart(huart2);

    Logger& logger = Logger::GetInstance();
    logger.Init(&console_uart);
    logger.Clear();
    logger.TestLogger();

    HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);

    if (!adc1.Start(data, 16)) {
        logger.LogLine("ADC Start Failed!");
        return -1;
    }

    while (1) {
        HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, value_dac);

        auto adc_value = adc1.Read();
        auto adc_avg = adc1.ReadAverage();

        logger.Plot("dac", value_dac);
        logger.Plot("adc", adc_value.has_value() ? adc_value.value() : 0xFFFF);
        logger.Plot("avg", adc_avg.has_value() ? adc_avg.value() : 0xFFFF);

        value_dac++;
        if (value_dac > 4095) {
            value_dac = 0;
        }

        HAL_Delay(10);
    }
}
