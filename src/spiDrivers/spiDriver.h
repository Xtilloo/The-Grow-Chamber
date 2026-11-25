#include "helpers.h"
#include "stm32l476xx.h"

// SPI_Init:
// Helper function to initialize SPI
void SPI_Init(SPI_TypeDef *SPIx);

void SPI_Write(SPI_TypeDef *SPIx, uint8_t *txBuffer, uint8_t *rxBuffer, int size);