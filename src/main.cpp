#include "Logger.hpp"
#include "Uart.hpp"
#include "stm32l4xx_hal.h"

// Currently we are targeting the Nucleo-L476RG board because that is all I have on hand.
// Once we get the actual board (Nucleo-L432KC), we can change the pin definitions.

// The Nucleo-L476RG Green LED is connected to PA5
#define LED_PIN GPIO_PIN_5
#define LED_PORT GPIOA

#define LD2_Pin

// Function Prototypes
static void MX_GPIO_Init(void);

UartConfig consoleConfig = {
	.instance = USART2,
	.baudRate = 115200,
	.wordLength = UART_WORDLENGTH_8B,
	.stopBits = UART_STOPBITS_1,
	.parity = UART_PARITY_NONE,
	.mode = UART_MODE_TX_RX,
	.hwFlowCtl = UART_HWCONTROL_NONE,
	.overSampling = UART_OVERSAMPLING_16,
};

int main(void)
{
	// This must be the first function called. It sets up the Flash prefetch,
	// configures the SysTick to generate an interrupt every 1ms, and sets NVIC.
	HAL_Init();

	MX_GPIO_Init();

	Uart consoleUart(consoleConfig);
	bool consoleInit = consoleUart.Init();
	if (!consoleInit)
	{
		// UART Initialization Failed
		while (1)
		{
			// Stay here
		}
	}

	Logger& logger = Logger::GetInstance();
	logger.Init(&consoleUart);
	logger.TestLogger();
	int count = 0;

	while (1)
	{
		HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
		HAL_Delay(1000);
		logger.Logf("Toggled LED %d\r\n", count++);
	}
}

// Configure the LED Pin as Output
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();

	// Configure GPIO pin Output Level
	HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);

	// Configure Pin Attributes
	GPIO_InitStruct.Pin = LED_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Push-Pull
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
}

// ----------------------------------------------------------------------------
// INTERRUPT IMPLEMENTATIONS
// ----------------------------------------------------------------------------

extern "C" void SysTick_Handler(void)
{
	HAL_IncTick();
}

// Dummy handlers to avoid getting stuck in the Default_Handler if a fault occurs
extern "C" void NMI_Handler(void) {}
extern "C" void HardFault_Handler(void)
{
	while (1)
	{
	}
}
extern "C" void MemManage_Handler(void)
{
	while (1)
	{
	}
}
extern "C" void BusFault_Handler(void)
{
	while (1)
	{
	}
}
extern "C" void UsageFault_Handler(void)
{
	while (1)
	{
	}
}
extern "C" void SVC_Handler(void) {}
extern "C" void DebugMon_Handler(void) {}
extern "C" void PendSV_Handler(void) {}
