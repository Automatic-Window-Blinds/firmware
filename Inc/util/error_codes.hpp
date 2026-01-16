#pragma once

#include <cstdint>

namespace awb {

enum class Error : std::uint8_t {
    OK = 0,

    Timeout,
    Busy,
    InvalidParam,
    DmaFailure,
    AdcConversionFailed,

    NotCalibrated,
    TargetUnreachable,
    MotorOvercurrent,
    EmergencyStop,

    Unknown
};

constexpr const char* ToString(const Error e) {
    switch (e) {
        case Error::OK:                  return "OK";
        case Error::Timeout:             return "Timeout";
        case Error::Busy:                return "Busy";
        case Error::InvalidParam:        return "Invalid Param";
        case Error::DmaFailure:          return "DMA Failure";
        case Error::AdcConversionFailed: return "ADC Failed";
        case Error::NotCalibrated:       return "Not Calibrated";
        case Error::TargetUnreachable:   return "Target Unreachable";
        case Error::MotorOvercurrent:    return "Motor Overcurrent";
        case Error::EmergencyStop:       return "ESTOP";
        default:                         return "Unknown";
    }
}

}  // namespace awb
