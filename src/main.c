#include "ILI9341.h"
#include "i2cDriver.h"
#include "spiDriver.h"
#include "stm32l476xx.h"

int main(void) {
    configure_SPI_pins();
    SPI_Init(SPI1);
    LCD_Init();

    return 0;
}

void SystemInit(void) {
    // optional: enable FPU here
    // For some reason, to build the project, we need this function
}