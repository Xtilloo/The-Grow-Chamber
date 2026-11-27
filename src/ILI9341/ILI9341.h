#ifndef ILI9341_H
#define ILI9341_H

#include "ILI9341_constants.h"
#include "spiDriver.h"
#include "stdbool.h"
#include "stm32l476xx.h"

#define LCD_DC_PIN 5 // PB5

#define ARRAY_LEN(arr) (sizeof(arr) / sizeof((arr)[0])) // Gives length of array

// Screen dimensions (Portrait default)
#define LCD_WIDTH 320
#define LCD_HEIGHT 240

// Common RGB565 Colors
#define COLOR_BLACK 0x0000
#define COLOR_WHITE 0xFFFF
#define COLOR_RED 0xF800
#define COLOR_GREEN 0x07E0
#define COLOR_BLUE 0x001F
#define COLOR_YELLOW 0xFFE0
#define COLOR_CYAN 0x07FF
#define COLOR_MAGENTA 0xF81F

void LCD_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                  uint16_t color);

// Helper macro to pack 8-bit R, G, B into 16-bit RGB565
#define RGB565(r, g, b)

// D/C (data/command) pin: PB5. LOW = command, HIGH = data.
// RESET and LED (backlight) are tied to 3.3 V in hardware. For now.
void LCD_Init(void);

// command = pointer to 1-byte ILI9341 command (or NULL for data-only transfer)
// *data = pointer to array of U8 data values (or NULL for command-only
// transfer) size = size of data array, not including command arg
void LCD_WriteData(uint8_t *command, uint8_t *data, int size);

void LCD_FillScreen(uint16_t color);

void LCD_DrawPixel(uint16_t x, uint16_t y, uint16_t color);

void LCD_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

void LCD_WriteReadData(uint8_t *command, uint8_t *readBuffer, int size);

void set_data(bool state);

void read_ID(uint8_t id[3]);

#endif // ILI9341_H
