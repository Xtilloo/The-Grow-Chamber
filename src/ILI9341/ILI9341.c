#include "ILI9341.h"

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
        set_data(false);                    // We are writing a command
        SPI_Write(SPI1, command, 1, false); // Write the command
    }

    if (hasData) {
        set_data(true);                    // We are sending data
        SPI_Write(SPI1, data, size, true); // Write the data (fast TX-only)
    }

    SPI_CS_High(); // End the transaction
}

void LCD_WriteReadData(uint8_t *command, uint8_t *readBuffer, int size) {
    // Write
    set_data(false); // Set DC to command
    SPI_CS_Low();
    SPI_Write(SPI1, command, 1, false);

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
    LCD_WriteData(LCD_CMD_SWRESET, NULL, 0);
    delay_ms(120);

    LCD_WriteData(LCD_CMD_SLPOUT, NULL, 0);
    delay_ms(120);

    // Set display mode to 16-bit RGB565
    uint8_t COLMOD_PARAM[1] = {0x55};
    LCD_WriteData(LCD_CMD_COLMOD, COLMOD_PARAM, ARRAY_LEN(COLMOD_PARAM));

    // Set to portrait mode
    uint8_t MADCTL_PARAM[1] = {0xA8};
    LCD_WriteData(LCD_CMD_MADCTL, MADCTL_PARAM, 1);

    // Turn on panel
    LCD_WriteData(LCD_CMD_DISPON, NULL, 0);
}

void LCD_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    uint8_t data[4];

    // Column Address Set
    data[0] = (x0 >> 8) & 0xFF;
    data[1] = x0 & 0xFF;
    data[2] = (x1 >> 8) & 0xFF;
    data[3] = x1 & 0xFF;
    LCD_WriteData(LCD_CMD_CASET, data, ARRAY_LEN(data));

    // Page Address Set (0x2B)
    data[0] = (y0 >> 8) & 0xFF;
    data[1] = y0 & 0xFF;
    data[2] = (y1 >> 8) & 0xFF;
    data[3] = y1 & 0xFF;
    LCD_WriteData(LCD_CMD_PASET, data, ARRAY_LEN(data));
}

void LCD_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT)
        return;

    LCD_SetWindow(x, y, x, y);
    LCD_WriteData(LCD_CMD_RAMWR, NULL, 0);

    uint8_t px[2] = {(uint8_t)(color >> 8), (uint8_t)(color & 0xFF)};
    LCD_WriteData(NULL, px, ARRAY_LEN(px));
}

void LCD_ClearScreen(uint16_t color) {
    LCD_SetWindow(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1); // Set window
    LCD_WriteData(LCD_CMD_RAMWR, NULL, 0);

    // Populate the pixel buffer
    uint8_t px[1024];

    for (size_t i = 0; i < sizeof(px); i += 2) {
        px[i] = (uint8_t)(color >> 8);
        px[i + 1] = (uint8_t)(color & 0xFF);
    }

    // The screen is 240 x 320 = 76800 pixels, and each pixel takes two color
    // bytes. Thus, to clear the entire screen, we need to send a total of
    // 153600 bytes over SPI We can chunk these bytes 512 at a time (so we don't
    // fill up ram). This leaves, 153600 / 512 = 300 total chunks
    for (int i = 0; i < (153600 / 1024); i++) {
        // Send the pixel buffer over SPI
        LCD_WriteData(NULL, px, ARRAY_LEN(px));
    }
}