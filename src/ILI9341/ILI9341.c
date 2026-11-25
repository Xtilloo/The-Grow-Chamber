#include "ILI9341.h"

#define LCD_DC_PIN 5 // PB5

#define LCD_CMD_SOFTWARE_RESET 0x01
#define LCD_CMD_SLEEP_OUT 0x11
#define LCD_CMD_DISPLAY_ON 0x29

// Configure display/command GPIO pin
static void LCD_DC_Init(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    delay_ms(1);

    GPIOB->MODER &= ~(0x3 << (LCD_DC_PIN * 2));
    GPIOB->MODER |= (0x1 << (LCD_DC_PIN * 2)); // general purpose output
    GPIOB->BSRR = (1U << LCD_DC_PIN);          // idle high (data)
}

void LCD_WriteCommand(uint8_t cmd) {
    GPIOB->BSRR = (1U << (LCD_DC_PIN + 16)); // D/C low = command
    SPI_Write(SPI1, &cmd, 0, 1);             // no RX data needed -> NULL
}

void LCD_WriteData(uint8_t *data, int size) {
    GPIOB->BSRR = (1U << LCD_DC_PIN); // D/C high = data
    SPI_Write(SPI1, data, 0, size);
}

void LCD_Init(void) {
    LCD_DC_Init();

    LCD_WriteCommand(LCD_CMD_SOFTWARE_RESET);
    delay_ms(120);

    LCD_WriteCommand(LCD_CMD_SLEEP_OUT);
    delay_ms(120);

    LCD_WriteCommand(LCD_CMD_DISPLAY_ON);
}
