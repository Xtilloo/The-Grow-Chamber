#include "spiDriver.h"

void SPI_Init(void) {
    /*
        Reference Section 22.3 of Textbook (name in SDD)
        Reference Reference Manual (RM0351) section 42.6 (SPI Registers)
    */
    /* 1. Enable SPI Clock, assuming SPI1 is used*/
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // Enable SPI clock

    delay_ms(50);

    RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST;  // Reset SPI1
    RCC->APB2RSTR *= ~RCC_APB2RSTR_SPI1RST; // Clear the reset of SPI1

    SPI1->CR1 &= ~SPI_CR1_SPE; // Disable SPI for configuration

    /* 2. Configure full duplex and 2-line unidirectional data mode*/
    SPI1->CR1 &= ~SPI_CR1_RXONLY;   // Clear bit 10, which enables full duplex mode, page 1468 in RM0351
    SPI1->CR1 &= ~SPI_CR1_BIDIMODE; // Clear bit 15, which enables 2-line unidirectional data mode

    /* 3. Configure data frame format*/
    SPI1->CR2 &= ~SPI_CR2_DS;                               // clear data size bits (bits 11:8)
    SPI1->CR2 = SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2; // 0111 (8-bit data frames)
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST;                         // Send MSB first (default for IL9341 display)

    /* 4. Configure Clock*/
    SPI1->CR1 &= ~SPI_CR1_CPHA; // Set clock phase to 0 (first clock transition is first data capture)
    SPI1->CR1 &= ~SPI_CR1_CPOL; // Set clock polarity to 0 (CK to 0 when idle)

    /* 5. Configure baud rate */
    SPI1->CR1 = SPI_CR1_BR_0; // Set baud rate to fclk / 4 (assuming fclk = 16MHz, baud = 4MHz)

    /* 6. Configure CRC */
    SPI1->CRCPR = 10;            // Set CRC polynomial
    SPI1->CR1 &= ~SPI_CR1_CRCEN; // Disable hardware CRC calculation

    /* 7. Frame format */
    SPI1->CR2 &= ~SPI_CR2_FRF; // Enable motorola mode

    /* 8. Master-Slave Configuration */
    SPI1->CR1 |= SPI_CR1_SSM;  // Software slave management enabled
    SPI1->CR1 |= SPI_CR1_MSTR; // We are the master
    SPI1->CR1 |= SPI_CR1_SSI;  // Slave selection via software
    SPI1->CR2 |= SPI_CR2_NSSP; // Enable NSS pulse management

    /*
    9. Receive buffer not empty. The RXNE flag
    Is set based on the value of FRXTH bit value.
    If FRTH is set, RXNE goes high and stays high until
    the RXFIFO level is greater or equal to 1/4 (8-bit)
    */
    SPI1->CR2 |= SPI_CR2_FRXTH;

    // Enable SPI
    SPI1->CR1 |= SPI_CR1_SPE;
}