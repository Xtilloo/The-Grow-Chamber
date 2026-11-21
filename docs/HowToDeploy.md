On debian 13, I ran the following commands to build, flash, and test the code on the stm32.

1. Build (Compile & Link)

Run make in your terminal (~/Repositories/The-Grow-Chamber):

- Compile (arm-none-eabi-gcc): Converts your C files (src/\*.c, including system_stm32l4xx.c) and GCC startup assembly (startup_stm32l476xx.s) into .o object files using the CMSIS headers in inc/.
- Link (STM32L476RGTx_FLASH.ld): Combines the object files into build/firmware.elf, mapping code to Flash (0x08000000) and variables/stack to SRAM (0x20000000).
- Convert (arm-none-eabi-objcopy): Strips debug symbols from .elf to produce the raw binary image build/firmware.bin.
- (Tip: Run make clean && make whenever you change headers or compiler flags).

2. Flash (Program the Microcontroller)

With your board plugged in via USB:

- Verify connection (if needed): st-info --probe confirms the ST-LINK sees your STM32L47x_L48x.
- Write to Flash: Run make flash (which executes st-flash write build/firmware.bin 0x08000000), or run st-flash --reset write build/firmware.bin 0x08000000 to automatically reset and start executing main().
