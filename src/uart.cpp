#include "uart.hpp"

Uart::Uart(const UartConfig& config) {
    huart_.Instance = config.instance;
    huart_.Init.BaudRate = config.baud_rate;
    huart_.Init.WordLength = config.word_length;
    huart_.Init.StopBits = config.stop_bits;
    huart_.Init.Parity = config.parity;
    huart_.Init.Mode = config.mode;
    huart_.Init.HwFlowCtl = config.hw_flow_ctrl;
    huart_.Init.OverSampling = config.over_sampling;
}

// ----------------------------------------------------------------------------
// UART HARDWARE INITIALIZATION (MSP)
// This is called automatically by HAL_UART_Init()
// ----------------------------------------------------------------------------
#ifndef CPPCHECK
extern "C" void HAL_UART_MspInit(UART_HandleTypeDef* huart) {
    GPIO_InitTypeDef gpio_init_struct = {0};

    if (huart->Instance == USART2) {
        // Enable the Peripheral Clock for USART2
        // Without this, the UART registers are frozen and writing to them causes a HardFault
        __HAL_RCC_USART2_CLK_ENABLE();

        // Enable the GPIO Clock for Port A (Where the pins are)
        __HAL_RCC_GPIOA_CLK_ENABLE();

        // Configure the Pins (PA2 = TX, PA3 = RX on Nucleo-64)
        gpio_init_struct.Pin = static_cast<uint32_t>(UartPins::NUCLEO_L476RG);
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;  // Alternate Function Push-Pull
        gpio_init_struct.Pull = GPIO_NOPULL;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        gpio_init_struct.Alternate = GPIO_AF7_USART2;  // This tells the MUX to connect USART2 to these pins

        HAL_GPIO_Init(GPIOA, &gpio_init_struct);
    }
}
#endif  // CPPCHECK

void Uart::Write(const uint8_t* data, size_t len) {
    if (len == 0) return;
    HAL_UART_Transmit(&huart_, data, len, 100);
}

bool Uart::Read(uint8_t* buffer, size_t len, uint32_t timeout) {
    if (len == 0) return false;

    return HAL_UART_Receive(&huart_, buffer, len, timeout) == HAL_OK;
}
