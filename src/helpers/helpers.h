#ifndef HELPERS_H
#define HELPERS_H

#include "stm32l476xx.h"

extern uint32_t SystemCoreClock;

void delay_ms(uint32_t ms);
void enable_hsi16_clk(void);
void enable_80mhz_clk(void);

#endif // HELPERS_H
