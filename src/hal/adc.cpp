#include "hal/adc.hpp"

#include <cstring>

#include "stm32l4xx_hal_def.h"
#include "util/error_codes.hpp"

namespace hal {

template <typename SampleType>
Adc<SampleType>::Adc(ADC_HandleTypeDef& handle) : handle_(handle) {
}

template <typename SampleType>
bool Adc<SampleType>::Start(SampleType* buffer, std::size_t length) {
    if (buffer == nullptr || length == 0) {
        return false;
    }

    // Ensure any previous operation is stopped
    Stop();

    // Ensure buffer is zeroed out, this makes ReadAverage() more predictable
    std::memset(buffer, 0, length * sizeof(SampleType));

    buffer_ = buffer;
    length_ = length;

    if (HAL_ADCEx_Calibration_Start(&handle_, ADC_SINGLE_ENDED) != HAL_OK) {
        return false;
    }

    if (IsDmaMode()) {
        // DMA Mode
        return HAL_ADC_Start_DMA(&handle_, reinterpret_cast<uint32_t*>(buffer), length_) == HAL_OK;
    } else {
        // Polling Mode
        return HAL_ADC_Start(&handle_) == HAL_OK;
    }
}

template <typename SampleType>
void Adc<SampleType>::Stop() {
    if (handle_.DMA_Handle != nullptr) {
        HAL_ADC_Stop_DMA(&handle_);
    } else {
        HAL_ADC_Stop(&handle_);
    }

    // De-init state so Read() knows to fail
    buffer_ = nullptr;
    length_ = 0;
}

template <typename SampleType>
std::expected<SampleType, awb::Error> Adc<SampleType>::Read(std::size_t index) {
    if (buffer_ == nullptr || length_ == 0) {
        return std::unexpected(awb::Error::InvalidParam);  // Not Started
    }

    if (IsDmaMode()) {
        if (index >= length_) {
            return std::unexpected(awb::Error::InvalidParam);
        }

        volatile SampleType* dma_view = buffer_;
        return dma_view[index];
    } else {
        // Strictness Check: Polling logic here only supports the 'current' conversion.
        // Requesting index 1+ implies we have a history buffer, which polling doesn't provide.
        if (index > 0) {
            return std::unexpected(awb::Error::InvalidParam);
        }
        if (HAL_ADC_PollForConversion(&handle_, MAX_TIMEOUT_MS_) != HAL_OK) {
            return std::unexpected(awb::Error::Timeout);
        }

        SampleType val = static_cast<SampleType>(HAL_ADC_GetValue(&handle_));
        buffer_[0] = val;

        return val;
    }
}

template <typename SampleType>
std::expected<SampleType, awb::Error> Adc<SampleType>::ReadAverage() {
    if (buffer_ == nullptr || length_ == 0) {
        return std::unexpected(awb::Error::InvalidParam);  // Not Started
    }

    uint64_t sum = 0;

    volatile SampleType* dma_view = buffer_;
    for (std::size_t i = 0; i < length_; ++i) {
        sum += dma_view[i];
    }

    return static_cast<SampleType>(sum / length_);
}

// -----------------------------------------------------------------------------
// Explicit Instantiation
// -----------------------------------------------------------------------------
// These lines force the compiler to generate code for these specific types.
// If you try to use Adc<float>, the linker will throw an error.

template class Adc<uint8_t>;   // 8-bit DMA
template class Adc<uint16_t>;  // 16-bit DMA (Half-Word) - Most Common
template class Adc<uint32_t>;  // 32-bit DMA (Word)

}  // namespace hal
