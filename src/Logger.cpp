#include "Logger.hpp"

#include <cstring>
#include <inttypes.h>
#include <unistd.h> // For STDOUT_FILENO

Logger &Logger::GetInstance()
{
    static Logger instance; // Guaranteed to be destroyed.
                            // Instantiated on first use.
    return instance;
}

Logger::Logger()
{
    // Initialize the handle to zero to be safe
    huart = {0};
}

void Logger::Init()
{
    ConfigureUART();
}

// ----------------------------------------------------------------------------
// UART HARDWARE INITIALIZATION (MSP)
// This is called automatically by HAL_UART_Init()
// ----------------------------------------------------------------------------
extern "C" void HAL_UART_MspInit(UART_HandleTypeDef *huart)
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
        GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; // Alternate Function Push-Pull
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2; // This tells the MUX to connect USART2 to these pins

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
}

void Logger::ConfigureUART()
{
    huart.Instance = USART2;
    huart.Init.BaudRate = 115200;
    huart.Init.WordLength = UART_WORDLENGTH_8B;
    huart.Init.StopBits = UART_STOPBITS_1;
    huart.Init.Parity = UART_PARITY_NONE;
    huart.Init.Mode = UART_MODE_TX_RX;
    huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart.Init.OverSampling = UART_OVERSAMPLING_16;

    HAL_UART_Init(&huart);
}

void Logger::Write(const char *data, size_t length)
{
    if (length > 0)
    {
        HAL_UART_Transmit(&huart, (const uint8_t *)data, length, 100);
    }
}

void Logger::Log(const char *message)
{
    Write(message, strlen(message));
}

void Logger::Log(const int value)
{
    Logf("%d\r\n", value);
}

void Logger::LogLine(const char *line)
{
    Logf("%s\r\n", line);
}

void Logger::LogHex(uint32_t value)
{
    Logf("0x%" PRIX32 "\r\n", value);
}

void Logger::LogBuffer(const uint8_t *data, size_t length)
{
    Write("[ ", 2);
    for (size_t i = 0; i < length; i++)
    {
        Logf("%02X ", data[i]);
    }
    Write("]\r\n", 3);
}

void Logger::Clear()
{
    // \033[2J clears the screen, \033[H moves cursor to top-left
    Write("\033[2J\033[H", 7);
}

void Logger::Logf(const char *format, ...)
{
    char buffer[128];
    va_list args;
    va_start(args, format);

    int len = vsnprintf(buffer, sizeof(buffer), format, args);

    va_end(args);

    if (len > 0)
    {
        Write(buffer, len);
    }
}

// This function acts as the bridge between standard C library and your hardware
extern "C" int _write(int file, char *ptr, int len)
{
    // Check if the request is for specific streams (Stdout or Stderr)
    if (file == STDOUT_FILENO || file == STDERR_FILENO)
    {
        // Forward the data to your Singleton
        Logger::GetInstance().Write(ptr, len);
        return len;
    }

    // If it's not stdout/stderr, treat it as an error (-1) or ignore
    return -1;
}

void Logger::TestLogger()
{
    Logger &log = Logger::GetInstance();

    // 1. Test Basic Strings
    log.LogLine("=== LOGGER TEST START ===");
    log.Log("Testing Basic String: OK\r\n");

    // 2. Test Integers
    log.Log("Testing Int: ");
    log.Log(42);
    log.LogLine(""); // Newline helper

    // 3. Test Formatted (Printf style)
    log.Logf("Testing Logf: Year is %d\r\n", 2026);

    // 4. Test Hex Logger (Great for registers)
    log.Log("Testing LogHex (Expected 0xFF): ");
    log.LogHex(255);

    // 5. Test [NEW] Buffer Dump (Simulating a sensor packet)
    uint8_t sensorData[] = {0xAA, 0x01, 0xFF, 0x55};
    log.Log("Testing Buffer Dump: ");
    log.LogBuffer(sensorData, 4);

    log.LogLine("=== TEST COMPLETE ===");
}