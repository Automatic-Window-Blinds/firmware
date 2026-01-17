#include <stm32l4xx_hal.h>

// Array to hold callbacks for all 16 EXTI lines (0-15)
static void (*exti_callbacks[16])(void){};

namespace hal::detail {
// Helper function used by FastGpio to register callbacks
void RegisterExtiCallback(uint8_t index, void (*cb)(void)) {
    if (index < 16) {
        exti_callbacks[index] = cb;
    }
}
}  // namespace hal::detail

// Override the HAL's weak callback
extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    // GPIO_Pin is a bitmask (e.g., 0x2000 for Pin 13).
    // We need to convert it to an index (13).
    // __builtin_ctz returns the number of trailing zeros (GCC intrinsic)
    uint32_t index = __builtin_ctz(GPIO_Pin);

    if (index < 16 && exti_callbacks[index] != nullptr) {
        exti_callbacks[index]();  // Call the registered function
    }
}
