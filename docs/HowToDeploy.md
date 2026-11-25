On debian 13, I ran the following commands to build, flash, and test the code on the stm32.

## 1. Build (Compile & Link)

Run make in your terminal (~/Repositories/The-Grow-Chamber):

- Compile (arm-none-eabi-gcc): Converts your C files (src/\*.c, including system_stm32l4xx.c) and GCC startup assembly (startup_stm32l476xx.s) into .o object files using the CMSIS headers in inc/.
- Link (STM32L476RGTx_FLASH.ld): Combines the object files into build/firmware.elf, mapping code to Flash (0x08000000) and variables/stack to SRAM (0x20000000).
- Convert (arm-none-eabi-objcopy): Strips debug symbols from .elf to produce the raw binary image build/firmware.bin.
- (Tip: Run make clean && make whenever you change headers or compiler flags).

## 2. Flash (Program the Microcontroller)

With your board plugged in via USB:

- Verify connection (if needed): st-info --probe confirms the ST-LINK sees your STM32L47x_L48x.
- Write to Flash: Run make flash (which executes st-flash write build/firmware.bin 0x08000000), or run st-flash --reset write build/firmware.bin 0x08000000 to automatically reset and start executing main().

## 3. Executing the code & debugging

**Terminal 1:**

```
st-info --probe
st-util
```

**Terminal 2:**

```
gdb-multiarch build/firmware.elf
```

Once in GDB:

```
target extended-remote localhost:4242
monitor reset halt
load
```

## 4. Fast Recompile

1. Edit and save your .c file.
2. Inside gdb, run `make` directly using `!`:

```
!make clean && make
```

3. Reload the newly compiled binary onto the STM32 and restart

```
monitor reset halt
load
c
```

## 5. General GDB commands

**Breakpoints & Watchpoints**:

- `break main`: Pause at the start of `main()`
- `break src/spiDrivers/spiDriver.c:42`: Pause at line 42 of spiDriver.c
- `watch my_var`: Pause whenever `my_var` changes value
- `info breakpoints`: List all active breakpoints
- `delete 1`: Delete breakpoint 1
- `disable 1 / enable 1`: Disable or enable breakpoint 1

**Control Flow**:

- `continue`: Run until the next breakpoint or crash
- `next`: Step over current line (don't enter functions)
- `step`: Step into function call on current line
- `finish`: Run until the current function returns
- `until 50`: Run until line 50 (good for escaping for loops)
- `monitor reset halt`: Hard-reset the STM32 and pause at the reset vector

**Inspecting Code, Variables, & Registers**

- `tui enable`: Split screen showing live C source code at the top (ctrl+x A toggles off)
- `list`: Print 10 lines of C code around current line
- `backtrace`: Show the call stack
- `print var`: Print C variable in decimal
- `print/x var`: Print C variable in hex
- `print/t var`: Print C variable in binary
- `info locals`: Print all local variables in current funciton
- `info registers`: Print CPU core registers

**Inspecting STM32 hardware registers**

- Read `SPI1` registers (CR1, CR2, SR, DR, ...):

```
x/4wx 0x40013000
```

- Read `RCC` registers (AHB2ENR, AHB1ENR1, ...):

```
x/1wx 0x4002104C # RCC_AHB2ENR (GPIO clocks)
x/1wx 0x40021060 # RCC_APB2ENR (SPI 1 Clock)
```

---

# Appendix: macOS (Apple Silicon, e.g. M3)

The firmware is plain C + CMSIS, so the same build works on macOS. Only tool installation and the GDB binary name differ.

## Native macOS (recommended)

```
brew install make arm-none-eabi-gcc stlink arm-none-eabi-gdb
```

- Build: `make` (the stock macOS GNU Make 3.81 should work; if not, use `gmake` from `brew install make`).
- Flash: `make flash`, or `st-flash --reset write build/firmware.bin 0x08000000`. Verify the board with `st-info --probe`. No USB driver is needed.
- Debug: use `arm-none-eabi-gdb` instead of `gdb-multiarch`:

```
# Terminal 1
st-util

# Terminal 2
arm-none-eabi-gdb build/firmware.elf
```

Then in GDB, the commands are the same as on Debian (`target extended-remote localhost:4242`, `monitor reset halt`, `load`).

## VM on a Mac

- Use an **arm64** Debian guest (UTM, Parallels, or VMware Fusion). Avoid x86 emulation (slow).
- Install: `sudo apt install gcc-arm-none-eabi stlink-tools gdb-multiarch make`
- Building works as on Debian 13.
- Flashing/debugging from inside the VM requires **USB passthrough** of the ST-LINK. Parallels and VMware Fusion handle it well; UTM/QEMU can be flaky. Make sure the stlink udev rules are installed (the `stlink-tools` package normally does this).
- Alternative: build in the VM, then flash from macOS using the shared `build/firmware.bin` with `st-flash`, which avoids USB passthrough.
