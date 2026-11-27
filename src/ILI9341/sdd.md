# Software Design Document (SDD) for ILI9341 Driver

## Hardware Interface (4-Line SPI)

- Shares `SPI1` (`PA5` SCK, `PA6` MISO, `PA7` MOSI, `PB6` CSX) from `spiDriver`.
- Uses `PB5` as the Data/Command select pin (`D/CX`): **LOW = command**, **HIGH = data/parameter**.
- `RESET` and backlight `LED` are currently tied to 3.3 V in hardware.

## File Organization

- `ILI9341_constants.h`: All 84 documented commands from the ILI9341 datasheet (Sections 8.2, 8.3, and 8.4), defined as C99 compound literals (`((uint8_t[]){0x..})`) with inline parameter/bitfield documentation so they can be passed directly as `uint8_t *` to `LCD_WriteData()` / `LCD_WriteReadData()`.
- `ILI9341.h`: Public driver API, screen dimensions (`240 × 320`), and common RGB565 colors.
- `ILI9341.c`: Low-level command/data I/O, initialization, windowing, and drawing routines.

## Low-Level Transfer Functions

- **`LCD_WriteData(uint8_t *command, uint8_t *data, int size)`:** Unified write function supporting command-only (`data == NULL`), command + parameters, or data-only (`command == NULL`) transfers within a single `CSX` low pulse:
  - Command byte is sent with `D/CX = 0` via `SPI_Write(SPI1, command, 1, false)`.
  - Data payload is sent with `D/CX = 1` via `SPI_Write(SPI1, data, size, true)` (pipelined TX-only SPI for full bus throughput).
- **`LCD_WriteReadData(uint8_t *command, uint8_t *readBuffer, int size)`:** Sends a 1-byte read command with `D/CX = 0` (`fast = false`), switches `D/CX = 1`, and reads `size` bytes while holding `CSX` low.

## Initialization & Datasheet Timing (`LCD_Init`)

1. `SWRESET (0x01)` + `delay_ms(120)` — resets registers to factory defaults (5 ms minimum before normal commands; 120 ms before `SLPOUT` if display was already awake).
2. `SLPOUT (0x11)` + `delay_ms(120)` — starts internal oscillator and DC/DC converter.
3. `COLMOD (0x3A)` with `0x55` — selects 16-bit/pixel RGB565 format (no delay required).
4. `MADCTL (0x36)` with `0xA8` — configures memory scan direction / color order (no delay required).
5. `DISPON (0x29)` — enables panel output from GRAM (no delay required).

## Drawing & Screen Clear

- **`LCD_SetWindow(x0, y0, x1, y1)`:** Sets the active GRAM rectangle via `CASET (0x2A)` and `PASET (0x2B)` (4 bytes each, big-endian 16-bit start/end coordinates).
- **`LCD_ClearScreen(color)`:** Sets a full-screen window (`0..239`, `0..319`), issues `RAMWR (0x2C)`, fills a 1,024-byte RGB565 buffer, and streams 150 chunks (`153,600` bytes total) using `LCD_WriteData(NULL, px, 1024)` over 10 MHz pipelined TX-only SPI (~123 ms wire time).
