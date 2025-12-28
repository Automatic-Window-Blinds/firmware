#include "hal/uart.hpp"

namespace hal {

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

void Uart::Write(const uint8_t* data, size_t len) {
    if (len == 0) return;
    HAL_UART_Transmit(&huart_, data, len, 100);
}

bool Uart::Read(uint8_t* buffer, size_t len, uint32_t timeout) {
    if (len == 0) return false;

    return HAL_UART_Receive(&huart_, buffer, len, timeout) == HAL_OK;
}

}  // namespace hal
