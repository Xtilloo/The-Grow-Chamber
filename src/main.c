#include "i2cDriver.h"
#include "spiDriver.h"
#include "stm32l476xx.h"

int main(void) {
    i2c1_init_100khz_hsi16();
    SPI_Init();
    return 0;
}

void SystemInit(void) {
    // optional: enable FPU here
    // For some reason, to build the project, we need this function
}