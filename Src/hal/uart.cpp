#include "hal/uart.hpp"

namespace hal {

void Uart::Write(const uint8_t* data, size_t len) {
    if (len == 0) return;
    HAL_UART_Transmit(&huart_, data, len, 100);
}

bool Uart::Read(uint8_t* buffer, size_t len, uint32_t timeout) {
    if (len == 0) return false;

    return HAL_UART_Receive(&huart_, buffer, len, timeout) == HAL_OK;
}

}  // namespace hal
