#include "hal/gpio.hpp"

#include "stm32l4xx_hal_gpio.h"

namespace {

static uint32_t OutputTypeToGpioMode(hal::OutputType type) {
    switch (type) {
        case hal::OutputType::PushPull:
            return GPIO_MODE_OUTPUT_PP;
        case hal::OutputType::OpenDrain:
            return GPIO_MODE_OUTPUT_OD;
        default:
            return GPIO_MODE_OUTPUT_PP;
    }
}

static uint32_t SpeedToGpioSpeed(hal::Speed speed) {
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

static uint32_t PullToGpioPull(hal::Pull pull) {
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

static void EnableGpioClock(hal::PortBase port_base) {
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

static inline GPIO_TypeDef* PortPtr(const hal::PortBase& p) {
    return reinterpret_cast<GPIO_TypeDef*>(p);
}

}  // namespace

namespace hal {
void Gpio::EnableClock() {
    EnableGpioClock(pin_.port);
}

void Gpio::ConfigureOutput(OutputType type, Speed speed, Pull pull) {
    if (is_initialized_) return;

    EnableClock();

    GPIO_InitTypeDef init{};

    init.Pin = pin_.mask;
    init.Mode = OutputTypeToGpioMode(type);
    init.Pull = PullToGpioPull(pull);
    init.Speed = SpeedToGpioSpeed(speed);

    HAL_GPIO_Init(PortPtr(pin_.port), &init);

    is_initialized_ = true;
    is_output_ = true;
}

void Gpio::ConfigureInput(Pull pull) {
    if (is_initialized_) return;

    EnableClock();

    GPIO_InitTypeDef init{};

    init.Pin = pin_.mask;
    init.Mode = GPIO_MODE_INPUT;
    init.Pull = PullToGpioPull(pull);
    init.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(PortPtr(pin_.port), &init);

    is_initialized_ = true;
    is_input_ = true;
}

void Gpio::Write(Level level) {
    if (!is_initialized_ || !is_output_) {
        return;
    }

    GPIO_PinState pin_state = (level == Level::High) ? GPIO_PIN_SET : GPIO_PIN_RESET;

    HAL_GPIO_WritePin(PortPtr(pin_.port), pin_.mask, pin_state);
}

void Gpio::Write(bool high) {
    Write(high ? Level::High : Level::Low);
}

void Gpio::Write(int v) {
    Write(v != 0 ? Level::High : Level::Low);
}

void Gpio::Set() {
    if (!is_initialized_ || !is_output_) return;
    HAL_GPIO_WritePin(PortPtr(pin_.port), pin_.mask, GPIO_PIN_SET);
}

void Gpio::Clear() {
    if (!is_initialized_ || !is_output_) return;
    HAL_GPIO_WritePin(PortPtr(pin_.port), pin_.mask, GPIO_PIN_RESET);
}

void Gpio::Toggle() {
    if (!is_initialized_ || !is_output_) return;
    HAL_GPIO_TogglePin(PortPtr(pin_.port), pin_.mask);
}

void ConfigureAlternatePin(const AfPinConfig& config) {
    EnableGpioClock(config.pin.port);

    GPIO_InitTypeDef init{};

    init.Pin = config.pin.mask;
    init.Mode = (config.type == OutputType::PushPull) ? GPIO_MODE_AF_PP : GPIO_MODE_AF_OD;
    init.Pull = (config.pull == Pull::Up) ? GPIO_PULLUP : (config.pull == Pull::Down) ? GPIO_PULLDOWN : GPIO_NOPULL;
    init.Speed = SpeedToGpioSpeed(config.speed);
    init.Alternate = config.af_num;

    HAL_GPIO_Init(PortPtr(config.pin.port), &init);
}

}  // namespace hal
