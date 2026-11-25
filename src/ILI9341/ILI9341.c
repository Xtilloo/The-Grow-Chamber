#include "ILI9341.h"

#define LCD_DC_PIN 5 // PB5

#define LCD_CMD_SOFTWARE_RESET 0x01
#define LCD_CMD_SLEEP_OUT 0x11
#define LCD_CMD_DISPLAY_ON 0x29
#define LCD_CMD_READ_ID 0x04

void LCD_WriteCommand(uint8_t cmd) {
    set_data(false);
    SPI_CS_Low();
    SPI_Write(SPI1, &cmd, 1);
    SPI_CS_High();
}

void LCD_WriteData(uint8_t *data, int size) {
    set_data(true);
    SPI_CS_Low();
    SPI_Write(SPI1, data, size);
    SPI_CS_High();
}

void LCD_WriteReadData(uint8_t command, uint8_t *readBuffer, int size) {
    // Write
    set_data(false); // Set DC to command
    SPI_CS_Low();
    SPI_Write(SPI1, &command, 1);

    // Read
    set_data(true); // Set DC to data
    SPI_Read(SPI1, readBuffer, size);
    SPI_CS_High();
}

void set_data(bool state) {
    if (state)
        GPIOB->BSRR = (1U << LCD_DC_PIN); // D/C high = data
    else
        GPIOB->BSRR = (1U << (LCD_DC_PIN + 16)); // D/C low = command
}

void LCD_Init(void) {
    /* 1. Enable GPIOB for data/command pin*/
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    delay_ms(1);

    /* 2. Configure GPIOB5 to be general purpose output */
    GPIOB->MODER &= ~(0x3 << (LCD_DC_PIN * 2));
    GPIOB->MODER |= (0x1 << (LCD_DC_PIN * 2)); // general purpose output
    GPIOB->BSRR = (1U << LCD_DC_PIN);          // idle high (data)

    /* 3. Write commands */
    LCD_WriteCommand(LCD_CMD_SOFTWARE_RESET);
    delay_ms(120);

    LCD_WriteCommand(LCD_CMD_SLEEP_OUT);
    delay_ms(120);

    LCD_WriteCommand(LCD_CMD_DISPLAY_ON);
    delay_ms(120);
}