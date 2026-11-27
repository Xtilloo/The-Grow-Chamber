#include "spiDriver.h"
#include "stdbool.h"
#include "stm32l476xx.h"

// D/C (data/command) pin: PB5. LOW = command, HIGH = data.
// RESET and LED (backlight) are tied to 3.3 V in hardware. For now.
void LCD_Init(void);

// command = U8 command for ILI9341 command
// *data = pointer to array of U8 data values
// size = size of data array, not including command arg
void LCD_WriteData(uint8_t command, uint8_t *data, int size);

void set_data(bool state);

void read_ID(uint8_t id[3]);
