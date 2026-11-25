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

# Preprocessor defines
DEFS = -DSTM32L476xx

# Compiler & Assembler flags (-MMD -MP tracks .h file changes automatically)
CFLAGS  = $(MCU) $(DEFS) -I$(INC_DIR) -O0 -g3 -Wall -Wextra -ffunction-sections -fdata-sections -MMD -MP
ASFLAGS = $(MCU) -x assembler-with-cpp -g3

# Linker flags
LDSCRIPT = STM32L476RGTX_FLASH.ld
LDFLAGS  = $(MCU) -T$(LDSCRIPT) -specs=nosys.specs -specs=nano.specs -Wl,--gc-sections -Wl,-Map=$(BUILD_DIR)/$(TARGET).map

# Find all .c files in src/ and subdirectories
C_SOURCES   = $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*/*.c)
ASM_SOURCES = startup_stm32l476xx.s

# Tell Make which folders to search for .c files
vpath %.c $(sort $(dir $(C_SOURCES)))

# Object and dependency files
OBJECTS  = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
DEPS     = $(OBJECTS:.o=.d)

# Default target
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin

# Link object files into .elf
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SIZE) $@

# Convert .elf to raw .bin for st-flash
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf
	$(OBJCOPY) -O binary -S $< $@

# Compile C files (found via vpath)
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
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

# Include auto-generated header dependencies
-include $(DEPS)

.PHONY: all flash clean

# Find all directories containing header files
INC_DIRS  = $(INC_DIR) $(wildcard $(INC_DIR)/*) $(wildcard $(SRC_DIR)/*)
INC_FLAGS = $(addprefix -I,$(INC_DIRS))

# Update CFLAGS to use $(INC_FLAGS) instead of -I$(INC_DIR)
CFLAGS = $(MCU) $(DEFS) $(INC_FLAGS) -O0 -g3 -Wall -Wextra -ffunction-sections -fdata-sections -MMD -MP
