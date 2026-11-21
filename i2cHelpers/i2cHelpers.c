#include "stm32l476xx.h"

// Enable GPIO and I2C clocks
// Configure SCL and SDA as open drain
// Configure timing
// Enable the peripheral and drive transfers
void i2c1_init_100khz_hsi16(void) {
    /* 1. Enable HSI16 and select it for I2C1 */
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY)) {
    }
    RCC->CCIPR &= ~RCC_CCIPR_I2C1SEL;
    RCC->CCIPR |= (2U << RCC_CCIPR_I2C1SEL_Pos); /* HSI16 */

    /* 2. Enable GPIOB and I2C1 clocks */
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;

    /* 3. Configure PB8 (SCL) and PB9 (SDA) as AF4, Open-Drain */
    // Configure GPIOB8 to be an alternate function pin (page 303 in ref manual)
    GPIOB->MODER &= ~(0x3 << (8 * 2)); // Clear the bits we are about to change (page 303 in reference manual)
    GPIOB->MODER |= (0x2 << (8 * 2));  // Set pin configuration to alternate function

    // Configure GPIOB8's alternate function as I2C (page 308 of ref manual and 89 of datasheet)
    GPIOB->AFRH &= ~(0xF << (0 * 4)); // Clear the alternate function select bits
    GPIOB->AFRH |= (0x4 << (0 * 4));  // Set the alternate function to I2C

    // Configure GPIOB8 as open drain (page 304 of reference manual)
    GPIOB->OTYPER &= ~(0x1 << (8 * 1)); // clear pin 8's values
    GPIOB->OTYPER |= (0x1 << (8 * 1));  // Set pin 8 to open drain

    // Configure GPIOB8 as pull up
    GPIOB->PUPDR &= ~(0x3 << (8 * 2)); // clear pin 8's pupdr bits
    GPIOB->PUPDR |= (0x1 << (8 * 2));  // set pin 8's pupdr bits to pull up

    // Configure GPIOB9 to be an alternate function pin (page 303 in ref manual)
    GPIOB->MODER &= ~(0x3 << (9 * 2)); // Clear the bits we are about to change (page 303 in reference manual)
    GPIOB->MODER |= (0x2 << (9 * 2));  // Set pin configuration to alternate function

    // Configure GPIOB9's alternate function as I2C (page 308 of ref manual and 89 of datasheet)
    GPIOB->AFRH &= ~(0xF << (1 * 4)); // Clear the alternate function select bits
    GPIOB->AFRH |= (0x4 << (1 * 4));  // Set the alternate function to I2C

    // Configure GPIOB9 as open drain (page 304 of reference manual)
    GPIOB->OTYPER &= ~(0x1 << (9 * 1)); // clear pin 8's values
    GPIOB->OTYPER |= (0x1 << (9 * 1));  // Set pin 8 to open drain

    // Configure GPIOB9 as pull up
    GPIOB->PUPDR &= ~(0x3 << (9 * 2)); // clear pin 8's pupdr bits
    GPIOB->PUPDR |= (0x1 << (9 * 2));  // set pin 8's pupdr bits to pull up

    /* 4. Disable I2C1 before configuring timing */
    I2C1->CR1 &= ~I2C_CR1_PE; // (Page 1316-18 in ref manual)

    /* 5. Set TIMINGR (page 1324 in ref manual) */
    I2C1->TIMINGR = (0x3U << (7 * 4)) |  // Prescaler
                    (0x4U << (5 * 4)) |  // SCLDEL
                    (0x2U << (4 * 4)) |  // SDADEL
                    (0x0FU << (2 * 4)) | // SCLH
                    (0x13U << (0 * 4));  // SCLL

    /* 6. Enable I2C1 */
    I2C1->CR1 |= I2C_CR1_PE; // (Page 1316-18 in ref manual)
}

void i2c_read(uint8_t address, uint8_t bytes) {
    // Page 1320 of reference manual:
    //
    // Write start
    // Write address + write bit (0)
    // ACK (from peripheral)
    // Write register address we want to access
    // Repeated start condition
    // Target device address + read bit (1)
    // Target device starts sending data
    // On the final expected byte, write a nack to tell peripheral to stop sending
    // Write stop

    while ((I2C1->ISR >> 15) & 1) {
        ; // Wait for I2C bus to become free
    }

    // Clear bits 0-9 and set the new 7-bit address
    I2C1->CR2 = (I2C1->CR2 & ~0xFF) | (address << 1);

    // Clear the transfer direction bit so we can write
    I2C1->CR2 = (I2C1->CR2 & ~0x0400);

    // Write how many bytes we are writing (just 1 for address)
    I2C1->CR2 = (I2C1->CR2 & ~0xFF0000) | (1 << 16);

    // Clear the AUTOEND bit
    I2C1->CR2 = (I2C1->CR2 & ~0x1000000);

    // Write start condition
    I2C1->CR2 |= (0x1 << 13);

    I2C1->CR2
}