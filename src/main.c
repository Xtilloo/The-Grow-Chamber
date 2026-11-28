#include "ILI9341.h"
#include "helpers.h"
#include "i2cDriver.h"
#include "spiDriver.h"
#include "stm32l476xx.h"

volatile uint8_t lcd_id[3];
volatile int lcd_id_ok;

int main(void) {
    enable_80mhz_clk();
    configure_SPI_pins();
    SPI_Init(SPI1);
    LCD_Init();

    LCD_FillScreen(COLOR_BLACK);

    delay_ms(2000);

    char *str = "Time left until bomb explodes:";
    LCD_DrawString(20, 200, str, COLOR_CYAN, COLOR_BLACK, 1);

    for (int i = 9; i >= 0; i--) {
        LCD_DrawChar(LCD_WIDTH / 2.0, LCD_HEIGHT / 2.0, i + '0', COLOR_RED,
                     COLOR_BLACK, 4);
        delay_ms(1000);
    }

    LCD_FillScreen(COLOR_RED);

    str = "Death.";
    LCD_DrawString(LCD_WIDTH / 2.0 - LCD_WIDTH / 4.0, LCD_HEIGHT / 2.0, str,
                   COLOR_CYAN, COLOR_RED, 4);

    return 0;
}

void SystemInit(void) {
    // optional: enable FPU here
    // For some reason, to build the project, we need this function
}