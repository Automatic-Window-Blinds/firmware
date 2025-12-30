#include "hal/gpio.hpp"

#include "stm32l4xx_hal_gpio.h"

namespace hal {

void Gpio::EnableClock() {
    detail::EnableGpioClock(pin_.port);
}

void Gpio::ConfigureOutput(OutputType type, Speed speed, Pull pull) {
    if (is_initialized_) return;

    detail::ConfigureGpio(pin_.port, pin_.mask, detail::OutputTypeToGpioMode(type), detail::PullToGpioPull(pull),
                          detail::SpeedToGpioSpeed(speed));

    is_initialized_ = true;
    is_output_ = true;
}

void Gpio::ConfigureInput(Pull pull) {
    if (is_initialized_) return;

    detail::ConfigureGpio(pin_.port, pin_.mask, GPIO_MODE_INPUT, detail::PullToGpioPull(pull), GPIO_SPEED_FREQ_LOW);

    is_initialized_ = true;
    is_input_ = true;
}

void Gpio::ConfigureAlternate(uint8_t af_num, OutputType type, Pull pull, Speed speed) {
    if (is_initialized_) return;

    detail::ConfigureGpio(pin_.port, pin_.mask, detail::OutputTypeToAfMode(type), detail::PullToGpioPull(pull),
                          detail::SpeedToGpioSpeed(speed), af_num);

    is_initialized_ = true;
}

void Gpio::Write(Level level) {
    if (!is_initialized_ || !is_output_) {
        return;
    }

    GPIO_PinState pin_state = (level == Level::High) ? GPIO_PIN_SET : GPIO_PIN_RESET;

    HAL_GPIO_WritePin(detail::PortPtr(pin_.port), pin_.mask, pin_state);
}

void Gpio::Write(bool high) {
    Write(high ? Level::High : Level::Low);
}

void Gpio::Write(int v) {
    Write(v != 0 ? Level::High : Level::Low);
}

void Gpio::Set() {
    if (!is_initialized_ || !is_output_) return;
    HAL_GPIO_WritePin(detail::PortPtr(pin_.port), pin_.mask, GPIO_PIN_SET);
}

void Gpio::Clear() {
    if (!is_initialized_ || !is_output_) return;
    HAL_GPIO_WritePin(detail::PortPtr(pin_.port), pin_.mask, GPIO_PIN_RESET);
}

void Gpio::Toggle() {
    if (!is_initialized_ || !is_output_) return;
    HAL_GPIO_TogglePin(detail::PortPtr(pin_.port), pin_.mask);
}

void ConfigureAlternatePin(const AfPinConfig& config) {
    detail::ConfigureGpio(config.pin.port, config.pin.mask, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW,
                          config.af_num);
}

}  // namespace hal
