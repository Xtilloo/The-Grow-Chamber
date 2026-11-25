#include "i2cDrivers/i2cDriver.c"
#include "stm32l476xx.h"

int main(void) {
    i2c1_init_100khz_hsi16();
    return 0;
}

void SystemInit(void) {
    // optional: enable FPU here
    // For some reason, to build the project, we need this function
}