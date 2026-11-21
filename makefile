# Target name
TARGET = firmware

# Directories
BUILD_DIR = build
SRC_DIR   = src
INC_DIR   = inc

# Toolchain
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

# MCU flags (STM32L476 = Cortex-M4 with hardware FPU)
MCU = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard

# Preprocessor defines (tells stm32l4xx.h which chip header to include)
DEFS = -DSTM32L476xx

# Compiler flags
CFLAGS  = $(MCU) $(DEFS) -I$(INC_DIR) -O0 -g3 -Wall -Wextra -ffunction-sections -fdata-sections
ASFLAGS = $(MCU) -g3

# Linker flags
LDSCRIPT = STM32L476RGTX_FLASH.ld
LDFLAGS  = $(MCU) -T$(LDSCRIPT) -specs=nosys.specs -specs=nano.specs -Wl,--gc-sections -Wl,-Map=$(BUILD_DIR)/$(TARGET).map

# Source files
C_SOURCES   = $(wildcard $(SRC_DIR)/*.c)
ASM_SOURCES = startup_stm32l476xx.s

# Object files
OBJECTS  = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))

ASFLAGS = $(MCU) -x assembler-with-cpp -g3

# Default target: build .elf and .bin
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin

# Link object files into .elf
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SIZE) $@

# Convert .elf to raw .bin for st-flash
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf
	$(OBJCOPY) -O binary -S $< $@

# Compile C files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

# Assemble startup file
$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
	$(CC) -c $(ASFLAGS) $< -o $@

# Create build directory
$(BUILD_DIR):
	mkdir -p $@

# Flash via ST-LINK
flash: $(BUILD_DIR)/$(TARGET).bin
	st-flash write $< 0x08000000

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all flash clean
