#pragma once

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

    // -----------------------------------------------------------------------
    // Control / Access
    // -----------------------------------------------------------------------

    void Write(Level level);
    void Write(bool high);
    void Write(int v);

    Level ReadLevel() const;
    int Read() const;
    bool IsHigh() const;

    void Set();
    void Clear();
    void Toggle();

    // Helpers
    void SetHigh() { Write(Level::High); }
    void SetLow() { Write(Level::Low); }

    // Access raw struct if needed by other HAL drivers
    Pin GetPin() const { return pin_; }

private:
    Pin pin_;

    bool is_initialized_ = false;
    bool is_output_ = false;
    bool is_input_ = false;

    void EnableClock();
};

// ---------------------------------------------------------------------------
// Alternate Function & Analog Handling
// ---------------------------------------------------------------------------

/**
 * @brief Configuration struct for Alternate Function pins (UART, I2C, SPI).
 * These don't need a class instance, just a one-time setup.
 */
struct AfPinConfig {
    Pin pin;
    uint8_t af_num;   // e.g., GPIO_AF7_USART2
    OutputType type;  // Usually PushPull for UART TX, OpenDrain for I2C
    Pull pull;        // Usually Up for I2C, None for UART
    Speed speed;
};

/**
 * @brief Initializes a pin for an Alternate Function.
 * Automatically enables the GPIO Port Clock.
 */
void ConfigureAlternatePin(const AfPinConfig& config);

}  // namespace hal
