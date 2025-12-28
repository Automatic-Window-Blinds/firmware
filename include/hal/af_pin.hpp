#pragma once

#include <cstdint>

#include "hal/gpio.hpp"
#include "stm32l4xx_hal.h"

namespace hal {

enum class AfMode : std::uint8_t { AfPushPull = GPIO_MODE_AF_PP, AfOpenDrain = GPIO_MODE_AF_OD };

struct AfPin {
    Pin pin;
    std::uint8_t af;  // GPIO_AFx_...
    AfMode mode;
    Pull pull;
    Speed speed;
};

inline void InitAfPin(const AfPin& p) {
    EnableGpioClock(p.pin.port);
    GPIO_InitTypeDef g{};
    g.Pin = p.pin.mask;
    g.Mode = static_cast<uint32_t>(p.mode);
    g.Pull = static_cast<uint32_t>(p.pull);
    g.Speed = static_cast<uint32_t>(p.speed);
    g.Alternate = p.af;
    HAL_GPIO_Init(PortPtr(p.pin.port), &g);
}

}  // namespace hal
