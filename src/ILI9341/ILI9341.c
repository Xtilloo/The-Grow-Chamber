#include "ILI9341.h"

#define LCD_DC_PIN 5 // PB5

#define ARRAY_LEN(arr) (sizeof(arr) / sizeof((arr)[0])) // Gives length of array

#define LCD_CMD_SOFTWARE_RESET ((uint8_t[]){0x01})
#define LCD_CMD_SLEEP_OUT ((uint8_t[]){0x11})
#define LCD_CMD_DISPLAY_ON ((uint8_t[]){0x29})
#define LCD_CMD_READ_ID ((uint8_t[]){0x04})
#define LCD_COLMOD ((uint8_t[]){0x3A})
#define LCD_MADCTL ((uint8_t[]){0x36})

void LCD_WriteData(uint8_t *command, uint8_t *data, int size) {
    bool hasCommand = (command != NULL);
    bool hasData = (data != NULL && size > 0);

    if (!hasCommand && !hasData) {
        // Nothign to do, return early
        return;
    }

    SPI_CS_Low(); // Chip Select (start the transaction)

    // Check if we are going to write a command
    if (hasCommand) {
        set_data(false);             // We are writing a command
        SPI_Write(SPI1, command, 1); // Write the command
    }

    if (hasData) {
        set_data(true);              // We are sending data
        SPI_Write(SPI1, data, size); // Write the data
    }

    SPI_CS_High(); // End the transaction
}

void LCD_WriteReadData(uint8_t *command, uint8_t *readBuffer, int size) {
    // Write
    set_data(false); // Set DC to command
    SPI_CS_Low();
    SPI_Write(SPI1, command, 1);

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
    LCD_WriteData(LCD_CMD_SOFTWARE_RESET, NULL, 0);
    delay_ms(120);

    LCD_WriteData(LCD_CMD_SLEEP_OUT, NULL, 0);
    delay_ms(120);

    // Set display mode to 16-bit RGB565
    uint8_t COLMOD_PARAM[1] = {0x55};
    LCD_WriteData(LCD_COLMOD, COLMOD_PARAM, ARRAY_LEN(COLMOD_PARAM));

    // Set to portrait mode (0x28 for landscape)
    uint8_t MADCTL_PARAM[1] = {0x48};
    LCD_WriteData(LCD_MADCTL, MADCTL_PARAM, 1);
}