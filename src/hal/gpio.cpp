#include "hal/gpio.hpp"

namespace hal {

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

void Gpio::ConfigureAnalog() {
    detail::ConfigureGpio(pin_.port, pin_.mask, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW);
}

void Gpio::ConfigureAlternate(uint8_t af_num, OutputType type, Speed speed, Pull pull) {
    if (is_initialized_) return;

    detail::ConfigureGpio(pin_.port, pin_.mask, detail::OutputTypeToAfMode(type), detail::PullToGpioPull(pull),
                          detail::SpeedToGpioSpeed(speed), af_num);

    is_initialized_ = true;
}

void Gpio::Write(Level level) {
    Write(level == Level::High);
}

void Gpio::Write(bool high) {
    high ? Set() : Clear();
}

void Gpio::Write(int v) {
    Write(v != 0);
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

bool Gpio::IsHigh() const {
    if (!is_initialized_ || !is_input_) return false;
    return HAL_GPIO_ReadPin(detail::PortPtr(pin_.port), pin_.mask) == GPIO_PIN_SET;
}

int Gpio::Read() const {
    return IsHigh() ? 1 : 0;
}

Level Gpio::ReadLevel() const {
    return IsHigh() ? Level::High : Level::Low;
}

}  // namespace hal
