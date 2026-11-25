#include "i2cDriver.h"
#include "spiDriver.h"
#include "stm32l476xx.h"

int main(void) {
    SPI_Init(SPI1);
    return 0;
}

void SystemInit(void) {
    // optional: enable FPU here
    // For some reason, to build the project, we need this function
}