#include "util/logger.hpp"

#include <inttypes.h>
#include <unistd.h>  // For STDOUT_FILENO

#include <cstring>

Logger& Logger::GetInstance() {
    static Logger instance;  // Guaranteed to be destroyed.
                             // Instantiated on first use.
    return instance;
}

void Logger::Init(hal::Uart* uart) {
    transport_ = uart;
}

void Logger::Write(const char* data, size_t length) {
    if (transport_ == nullptr) return;

    transport_->Write(reinterpret_cast<const uint8_t*>(data), length);
}

void Logger::Log(const char* message) {
    Write(message, strlen(message));
}

void Logger::Log(const int value) {
    Logf("%" PRId16 "\r\n", value);
}

void Logger::LogLine(const char* line) {
    Logf("%s\r\n", line);
}

void Logger::LogHex(uint32_t value) {
    Logf("0x%" PRIX32 "\r\n", value);
}

template <typename DataType>
void Logger::LogBuffer_(const char* format, const DataType* data, std::size_t length) {
    Write("[ ", 2);
    for (std::size_t i = 0; i < length; i++) {
        Logf(format, data[i]);
    }
    Write("]\r\n", 3);
}

void Logger::LogBuffer(const uint8_t* data, size_t length) {
    LogBuffer_<uint8_t>("%02X ", data, length);
}

void Logger::LogBuffer(const uint16_t* data, size_t length) {
    LogBuffer_<uint16_t>("%04X ", data, length);
}

void Logger::LogBuffer(const uint32_t* data, size_t length) {
    LogBuffer_<uint32_t>("%08X ", data, length);
}

void Logger::Clear() {
    // \033[2J clears the screen, \033[H moves cursor to top-left
    Write("\033[2J\033[H", 7);
}

void Logger::Logf(const char* format, ...) {
    char buffer[128];
    va_list args;
    va_start(args, format);

    int len = vsnprintf(buffer, sizeof(buffer), format, args);

    va_end(args);

    if (len > 0) {
        Write(buffer, len);
    }
}

// This function acts as the bridge between standard C library and hardware logging
#ifndef CPPCHECK
extern "C" int _write(int file, char* ptr, int len) {
    if (file != STDOUT_FILENO && file != STDERR_FILENO) return -1;

    Logger::GetInstance().Write(ptr, len);
    return len;
}
#endif  // CPPCHECK

void Logger::TestLogger() {
    Logger& log = Logger::GetInstance();

    // 1. Test Basic Strings
    log.LogLine("=== LOGGER TEST START ===");
    log.Log("Testing Basic String: OK\r\n");

    // 2. Test Integers
    log.Log("Testing Int: ");
    log.Log(42);
    log.LogLine("");  // Newline helper

    // 3. Test Formatted (Printf style)
    log.Logf("Testing Logf: Year is %d\r\n", 2026);

    // 4. Test Hex Logger (Great for registers)
    log.Log("Testing LogHex (Expected 0xFF): ");
    log.LogHex(255);

    // 5. Test [NEW] Buffer Dump (Simulating a sensor packet)
    const uint8_t sensor_data[] = {0xAA, 0x01, 0xFF, 0x55};
    log.Log("Testing Buffer Dump: ");
    log.LogBuffer(sensor_data, 4);

    log.LogLine("=== TEST COMPLETE ===");
}
