#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include "helpers.h" // For delay
#include "stdbool.h" // For bool
#include "stddef.h"  // For NULL
#include "stm32l476xx.h"

// SPI_Init:
// Helper function to initialize SPI
void SPI_Init(SPI_TypeDef *SPIx);

// configure_SPI_pins:
// Helper function that sets PA5 (SCK), PA6 (MISO), PA7 (MOSI) as alternate functions
// Also sets PB6 (CSX) to general purpose output and idle high (only 1 chip being selected)
void configure_SPI_pins();

// SPI_Write:
// Writes size bytes from txBuffer.
// fast = true  -> pipelined TX-only write (no per-byte RXNE stall; drains RX FIFO at end)
// fast = false -> full-duplex byte-by-byte write via SPI_Transfer
void SPI_Write(SPI_TypeDef *SPIx, const uint8_t *txBuffer, int size, bool fast);
void SPI_Read(SPI_TypeDef *SPIx, uint8_t *rxBuffer, int size);

// SPI_CS_Low / SPI_CS_High:
// Assert/deassert chip select (PB6). SPI_Write and SPI_Read do NOT touch CS,
// so the caller can hold CS low across a command followed by a read.
void SPI_CS_Low(void);
void SPI_CS_High(void);

// SPI_Transfer
// Sends data if txBuf != NULL, reads data if rxBuf != NULL
void SPI_Transfer(SPI_TypeDef *SPIx, const uint8_t *txBuf, uint8_t *rxBuf, int size);

#endif // SPI_DRIVER_H
