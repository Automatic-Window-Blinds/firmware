#pragma once

#include "hal/gpio_impl.hpp"
#include "hal/gpio_types.hpp"

namespace hal {

/**
 * @brief Represents a standard General Purpose Input/Output pin.
 * Used for LEDs, Buttons, Relays, etc.
 */
class Gpio {
public:
    /**
     * @brief Construct a Gpio object.
     * @note  This does NOT initialize the hardware. Call Configure*() before use.
     */
    constexpr Gpio(PortBase port, PinMask pin) : pin_{port, pin} {}

    /**
     * @brief Construct a Gpio object from a Pin struct.
     * @note  Marked explicit to prevent implicit conversions from Pin -> Gpio.
     */
    explicit constexpr Gpio(Pin pin) : pin_{pin} {}

    // -----------------------------------------------------------------------
    // Initialization
    // -----------------------------------------------------------------------

    /**
     * @brief Configures the pin as an Output.
     * Automatically enables the GPIO Port Clock.
     */
    void ConfigureOutput(OutputType type = OutputType::PushPull, Speed speed = Speed::Low, Pull pull = Pull::None);

    /**
     * @brief Configures the pin as an Input.
     * Automatically enables the GPIO Port Clock.
     */
    void ConfigureInput(Pull pull = Pull::None);

    void ConfigureAnalog();

    void ConfigureAlternate(uint8_t af_num, OutputType type = OutputType::PushPull, Pull pull = Pull::None,
                            Speed speed = Speed::Low);

    void DeInit() { HAL_GPIO_DeInit(hal::detail::PortPtr(pin_.port), pin_.mask); }

    // -----------------------------------------------------------------------
    // Control / Access
    // -----------------------------------------------------------------------

    void Lock() { HAL_GPIO_LockPin(hal::detail::PortPtr(pin_.port), pin_.mask); }

    void Write(Level level);
    void Write(bool high);
    void Write(int v);

    Level ReadLevel() const;
    int Read() const;
    bool IsHigh() const;

    void Set();
    void Clear();
    void Toggle();

    // Access raw struct if needed by other HAL drivers
    Pin GetPin() const { return pin_; }

private:
    Pin pin_;

    bool is_initialized_ = false;
    bool is_output_ = false;
    bool is_input_ = false;
};

}  // namespace hal
