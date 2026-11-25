#include "stm32l476xx.h"
void i2c1_init_100khz_hsi16(void);
int i2c_read(uint8_t device_address, uint8_t register_address, uint8_t bytes, uint8_t *data);