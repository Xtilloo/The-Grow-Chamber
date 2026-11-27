# Software Design Document (SDD) for Helpers

## System Clock Management (`SystemCoreClock`)

`SystemCoreClock` tracks the active `SYSCLK` frequency (reset default: `4000000U` for 4 MHz MSI) and is updated automatically by the clock helper functions so timing utilities stay accurate.

### `enable_hsi16_clk()` — 16 MHz
- Enables the internal 16 MHz RC oscillator (`HSI16`), switches `SYSCLK` to `HSI16`, and sets `SystemCoreClock = 16000000U`.

### `enable_80mhz_clk()` — 80 MHz (PLL from HSI16)
Configures the STM32L476 to its maximum 80 MHz operating frequency (RM0351 Sections 3.3.3, 5.1.8, 6.2):
1. **Voltage Scaling:** Enables `PWR` clock and ensures Range 1 (`VOS = 01`, required for >26 MHz), waiting until `PWR_SR2_VOSF == 0`.
2. **HSI16 Source:** Enables `HSI16` (`16 MHz`) and temporarily switches `SYSCLK` to `HSI16` if the PLL was already active.
3. **PLL Off:** Disables the PLL and waits for `PLLRDY == 0` before changing multipliers.
4. **Flash Latency & Caches:** Sets **4 wait states** (`FLASH_ACR_LATENCY_4WS`, required for 64–80 MHz in Range 1) and enables Flash prefetch, instruction cache, and data cache (`PRFTEN | ICEN | DCEN`).
5. **PLL Math (`RCC->PLLCFGR`):**
   - `PLLSRC = HSI16` (`16 MHz`)
   - `PLLM = 1` (`000` -> `f_VCO_in = 16 MHz`, valid range `4–16 MHz`)
   - `PLLN = 10` (`f_VCO_out = 160 MHz`, valid range `64–344 MHz`)
   - `PLLR = 2` (`00` -> `f_PLL_R = 80 MHz`) with `PLLREN = 1`
6. **Bus Prescalers & Switch:** Sets AHB (`HPRE`), APB1 (`PPRE1`), and APB2 (`PPRE2`) prescalers to `/1` (`80 MHz`), switches `SYSCLK` to PLL, and sets `SystemCoreClock = 80000000U`.

*(Note: `I2C1` independently selects `HSI16` via `RCC->CCIPR`, so keeping `HSI16` enabled as the PLL source preserves I2C1 timing without changes.)*

## Blocking Delay (`delay_ms`)

- Uses the ARM Cortex-M4 DWT cycle counter (`DWT->CYCCNT`).
- Computes `ticks_per_ms = SystemCoreClock / 1000U` and loops 1 ms at a time (`while (ms--)`).
- Looping per millisecond keeps delays accurate across 4 MHz, 16 MHz, and 80 MHz while preventing 32-bit unsigned overflow for long delays.
