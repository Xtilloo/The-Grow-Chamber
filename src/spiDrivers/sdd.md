# Software Design Document (SDD) for SPIDriver

## Resources used

- *Embedded Systems with ARM Cortex-M Microcontrollers in Assembly Language and C* — Dr. Yifeng Zhu, Chapter 22.
- STM32L4x6 Reference Manual (RM0351), Section 42 (SPI).

## Pin Configuration (`configure_SPI_pins`)

| Pin   | Signal | Mode | Alternate Function | Output Speed (`OSPEEDR`) |
| ----- | ------ | ---- | ------------------ | ------------------------ |
| `PA5` | `SCK`  | AF   | `AF5` (`SPI1_SCK`) | High (`10`)              |
| `PA6` | `MISO` | AF   | `AF5` (`SPI1_MISO`) | High (`10`)             |
| `PA7` | `MOSI` | AF   | `AF5` (`SPI1_MOSI`) | High (`10`)             |
| `PB6` | `CSX`  | GPO  | Software CS (active low, idle high) | High (`10`) |

High output speed (`OSPEEDR = 10`) is enabled on all SPI pins so clock and data edges stay sharp at 10 MHz+.

## Peripheral Setup (`SPI_Init`)

- **Mode:** Master, full-duplex, 2-line unidirectional, Motorola frame format.
- **Frame:** 8-bit (`DS = 0111`), MSB-first, `FRXTH = 1` (sets `RXNE` at 8-bit / 1/4 FIFO threshold).
- **Clock polarity/phase:** Mode 0 (`CPOL = 0`, `CPHA = 0`).
- **Slave select:** Software-managed (`SSM = 1`, `SSI = 1`, `NSSP = 0`), controlled explicitly via `SPI_CS_Low()` / `SPI_CS_High()` so callers can hold CS low across multi-step command + data/read transactions.
- **Baud rate:** `BR[2:0] = 010` (`SPI_CR1_BR_1`, `fPCLK / 8`). With `SYSCLK = PCLK2 = 80 MHz`, `SPI1` runs at **10 MHz** (matching the ILI9341 datasheet write spec `tscycw = 100 ns`).

## Transfer Modes & Fast TX-Only Path

- **`SPI_Transfer(SPIx, txBuf, rxBuf, size)`:** Full-duplex byte-by-byte transfer. Waits for `TXE`, writes 1 byte, waits for `RXNE`, reads 1 byte to keep the RX FIFO drained, then waits for `BSY = 0`.
- **`SPI_Read(SPIx, rxBuf, size)`:** Calls `SPI_Transfer` with `txBuf = NULL` (clocks out `0xFF` dummy bytes while capturing MISO).
- **`SPI_Write(SPIx, txBuf, size, bool fast)`:**
  - **`fast = false`:** Routes through `SPI_Transfer(SPIx, txBuf, NULL, size)`. Simple and safe for short 1-byte commands or command-before-read sequences.
  - **`fast = true`:** Runs `SPI_WriteTxOnly()`, which feeds the STM32L476's 4-byte hardware TX FIFO as soon as `TXE` is set without stalling on `RXNE` after every byte. Once all bytes are queued, it waits for `BSY = 0`, drains all unread bytes from the RX FIFO (`while (SR & RXNE) DR`), and reads `SR` to clear the overrun flag (`OVR`) so subsequent reads start cleanly.
