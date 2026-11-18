# Build tooling for a from-scratch STM32F446RE (Cortex-M4F) project.
# You write: linker.ld, src/*.c, src/*.s / *.S  (startup, vector table, main, ...)
#
#   make            build build/firmware.elf, .bin, .hex and print sizes
#   make flash      program the board via the Nucleo's ST-LINK and reset it
#   make debug      start an OpenOCD GDB server (leave it running)
#   make gdb        in a second terminal: connect GDB, load, halt at reset
#   make disasm     write a full disassembly to build/firmware.lst
#   make clean      delete build/

TARGET   := firmware
BUILD    := build
SRC_DIR  := src
INC_DIR  := include
LDSCRIPT := linker.ld

# ---- toolchain -------------------------------------------------------------
PREFIX  := arm-none-eabi-
CC      := $(PREFIX)gcc
OBJCOPY := $(PREFIX)objcopy
OBJDUMP := $(PREFIX)objdump
SIZE    := $(PREFIX)size
GDB     := $(PREFIX)gdb
OPENOCD := openocd

# ---- sources (picked up automatically, including subfolders of src/) -------
rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
SRCS := $(call rwildcard,$(SRC_DIR),*.c *.s *.S)
OBJS := $(patsubst %,$(BUILD)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

# ---- flags -----------------------------------------------------------------
# Cortex-M4 with single-precision FPU, hard-float ABI.
CPU := -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard

OPT ?= -Og

CFLAGS  := $(CPU) $(OPT) -g3 -std=c11 \
           -Wall -Wextra -Wshadow -Wundef -Wdouble-promotion \
           -ffunction-sections -fdata-sections -ffreestanding \
           -I$(INC_DIR) -MMD -MP
ASFLAGS := $(CPU) -g3 -I$(INC_DIR) -MMD -MP

# -nostartfiles: no crt0 -- your own startup code and Reset_Handler are used.
# nano.specs gives a small newlib (for printf later; you supply _write etc.).
# nosys.specs stubs the remaining syscalls so linking doesn't fail.
LDFLAGS := $(CPU) -T$(LDSCRIPT) -nostartfiles \
           --specs=nano.specs --specs=nosys.specs \
           -Wl,--gc-sections -Wl,-Map=$(BUILD)/$(TARGET).map -Wl,--print-memory-usage

ELF := $(BUILD)/$(TARGET).elf

# ---- rules -----------------------------------------------------------------
.PHONY: all flash debug gdb disasm size clean

all: $(ELF) $(BUILD)/$(TARGET).bin $(BUILD)/$(TARGET).hex size

$(ELF): $(OBJS) $(LDSCRIPT)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(BUILD)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.s.o: %.s
	@mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) -c $< -o $@

$(BUILD)/%.S.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) -c $< -o $@

$(BUILD)/%.bin: $(ELF)
	$(OBJCOPY) -O binary $< $@

$(BUILD)/%.hex: $(ELF)
	$(OBJCOPY) -O ihex $< $@

size: $(ELF)
	$(SIZE) $<

disasm: $(ELF)
	$(OBJDUMP) -d -S -h -t $< > $(BUILD)/$(TARGET).lst
	@echo "wrote $(BUILD)/$(TARGET).lst"

flash: $(ELF)
	$(OPENOCD) -f openocd.cfg -c "program $< verify reset exit"

debug:
	$(OPENOCD) -f openocd.cfg

gdb: $(ELF)
	$(GDB) -q -x gdb/init.gdb $<

clean:
	rm -rf $(BUILD)

-include $(DEPS)
