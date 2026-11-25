#include "spiDriver.h"
#include "stdbool.h"
#include "stm32l476xx.h"

// D/C (data/command) pin: PB5. LOW = command, HIGH = data.
// RESET and LED (backlight) are tied to 3.3 V in hardware. For now.
void LCD_Init(void);
void LCD_WriteCommand(uint8_t cmd);
void LCD_WriteData(uint8_t *data, int size);
void LCD_WriteReadData(uint8_t command, uint8_t *readBuffer, int size);
void set_data(bool state);

void read_ID(uint8_t id[3]);
