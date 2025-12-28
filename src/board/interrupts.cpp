#include "stm32l4xx_hal.h"

extern "C" void SysTick_Handler(void) {
    HAL_IncTick();
}

// Dummy handlers to avoid getting stuck in the Default_Handler if a fault occurs
extern "C" void NMI_Handler(void) {}
extern "C" void HardFault_Handler(void) {
    while (1) {
    }
}
extern "C" void MemManage_Handler(void) {
    while (1) {
    }
}
extern "C" void BusFault_Handler(void) {
    while (1) {
    }
}
extern "C" void UsageFault_Handler(void) {
    while (1) {
    }
}
extern "C" void SVC_Handler(void) {}
extern "C" void DebugMon_Handler(void) {}
extern "C" void PendSV_Handler(void) {}
