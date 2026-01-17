#include "hal/gpio.hpp"

namespace hal {

namespace detail {
void RegisterExtiCallback(uint8_t index, void (*cb)(void));
}

void Gpio::AttachInterrupt(void (*callback)(void)) {
    const uint8_t pin_index = __builtin_ctz(pin_.mask);

    // Register the callback in our central array
    detail::RegisterExtiCallback(pin_index, callback);
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
    HAL_GPIO_WritePin(detail::PortPtr(pin_.port), pin_.mask, GPIO_PIN_SET);
}

void Gpio::Clear() {
    HAL_GPIO_WritePin(detail::PortPtr(pin_.port), pin_.mask, GPIO_PIN_RESET);
}

void Gpio::Toggle() {
    HAL_GPIO_TogglePin(detail::PortPtr(pin_.port), pin_.mask);
}

bool Gpio::IsHigh() const {
    return HAL_GPIO_ReadPin(detail::PortPtr(pin_.port), pin_.mask) == GPIO_PIN_SET;
}

int Gpio::Read() const {
    return IsHigh() ? 1 : 0;
}

Level Gpio::ReadLevel() const {
    return IsHigh() ? Level::High : Level::Low;
}

}  // namespace hal
