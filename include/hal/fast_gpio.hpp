#pragma once

#include "hal/gpio_impl.hpp"
#include "hal/gpio_types.hpp"

namespace hal {

template <PortBase PORT, PinMask PIN>
class FastGpio {
public:
    // Prevent instantiation
    FastGpio() = delete;

    static void ConfigureOutput(OutputType type = OutputType::PushPull, Speed speed = Speed::Low,
                                Pull pull = Pull::None) {
        detail::ConfigureGpio(PORT, PIN, detail::OutputTypeToGpioMode(type), detail::SpeedToGpioSpeed(speed),
                              detail::PullToGpioPull(pull));
    }

    static void ConfigureInput(Pull pull = Pull::None) {
        detail::ConfigureGpio(PORT, PIN, GPIO_MODE_INPUT, detail::PullToGpioPull(pull), GPIO_SPEED_FREQ_LOW);
    }

    static void ConfigureAnalog() {
        detail::ConfigureGpio(PORT, PIN, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW);
    }

    static void ConfigureAlternate(uint8_t af_num, OutputType type = OutputType::PushPull, Speed speed = Speed::Low,
                                   Pull pull = Pull::None) {
        detail::ConfigureGpio(PORT, PIN, detail::OutputTypeToAfMode(type), detail::SpeedToGpioSpeed(speed),
                              detail::PullToGpioPull(pull), af_num);
    }

    static void DeInit() { HAL_GPIO_DeInit(detail::PortPtr(PORT), PIN); }

    static void Lock() { HAL_GPIO_LockPin(detail::PortPtr(PORT), PIN); }

    [[gnu::always_inline]]
    static inline void Write(int v) {
        Write(v != 0);
    }

    [[gnu::always_inline]]
    static inline void Write(bool high) {
        high ? Set() : Clear();
    }

    [[gnu::always_inline]]
    static inline void Write(Level level) {
        Write(level == Level::High);
    }

    [[gnu::always_inline]]
    static inline void Set() {
        HAL_GPIO_WritePin(detail::PortPtr(PORT), PIN, GPIO_PIN_SET);
    }

    [[gnu::always_inline]]
    static inline void Clear() {
        HAL_GPIO_WritePin(detail::PortPtr(PORT), PIN, GPIO_PIN_RESET);
    }

    [[gnu::always_inline]]
    static inline void Toggle() {
        HAL_GPIO_TogglePin(detail::PortPtr(PORT), PIN);
    }

    [[gnu::always_inline]]
    static inline Level ReadLevel() {
        GPIO_PinState state = HAL_GPIO_ReadPin(detail::PortPtr(PORT), PIN);
        return (state == GPIO_PIN_SET) ? Level::High : Level::Low;
    }

    [[gnu::always_inline]]
    static inline int Read() {
        GPIO_PinState state = HAL_GPIO_ReadPin(detail::PortPtr(PORT), PIN);
        return static_cast<int>(state == GPIO_PIN_SET);
    }
};

}  // namespace hal
