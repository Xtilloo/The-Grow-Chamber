#include "spiDriver.h"
#include "stm32l476xx.h"

// D/C (data/command) pin: PB5. LOW = command, HIGH = data.
// RESET and LED (backlight) are tied to 3.3 V in hardware.
void LCD_Init(void);
void LCD_WriteCommand(uint8_t cmd);
void LCD_WriteData(uint8_t *data, int size);
