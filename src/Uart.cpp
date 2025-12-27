#include "Uart.hpp"

Uart::Uart(const UartConfig& huart)
{
	huart_.Instance = huart.instance;
	huart_.Init.BaudRate = huart.baudRate;
	huart_.Init.WordLength = huart.wordLength;
	huart_.Init.StopBits = huart.stopBits;
	huart_.Init.Parity = huart.parity;
	huart_.Init.Mode = huart.mode;
	huart_.Init.HwFlowCtl = huart.hwFlowCtl;
	huart_.Init.OverSampling = huart.overSampling;
}

// ----------------------------------------------------------------------------
// UART HARDWARE INITIALIZATION (MSP)
// This is called automatically by HAL_UART_Init()
// ----------------------------------------------------------------------------
#ifndef CPPCHECK
extern "C" void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if (huart->Instance == USART2)
	{
		// Enable the Peripheral Clock for USART2
		// Without this, the UART registers are frozen and writing to them causes a HardFault
		__HAL_RCC_USART2_CLK_ENABLE();

		// Enable the GPIO Clock for Port A (Where the pins are)
		__HAL_RCC_GPIOA_CLK_ENABLE();

		// Configure the Pins (PA2 = TX, PA3 = RX on Nucleo-64)
		GPIO_InitStruct.Pin = static_cast<uint32_t>(UartPins::NUCLEO_L476RG);
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; // Alternate Function Push-Pull
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART2; // This tells the MUX to connect USART2 to these pins

		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}
#endif // CPPCHECK

/**
 * @brief Generic write function.
 * @param data Pointer to raw byte buffer.
 * @param len  Number of bytes to send.
 */
void Uart::Write(const uint8_t* data, size_t len)
{
	if (len > 0)
	{
		HAL_UART_Transmit(&huart_, static_cast<const uint8_t*>(data), len, 100);
	}
}

/**
 * @brief Generic read function.
 */
bool Uart::Read(uint8_t* buffer, size_t len, uint32_t timeout)
{
	if (len == 0)
		return false;

	return HAL_UART_Receive(&huart_, buffer, len, timeout) == HAL_OK;
}
