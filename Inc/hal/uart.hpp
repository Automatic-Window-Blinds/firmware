#pragma once

#include <stm32l4xx_hal.h>

namespace hal {

class Uart final {
public:
    /**
     * @brief Construct a UART wrapper using the provided configuration.
     * @param config Configuration for the underlying HAL UART peripheral.
     * @note This constructor only stores configuration; call Init() before use.
     */
    explicit Uart(const UART_HandleTypeDef& huart) : huart_(huart) {}

    /**
     * @brief Generic write function.
     * @param data Pointer to raw byte buffer.
     * @param len  Number of bytes to send.
     */
    void Write(const uint8_t* data, size_t len);

    /**
     * @brief Generic read function.
     * @param buffer Destination buffer to store received bytes.
     * @param len    Maximum number of bytes to read.
     * @param timeout Timeout in milliseconds to wait for data.
     * @return true if the expected number of bytes was received within timeout; false otherwise.
     */
    bool Read(uint8_t* buffer, size_t len, uint32_t timeout);

    /**
     * @brief Access the underlying HAL UART handle.
     * @return Pointer to the internal UART_HandleTypeDef.
     * @note Prefer using Write()/Read() for typical I/O operations.
     */
    UART_HandleTypeDef* GetHandle() { return &huart_; }

private:
    UART_HandleTypeDef huart_{};
};

}  // namespace hal
