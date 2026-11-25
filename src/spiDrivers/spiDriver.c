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
        RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN; // Enable SPI clock

        delay_ms(50);

        RCC->APB1RSTR1 |= RCC_APB1RSTR1_SPI2RST;  // Reset SPIx
        RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_SPI2RST; // Clear the reset of SPIx
    }

    SPIx->CR1 &= ~SPI_CR1_SPE; // Disable SPI for configuration

    /* 2. Configure full duplex and 2-line unidirectional data mode*/
    SPIx->CR1 &= ~SPI_CR1_RXONLY;   // Clear bit 10, which enables full duplex mode, page 1468 in RM0351
    SPIx->CR1 &= ~SPI_CR1_BIDIMODE; // Clear bit 15, which enables 2-line unidirectional data mode

    /* 3. Configure data frame format*/
    SPIx->CR2 &= ~SPI_CR2_DS;                                // clear data size bits (bits 11:8)
    SPIx->CR2 |= SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2; // 0111 (8-bit data frames)
    SPIx->CR1 &= ~SPI_CR1_LSBFIRST;                          // Send MSB first (default for IL9341 display)

    /* 4. Configure Clock*/
    SPIx->CR1 &= ~SPI_CR1_CPHA; // Set clock phase to 0 (first clock transition is first data capture)
    SPIx->CR1 &= ~SPI_CR1_CPOL; // Set clock polarity to 0 (CK to 0 when idle)

    /* 5. Configure baud rate */
    SPIx->CR1 |= SPI_CR1_BR_0; // Set baud rate to fclk / 4 (assuming fclk = 16MHz, baud = 4MHz)

    /* 6. Configure CRC */
    SPIx->CRCPR = 10;            // Set CRC polynomial
    SPIx->CR1 &= ~SPI_CR1_CRCEN; // Disable hardware CRC calculation

    /* 7. Frame format */
    SPIx->CR2 &= ~SPI_CR2_FRF; // Enable motorola mode

    /* 8. Master-Slave Configuration */
    SPIx->CR1 |= SPI_CR1_SSM;   // Software slave management enabled
    SPIx->CR1 |= SPI_CR1_MSTR;  // We are the master
    SPIx->CR1 |= SPI_CR1_SSI;   // Slave selection via software
    SPIx->CR2 &= ~SPI_CR2_NSSP; // Disable NSS pulse management

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

void configure_SPI_pins() {
    /* 1. Enable GPIOA and GPIOB clocks */
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

    delay_ms(50);

    /* 2. Configure PA5 (SCK), PA6 (MISO), PA7 (MOSI) as alternate function (10) */
    GPIOA->MODER &= ~(0x3 << (5 * 2));  // Clear the bits we are about to change
    GPIOA->MODER |= (0x2 << (5 * 2));   // Set PA5 to alternate function
    GPIOA->AFR[0] &= ~(0xF << (5 * 4)); // Clear the alternate function bits
    GPIOA->AFR[0] |= (0x5 << (5 * 4));  // Set alternate function to AF5

    GPIOA->MODER &= ~(0x3 << (6 * 2));  // Clear the bits we are about to change
    GPIOA->MODER |= (0x2 << (6 * 2));   // Set PA6 to alternate function
    GPIOA->AFR[0] &= ~(0xF << (6 * 4)); // Clear the alternate function bits
    GPIOA->AFR[0] |= (0x5 << (6 * 4));  // Set alternate function to AF5

    GPIOA->MODER &= ~(0x3 << (7 * 2));  // Clear the bits we are about to change
    GPIOA->MODER |= (0x2 << (7 * 2));   // Set PA7 to alternate function
    GPIOA->AFR[0] &= ~(0xF << (7 * 4)); // Clear the alternate function bits
    GPIOA->AFR[0] |= (0x5 << (7 * 4));  // Set alternate function to AF5

    /* 3. Configure PB6 (CSX) as general purpose output (01) and idle high */
    GPIOB->MODER &= ~(0x3 << (6 * 2));
    GPIOB->MODER |= (0x1 << (6 * 2));
    GPIOB->BSRR = GPIO_BSRR_BS6;
}

void SPI_CS_Low(void) {
    GPIOB->BSRR = GPIO_BSRR_BR6; // PB6, active low
}

void SPI_CS_High(void) {
    GPIOB->BSRR = GPIO_BSRR_BS6;
}

void SPI_Transfer(SPI_TypeDef *SPIx, const uint8_t *txBuf, uint8_t *rxBuf, int size) {
    for (int i = 0; i < size; i++) {
        // If we are transmitting something, set txByte to that value, otherwise, dummy byte.
        uint8_t txByte = (txBuf == NULL) ? 0xFF : txBuf[i];

        // Wait for TXE (transmit buffer empty)
        while ((SPIx->SR & SPI_SR_TXE) != SPI_SR_TXE)
            ;

        *(volatile uint8_t *)&SPIx->DR = txByte;

        // Wait for RXNE (receive buffer not empty)
        while ((SPIx->SR & SPI_SR_RXNE) != SPI_SR_RXNE)
            ;

        // Read DR to drain the RX FIFO
        uint8_t rxByte = *(volatile uint8_t *)&SPIx->DR;
        if (rxBuf != NULL) {
            rxBuf[i] = rxByte;
        }
    }
    while (SPIx->SR & SPI_SR_BSY)
        ;
}

void SPI_Write(SPI_TypeDef *SPIx, uint8_t *txBuf, int size) {
    SPI_Transfer(SPIx, txBuf, NULL, size); // Send data, ignore MISO
}

void SPI_Read(SPI_TypeDef *SPIx, uint8_t *rxBuf, int size) {
    SPI_Transfer(SPIx, NULL, rxBuf, size); // Send 0x00s, save MISO
}
