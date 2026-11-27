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

    LCD_FillScreen(COLOR_WHITE);

    LCD_FillRect(0, 0, 50, 50, COLOR_BLACK);

    delay_ms(50);

    return 0;
}

void SystemInit(void) {
    // optional: enable FPU here
    // For some reason, to build the project, we need this function
}