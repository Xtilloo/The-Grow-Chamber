#include "spiDriver.h"

void SPI_Init(SPI_TypeDef *SPIx) {
    /*
        Reference Section 22.3 of Textbook (name in SDD)
        Reference Reference Manual (RM0351) section 42.6 (SPI Registers)
    */
    /* 1. Enable SPI Clock, assuming SPIx is used*/
    if (SPIx == SPI1) {
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // Enable SPI clock

        delay_ms(50);

        RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST;  // Reset SPIx
        RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST; // Clear the reset of SPIx
    } else if (SPIx == SPI2) {
        RCC->APB2ENR |= RCC_APB1ENR1_SPI2EN; // Enable SPI clock

        delay_ms(50);

        RCC->APB2RSTR |= RCC_APB1RSTR1_SPI2RST;  // Reset SPIx
        RCC->APB2RSTR &= ~RCC_APB1RSTR1_SPI2RST; // Clear the reset of SPIx
    }

    SPIx->CR1 &= ~SPI_CR1_SPE; // Disable SPI for configuration

    /* 2. Configure full duplex and 2-line unidirectional data mode*/
    SPIx->CR1 &= ~SPI_CR1_RXONLY;   // Clear bit 10, which enables full duplex mode, page 1468 in RM0351
    SPIx->CR1 &= ~SPI_CR1_BIDIMODE; // Clear bit 15, which enables 2-line unidirectional data mode

    /* 3. Configure data frame format*/
    SPIx->CR2 &= ~SPI_CR2_DS;                               // clear data size bits (bits 11:8)
    SPIx->CR2 = SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2; // 0111 (8-bit data frames)
    SPIx->CR1 &= ~SPI_CR1_LSBFIRST;                         // Send MSB first (default for IL9341 display)

    /* 4. Configure Clock*/
    SPIx->CR1 &= ~SPI_CR1_CPHA; // Set clock phase to 0 (first clock transition is first data capture)
    SPIx->CR1 &= ~SPI_CR1_CPOL; // Set clock polarity to 0 (CK to 0 when idle)

    /* 5. Configure baud rate */
    SPIx->CR1 = SPI_CR1_BR_0; // Set baud rate to fclk / 4 (assuming fclk = 16MHz, baud = 4MHz)

    /* 6. Configure CRC */
    SPIx->CRCPR = 10;            // Set CRC polynomial
    SPIx->CR1 &= ~SPI_CR1_CRCEN; // Disable hardware CRC calculation

    /* 7. Frame format */
    SPIx->CR2 &= ~SPI_CR2_FRF; // Enable motorola mode

    /* 8. Master-Slave Configuration */
    SPIx->CR1 |= SPI_CR1_SSM;  // Software slave management enabled
    SPIx->CR1 |= SPI_CR1_MSTR; // We are the master
    SPIx->CR1 |= SPI_CR1_SSI;  // Slave selection via software
    SPIx->CR2 |= SPI_CR2_NSSP; // Enable NSS pulse management

    /*
    9. Receive buffer not empty. The RXNE flag
    Is set based on the value of FRXTH bit value.
    If FRTH is set, RXNE goes high and stays high until
    the RXFIFO level is greater or equal to 1/4 (8-bit)
    */
    SPIx->CR2 |= SPI_CR2_FRXTH;

    // Enable SPI
    SPIx->CR1 |= SPI_CR1_SPE;
}

void SPI_Write(SPI_TypeDef *SPIx, uint8_t *txBuffer, uint8_t *rxBuffer, int size) {
    int i = 0;
    for (int i = 0; i < size; i++) {
        // Wait for TXE (transmit buffer empty)
        while ((SPIx->SR & SPI_SR_TXE) != SPI_SR_TXE)
            ;
        SPIx->DR = txBuffer[i];

        // Wait until RXNE (receive buffer not empty)
        while ((SPIx->SR & SPI_SR_RXNE) != SPI_SR_RXNE)
            ;
        rxBuffer[i] = SPIx->DR;
    }

    // Wait for BSY flag to clear
    while ((SPIx->SR & SPI_SR_BSY) == SPI_SR_BSY)
        ;
}

void SPI_Read(SPI_TypeDef *SPIx, uint8_t *rxBuffer, int size) {
    int i = 0;
    for (i = 0; i < size; i++) {
        // Wait for TXE (transmit buffer empty)
        while ((SPIx->SR & SPI_SR_TXE) != SPI_SR_TXE)
            ;

        // The clock is controlled by master
        // Thus, the master must send a byte
        SPIx->DR = 0xFF; // Dummy byte

        while ((SPIx->SR & SPI_SR_RXNE) != SPI_SR_RXNE)
            ;
        rxBuffer[i] = SPIx->DR;
    }

    // Wait for BSY flag to clear
    while ((SPIx->SR & SPI_SR_BSY) == SPI_SR_BSY)
        ;
}