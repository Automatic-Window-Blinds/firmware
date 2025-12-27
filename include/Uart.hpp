#pragma once

#include "stm32l4xx_hal.h"

enum class UartPins
{
	NUCLEO_L476RG = GPIO_PIN_2 | GPIO_PIN_3, // TX=PA2, RX=PA3
};

struct UartConfig
{
	USART_TypeDef* instance; // e.g., USART2, USART1
	uint32_t baudRate;		 // e.g., 115200
	uint32_t wordLength;	 // e.g., UART_WORDLENGTH_8B
	uint32_t stopBits;		 // e.g., UART_STOPBITS_1
	uint32_t parity;		 // e.g., UART_PARITY_NONE
	uint32_t mode;			 // e.g., UART_MODE_TX_RX
	uint32_t hwFlowCtl;		 // e.g., UART_HWCONTROL_NONE
	uint32_t overSampling;	 // e.g., UART_OVERSAMPLING_16
};

class Uart final
{
public:
	explicit Uart(const UartConfig& huart);

	bool Init()
	{
		return HAL_UART_Init(&huart_) == HAL_OK;
	}

	/**
	 * @brief Generic write function.
	 * @param data Pointer to raw byte buffer.
	 * @param len  Number of bytes to send.
	 */
	void Write(const uint8_t* data, size_t len);

	/**
	 * @brief Generic read function.
	 */
	bool Read(uint8_t* buffer, size_t len, uint32_t timeout);

	UART_HandleTypeDef* GetHandle()
	{
		return &huart_;
	}

private:
	UART_HandleTypeDef huart_{};
};
