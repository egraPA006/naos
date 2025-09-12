# Root Makefile

# Toolchain configuration
CROSS_COMPILE ?= $(HOME)/opt/cross/bin/i686-elf-
CC := $(CROSS_COMPILE)gcc
CXX := $(CROSS_COMPILE)g++
AS := $(CROSS_COMPILE)as
LD := $(CC)

# Directory structure
BUILD_DIR := build
ISO_DIR := $(BUILD_DIR)/isodir
BOOT_DIR := $(ISO_DIR)/boot
GRUB_DIR := $(BOOT_DIR)/grub

# Output files
KERNEL_BIN := $(BUILD_DIR)/naos.bin
ISO_IMAGE := naos.iso

# Include architecture-specific config
ARCH ?= x86
include src/arch/$(ARCH)/config.mk

# Include other module configs
include src/kernel/config.mk
include src/libs/klib/config.mk

# Common compiler flags
COMMON_FLAGS := -ffreestanding -O2 -Wall -Wextra -I src/include
CFLAGS := $(COMMON_FLAGS)
CXXFLAGS := $(COMMON_FLAGS) -fno-exceptions -fno-rtti -mgeneral-regs-only
LDFLAGS := -T $(LINKER_SCRIPT) -ffreestanding -O2 -nostdlib -nostartfiles -lgcc

# Combine all object files
ALL_OBJS := $(CRTI_OBJ) $(CRTBEGIN_OBJ) \
            $(ARCH_OBJS) \
            $(KERNEL_OBJS) \
            $(KLIB_OBJS) \
            $(CRTEND_OBJ) $(CRTN_OBJ)

.PHONY: all build run clean lsp help _ensure_build_dir

all: build

help:
	@echo "NAOS Makefile Help"
	@echo "================="
	@echo "Available targets:"
	@echo "  all       - Build everything (default target)"
	@echo "  build     - Build the OS image"
	@echo "  run       - Run the OS in QEMU"
	@echo "  clean     - Remove all build artifacts"
	@echo "  help      - Show this help message"

build: $(ISO_IMAGE)

run: $(ISO_IMAGE)
	@echo "Running NAOS..."
	@qemu-system-i386 $(QEMU_FLAGS)

clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR) $(ISO_IMAGE)
	@echo "Done."

lsp: | _check-bear
	bear -- make build

_check-bear:
	@if ! command -v bear >/dev/null 2>&1; then \
		echo "ERROR: bear not found. Please install it for compilation database support."; \
		exit 1; \
	fi

$(ISO_IMAGE): $(KERNEL_BIN) $(GRUB_CFG) | $(GRUB_DIR)
	@echo "Creating ISO image..."
	@cp $(KERNEL_BIN) $(BOOT_DIR)/naos.bin
	@cp $(GRUB_CFG) $(GRUB_DIR)/grub.cfg
	grub-mkrescue -o $@ $(ISO_DIR)

$(KERNEL_BIN): $(ALL_OBJS) $(LINKER_SCRIPT)
	@echo "Linking kernel..."
	$(LD) $(LDFLAGS) $(ALL_OBJS) -o $@
	@if ! grub-file --is-x86-multiboot $@; then \
		echo "Error: Kernel is not a valid Multiboot image"; \
		exit 1; \
	fi

# Pattern rules for object files
$(BUILD_DIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(ARCH_CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: src/%.S
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS) $(ARCH_ASFLAGS) $< -o $@

# Directory creation
$(GRUB_DIR): | $(BOOT_DIR)
	@mkdir -p $@

$(BOOT_DIR): | $(ISO_DIR)
	@mkdir -p $@

$(ISO_DIR): | $(BUILD_DIR)
	@mkdir -p $@