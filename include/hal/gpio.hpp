#pragma once

#include "hal/gpio_impl.hpp"
#include "hal/gpio_types.hpp"

namespace hal {

/**
 * @class Gpio
 * @brief Runtime-configurable GPIO pin control.
 *
 * Provides flexible control for digital I/O pins (LEDs, buttons, relays, etc.).
 * Unlike FastGpio (template-based), this class supports dynamic runtime configuration
 * and initialization tracking. Constructors do NOT initialize hardware; call Configure*() first.
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
     * @brief Configures the pin as a digital output.
     * @param type  Output driver type (default: PushPull).
     * @param speed Switching speed (default: Low).
     * @param pull  Pull resistor configuration (default: None).
     * @details Automatically enables the GPIO port clock.
     */
    void ConfigureOutput(OutputType type = OutputType::PushPull, Speed speed = Speed::Low, Pull pull = Pull::None);

    /**
     * @brief Configures the pin as a digital input.
     * @param pull Pull resistor configuration (default: None).
     * @details Automatically enables the GPIO port clock.
     */
    void ConfigureInput(Pull pull = Pull::None);

    /**
     * @brief Configures the pin for analog input.
     * @details Disconnects digital logic from the pin, making it available for an ADC driver to read.
     * Automatically enables the GPIO port clock.
     */
    void ConfigureAnalog();

    /**
     * @brief Configures the pin for an alternate function (UART, SPI, etc.).
     * @param af_num The alternate function number (0-15, device-specific).
     * @param type   Output driver type (default: PushPull).
     * @param pull   Pull resistor configuration (default: None).
     * @param speed  Switching speed (default: Low).
     * @details Automatically enables the GPIO port clock.
     */
    void ConfigureAlternate(uint8_t af_num, OutputType type = OutputType::PushPull, Pull pull = Pull::None,
                            Speed speed = Speed::Low);

    /**
     * @brief Resets the GPIO pin to its default state.
     */
    void DeInit() { HAL_GPIO_DeInit(hal::detail::PortPtr(pin_.port), pin_.mask); }

    // -----------------------------------------------------------------------
    // Control / Access
    // -----------------------------------------------------------------------

    /**
     * @brief Locks the GPIO pin configuration to prevent accidental modification.
     */
    void Lock() { HAL_GPIO_LockPin(hal::detail::PortPtr(pin_.port), pin_.mask); }

    /**
     * @brief Writes a Level value to the pin.
     * @param level Level::High or Level::Low.
     * @pre The pin must be configured as an output.
     */
    void Write(Level level);
    /**
     * @brief Writes a boolean value to the pin.
     * @param high `true` = HIGH, `false` = LOW.
     * @pre The pin must be configured as an output.
     */
    void Write(bool high);
    /**
     * @brief Writes an integer value to the pin.
     * @param v Non-zero = HIGH, zero = LOW.
     * @pre The pin must be configured as an output.
     */
    void Write(int v);

    /**
     * @brief Reads the pin state as a Level enumeration.
     * @return Level::High or Level::Low.
     * @pre The pin must be configured as an input.
     */
    Level ReadLevel() const;
    /**
     * @brief Reads the pin state as an integer.
     * @return `1` if HIGH, `0` if LOW.
     * @pre The pin must be configured as an input.
     */
    int Read() const;
    /**
     * @brief Checks if the pin is currently HIGH.
     * @return `true` if HIGH, `false` if LOW.
     * @pre The pin must be configured as an input.
     */
    bool IsHigh() const;

    /**
     * @brief Drives the pin HIGH.
     * @pre The pin must be configured as an output.
     */
    void Set();
    /**
     * @brief Drives the pin LOW.
     * @pre The pin must be configured as an output.
     */
    void Clear();
    /**
     * @brief Toggles the pin state.
     * @pre The pin must be configured as an output.
     */
    void Toggle();

    /**
     * @brief Gets the raw Pin struct for use by other HAL drivers.
     * @return The underlying Pin structure.
     */
    Pin GetPin() const { return pin_; }

private:
    Pin pin_;

    bool is_initialized_ = false;
    bool is_output_ = false;
    bool is_input_ = false;
};

}  // namespace hal
