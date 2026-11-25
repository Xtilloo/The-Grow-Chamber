#include "helpers.h"
#include "stm32l476xx.h"

// SPI_Init:
// Helper function to initialize SPI
void SPI_Init(SPI_TypeDef *SPIx);

// configure_SPI_pins:
// Helper function that sets PA5 (SCK), PA6 (MISO), PA7 (MOSI) as alternate functions
// Also sets PB6 (CSX) to general purpose output and idle high (only 1 chip being selected)
void configure_SPI_pins();

void SPI_Write(SPI_TypeDef *SPIx, uint8_t *txBuffer, uint8_t *rxBuffer, int size);
void SPI_Read(SPI_TypeDef *SPIx, uint8_t *rxBuffer, int size);