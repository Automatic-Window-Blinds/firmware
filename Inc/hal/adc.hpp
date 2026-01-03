#pragma once

#include <stm32l4xx_hal.h>

#include <optional>

namespace hal {

/**
 * @brief ADC Driver with DMA support.
 * @tparam SampleType The data width of the ADC conversion.
 *
 * CRITICAL: This type MUST match the "Data Width" setting in CubeMX/DMA.
 * - 12-bit ADC + HalfWord DMA -> uint16_t (Recommended)
 * - 12-bit ADC + Word DMA     -> uint32_t
 * - 8-bit  ADC + Byte DMA     -> uint8_t
 */
template <typename SampleType = std::uint16_t>
class Adc {
public:
    /**
     * @brief Lightweight wrapper around a HAL ADC handle.
     * @param handle Reference to the HAL-generated ADC handle (e.g., hadc1).
     */
    explicit Adc(ADC_HandleTypeDef& handle);

    // Delete copy/move to prevent handle duplication
    Adc(const Adc&) = delete;
    Adc& operator=(const Adc&) = delete;

    /**
     * @brief Starts the ADC (and DMA if configured).
     * @param buffer Pointer to the buffer.
     * @param length Number of 'SampleType' items to record.
     * @return true if started successfully.
     */
    bool Start(SampleType* buffer, std::size_t length);

    /**
     * @brief Stops ADC conversions and DMA (if enabled).
     */
    void Stop();

    /**
     * @brief Reads a single value from the buffer (DMA) or hardware (Polling).
     * @param index Array index to read (DMA only).
     * @return Value on success, std::nullopt on error/timeout.
     */
    std::optional<SampleType> Read(std::size_t index = 0);

    /**
     * @brief Calculates the average of the entire DMA buffer.
     * @return Average value, or std::nullopt if not running.
     */
    std::optional<SampleType> ReadAverage();

    uint32_t GetMaxTimeoutMs() const { return MAX_TIMEOUT_MS_; }
    void SetMaxTimeoutMs(std::size_t timeout_ms) { MAX_TIMEOUT_MS_ = timeout_ms; }

private:
    ADC_HandleTypeDef& handle_;
    SampleType* buffer_ = nullptr;  // pointer to the user's data buffer
    std::size_t length_ = 0;        // length of the buffer
    std::size_t MAX_TIMEOUT_MS_ = 10;

    bool IsDmaMode() const { return handle_.DMA_Handle != nullptr; }
};

}  // namespace hal
