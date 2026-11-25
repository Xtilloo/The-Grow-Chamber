#include "helpers.h"

// Default STM32L476 MSI boot clock is 4 MHz (change to 80000000U if running at 80 MHz)
uint32_t SystemCoreClock = 4000000U;

void delay_ms(uint32_t ms) {
    // Enable TRC (Trace) and DWT cycle counter if not already enabled
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    uint32_t start = DWT->CYCCNT;
    // SystemCoreClock is typically 4000000 (4 MHz MSI default) or 80000000 (80 MHz)
    uint32_t ticks = (SystemCoreClock / 1000) * ms;

    while ((DWT->CYCCNT - start) < ticks) {
        __NOP();
    }
}