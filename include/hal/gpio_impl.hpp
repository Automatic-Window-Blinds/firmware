#pragma once

#include "hal/gpio_types.hpp"

namespace hal::detail {

static inline uint32_t OutputTypeToAfMode(const OutputType type) {
    switch (type) {
        case hal::OutputType::OpenDrain:
            return GPIO_MODE_AF_OD;
        case hal::OutputType::PushPull:
        default:
            return GPIO_MODE_AF_PP;
    }
}
static inline uint32_t OutputTypeToGpioMode(const OutputType type) {
    switch (type) {
        case hal::OutputType::OpenDrain:
            return GPIO_MODE_OUTPUT_OD;
        case hal::OutputType::PushPull:
        default:
            return GPIO_MODE_OUTPUT_PP;
    }
}

static inline uint32_t SpeedToGpioSpeed(const Speed speed) {
    switch (speed) {
        case hal::Speed::Medium:
            return GPIO_SPEED_FREQ_MEDIUM;
        case hal::Speed::High:
            return GPIO_SPEED_FREQ_HIGH;
        case hal::Speed::VeryHigh:
            return GPIO_SPEED_FREQ_VERY_HIGH;
        case hal::Speed::Low:
        default:
            return GPIO_SPEED_FREQ_LOW;
    }
}

static inline uint32_t PullToGpioPull(const Pull pull) {
    switch (pull) {
        case hal::Pull::Up:
            return GPIO_PULLUP;
        case hal::Pull::Down:
            return GPIO_PULLDOWN;
        case hal::Pull::None:
        default:
            return GPIO_NOPULL;
    }
}

static inline void EnableGpioClock(const PortBase port_base) {
    switch (port_base) {
        case GPIOA_BASE:
            __HAL_RCC_GPIOA_CLK_ENABLE();
            break;
        case GPIOB_BASE:
            __HAL_RCC_GPIOB_CLK_ENABLE();
            break;
        case GPIOC_BASE:
            __HAL_RCC_GPIOC_CLK_ENABLE();
            break;
        case GPIOD_BASE:
            __HAL_RCC_GPIOD_CLK_ENABLE();
            break;
        case GPIOE_BASE:
            __HAL_RCC_GPIOE_CLK_ENABLE();
            break;
    }
}

static inline GPIO_TypeDef* PortPtr(const PortBase& p) {
    return reinterpret_cast<GPIO_TypeDef*>(p);
}

static inline void ConfigureGpio(PortBase port, PinMask pin, uint32_t mode, uint32_t pull, uint32_t speed,
                                 uint32_t af_num = 0) {
    EnableGpioClock(port);
    GPIO_InitTypeDef init = {};
    init.Pin = pin;
    init.Mode = mode;
    init.Pull = pull;
    init.Speed = speed;
    init.Alternate = af_num;
    HAL_GPIO_Init(PortPtr(port), &init);
}

}  // namespace hal::detail
