#ifndef LOGGER_H
#define LOGGER_H

#include "stm32l4xx_hal.h"
#include <cstdarg>
#include <cstdio>

class Logger
{
public:
    /**
     * @brief The maximum length of a single log message in bytes.
     */
    constexpr static size_t LOG_BUFFER_SIZE = 128;

    /**
     * @brief The configured baud rate for the UART peripheral.
     */
    constexpr static uint32_t UART_BAUDRATE = 115200;

    /**
     * @brief  Gets the singleton instance of the Logger.
     * @return Reference to the single Logger object.
     * @note   The object is instantiated on the first call to this function.
     */
    static Logger &GetInstance();

    /**
     * @brief  Initializes the UART peripheral (USART2) and GPIO pins.
     * @note   This must be called in main() after HAL_Init() but before using any log functions.
     * Configures PA2 (TX) and PA3 (RX).
     */
    void Init();

    /**
     * @brief  Transmits raw data over UART.
     * @param  data   Pointer to the data buffer to send.
     * @param  length Number of bytes to transmit.
     * @note   This is a blocking call using HAL_UART_Transmit.
     */
    void Write(const char *data, size_t length);

    /**
     * @brief  Logs a standard C-style string.
     * @param  message Null-terminated string to transmit.
     * @note   Does not append a newline automatically.
     */
    void Log(const char *message);

    /**
     * @brief  Logs an integer value as a decimal string.
     * @param  value The integer to log.
     * @note   Automatically appends a newline (\r\n).
     */
    void Log(const int value);

    /**
     * @brief  Sends a formatted string to the UART console.
     * Behaves like the standard C printf().
     * @param  format Standard printf format string (e.g., "Val: %d")
     * @param  ...    Variable arguments matching the format
     * @note   This function uses an internal buffer. Strings longer than
     * @ref LOG_BUFFER_SIZE will be truncated to fit.
     */
    [[gnu::format(printf, 2, 3)]]
    void Logf(const char *format, ...);

    /**
     * @brief  Logs a message followed by a Carriage Return + Line Feed.
     * @param  line The message string to log.
     */
    void LogLine(const char *line);

    /**
     * @brief  Logs a 32-bit value in Hexadecimal format.
     * @param  value The value to log (e.g., 255 -> "0xFF").
     * @note   Useful for debugging registers or memory addresses.
     */
    void LogHex(uint32_t value);

    /**
     * @brief  Dumps a raw byte array as a readable hex sequence.
     * @param  data   Pointer to the byte array (e.g., sensor packet).
     * @param  length Number of bytes to read.
     * @note   Output format: "[ AA 01 FF 55 ]"
     */
    void LogBuffer(const uint8_t *data, size_t length);

    /**
     * @brief  Sends ANSI escape codes to clear the terminal screen.
     * @note   Also resets the cursor position to the top-left (0,0).
     */
    void Clear();

    /**
     * @brief  Runs a sequence of log commands to verify UART functionality.
     * @note   Call this once at startup to ensure the console is working.
     */
    void TestLogger();

    // We delete the Copy Constructor and Assignment Operator.
    // This prevents the compiler from accidentally creating a copy of your logger.
    // Example: "Logger log2 = Logger::GetInstance();" will now cause a compile error.
    Logger(const Logger &) = delete;
    void operator=(const Logger &) = delete;

private:
    /**
     * @brief Private constructor to enforce Singleton pattern.
     */
    Logger();

    UART_HandleTypeDef huart; // The raw handle is hidden here

    /**
     * @brief Internal helper to set up UART registers and baud rate.
     */
    void ConfigureUART();
};

#endif // LOGGER_H