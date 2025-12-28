#pragma once

#include "hal/gpio_types.hpp"
#include "stm32l4xx_hal.h"

namespace hal {

static inline GPIO_TypeDef* PortPtr(const PortBase& p) {
    return reinterpret_cast<GPIO_TypeDef*>(p);
}

static inline void EnableGpioClock(PortBase port_base) {
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

}  // namespace hal
