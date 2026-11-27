#include "helpers.h"

// Default STM32L476 MSI boot clock is 4 MHz; updated automatically by clock
// helper functions below.
uint32_t SystemCoreClock = 4000000U;

void delay_ms(uint32_t ms) {
    // Enable TRC (Trace) and DWT cycle counter if not already enabled
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    uint32_t ticks_per_ms = SystemCoreClock / 1000U;
    while (ms--) {
        uint32_t start = DWT->CYCCNT;
        while ((DWT->CYCCNT - start) < ticks_per_ms) {
            __NOP();
        }
    }
}

void enable_hsi16_clk(void) {
    // Switch SYSCLK from 4 MHz MSI reset default to 16 MHz HSI16
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY))
        ;

    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_HSI;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)
        ;

    SystemCoreClock = 16000000U;
}

void enable_80mhz_clk(void) {
    /* 1. Ensure Voltage Scaling Range 1 (required for 80 MHz operation) */
    RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;
    PWR->CR1 = (PWR->CR1 & ~PWR_CR1_VOS) | PWR_CR1_VOS_0; // 01 = Range 1
    while (PWR->SR2 & PWR_SR2_VOSF)
        ;

    /* 2. Enable HSI16 (16 MHz) as PLL input clock */
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY))
        ;

    /* 3. If SYSCLK is currently PLL, switch to HSI16 before reconfiguring */
    if ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_PLL) {
        RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_HSI;
        while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)
            ;
    }

    /* 4. Disable PLL and wait until stopped */
    RCC->CR &= ~RCC_CR_PLLON;
    while (RCC->CR & RCC_CR_PLLRDY)
        ;

    /* 5. Set Flash latency to 4 wait states (Range 1 @ 80 MHz) + caches */
    FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY) | FLASH_ACR_LATENCY_4WS |
                 FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN;
    while ((FLASH->ACR & FLASH_ACR_LATENCY) != FLASH_ACR_LATENCY_4WS)
        ;

    /* 6. Configure PLL: HSI16 (16 MHz) / M(1) * N(10) / R(2) = 80 MHz */
    RCC->PLLCFGR = RCC_PLLCFGR_PLLSRC_HSI | // Source = HSI16 (16 MHz)
                   (0U << 4) |              // PLLM = 1 (000 -> /1, VCO in = 16 MHz)
                   (10U << 8) |             // PLLN = 10 (VCO out = 160 MHz)
                   (0U << 25) |             // PLLR = 2 (00 -> /2, PLLCLK = 80 MHz)
                   RCC_PLLCFGR_PLLREN;      // Enable PLLCLK (R) output

    /* 7. Enable PLL and wait for lock */
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY))
        ;

    /* 8. Ensure AHB/APB1/APB2 prescalers are /1 and switch SYSCLK to PLL */
    RCC->CFGR &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2);
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
        ;

    SystemCoreClock = 80000000U;
}