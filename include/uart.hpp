#pragma once

#include "stm32l4xx_hal.h"

enum class UartPins {
    NUCLEO_L476RG = GPIO_PIN_2 | GPIO_PIN_3,  // TX=PA2, RX=PA3
};

struct UartConfig {
    USART_TypeDef* instance;  // e.g., USART2, USART1
    uint32_t baud_rate;       // e.g., 115200
    uint32_t word_length;     // e.g., UART_WORDLENGTH_8B
    uint32_t stop_bits;       // e.g., UART_STOPBITS_1
    uint32_t parity;          // e.g., UART_PARITY_NONE
    uint32_t mode;            // e.g., UART_MODE_TX_RX
    uint32_t hw_flow_ctrl;    // e.g., UART_HWCONTROL_NONE
    uint32_t over_sampling;   // e.g., UART_OVERSAMPLING_16
};

class Uart final {
public:
    /**
     * @brief Construct a UART wrapper using the provided configuration.
     * @param config Configuration for the underlying HAL UART peripheral.
     * @note This constructor only stores configuration; call Init() before use.
     */
    explicit Uart(const UartConfig& config);

    /**
     * @brief Initialize the UART peripheral based on the stored configuration.
     * @return true if initialization succeeds; false otherwise.
     * @details Internally calls HAL_UART_Init(), which triggers HAL_UART_MspInit()
     *          to configure clocks and pins.
     */
    bool Init() { return HAL_UART_Init(&huart_) == HAL_OK; }

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
