# Toolchain configuration
CROSS_COMPILE ?= $(HOME)/opt/cross/bin/i686-elf-
CC := $(CROSS_COMPILE)gcc
CXX := $(CROSS_COMPILE)g++
AS := nasm
LD := $(CC)

# Directory structure
BUILD_DIR := build
ISO_DIR := $(BUILD_DIR)/isodir
BOOT_DIR := $(ISO_DIR)/boot
GRUB_DIR := $(BOOT_DIR)/grub

# Source files
BOOT_SRC := src/arch/x86/boot.asm
LINKER_SCRIPT := src/boot/linker.ld
GRUB_CFG := src/boot/grub.cfg

# Find all kernel and klib sources
ARCH_SRCS := $(shell find src/arch -name '*.cpp')
KERNEL_SRCS := $(shell find src/kernel -name '*.cpp')
KLIB_SRCS := $(shell find src/klib -name '*.cpp')

# Object files
ARCH_OBJS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(ARCH_SRCS))
BOOT_OBJ := $(BUILD_DIR)/boot.o
KERNEL_OBJS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(KERNEL_SRCS))
KLIB_OBJS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(KLIB_SRCS))


# Output files
KERNEL_BIN := $(BUILD_DIR)/naos.bin
ISO_IMAGE := naos.iso

# Compiler flags
CFLAGS := -ffreestanding -O2 -Wall -Wextra -I src/include
CXXFLAGS := $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS := -felf32
LDFLAGS := -T $(LINKER_SCRIPT) -ffreestanding -O2 -nostdlib -lgcc

.PHONY: all build run clean lsp help _ensure_build_dir

all: lsp run

help:
	@echo "NAOS Makefile Help"
	@echo "=================="
	@echo "Available targets:"
	@echo "  all       - Build everything (default target)"
	@echo "  build     - Build the OS image"
	@echo "  run       - Run the OS in QEMU"
	@echo "  clean     - Remove all build artifacts"
	@echo "  lsp       - Generate compilation database (for LSP support)"
	@echo "  help      - Show this help message"

build: $(ISO_IMAGE) | _check-toolchain

run: $(ISO_IMAGE) | _check-runtime
	@echo "Running naos..."
	@qemu-system-i386 -cdrom $(ISO_IMAGE) -serial stdio

lsp: | _check-bear
	bear -- make build

clean:
	@echo "Cleaning build artefacts..."
	@rm -rf $(BUILD_DIR) $(ISO_IMAGE) .cache/ compile_commands.json
	@echo "Done."

# Ensure build directory exists before any compilation
_ensure_build_dir:
	@mkdir -p $(BUILD_DIR)

# Main build targets
$(ISO_IMAGE): $(KERNEL_BIN) $(GRUB_CFG) | $(GRUB_DIR)
	@echo "Creating ISO image..."
	@cp $(KERNEL_BIN) $(BOOT_DIR)/naos.bin
	@cp $(GRUB_CFG) $(GRUB_DIR)/grub.cfg
	grub-mkrescue -o $@ $(ISO_DIR)

$(KERNEL_BIN): $(ARCH_OBJS) $(BOOT_OBJ) $(KERNEL_OBJS) $(KLIB_OBJS) $(LINKER_SCRIPT)
	@echo "Linking kernel..."
	$(LD) $(LDFLAGS) $(ARCH_OBJS) $(BOOT_OBJ) $(KERNEL_OBJS) $(KLIB_OBJS) -o $@
	@if ! grub-file --is-x86-multiboot $@; then \
		echo "Error: Kernel is not a valid Multiboot image"; \
		exit 1; \
	fi

# Pattern rules for object files
$(BUILD_DIR)/%.o: src/%.cpp | _ensure_build_dir
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BOOT_OBJ): $(BOOT_SRC) | _ensure_build_dir
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS) $< -o $@

# Directory creation
$(GRUB_DIR): | $(BOOT_DIR)
	@mkdir -p $@

$(BOOT_DIR): | $(ISO_DIR)
	@mkdir -p $@

$(ISO_DIR): | $(BUILD_DIR)
	@mkdir -p $@

# Dependency checks
_check-toolchain:
	@for tool in grub-file xorriso mformat $(CC) $(CXX) nasm; do \
		if ! command -v $$tool >/dev/null 2>&1; then \
			case $$tool in \
				$(CC)) echo "ERROR: i686-elf cross-compiler not found. See: https://wiki.osdev.org/GCC_Cross-Compiler";; \
				$(CXX)) echo "ERROR: C++ support missing in cross-compiler";; \
				*) echo "ERROR: $$tool not found. Please install it.";; \
			esac; \
			exit 1; \
		fi; \
	done

_check-runtime:
	@if ! command -v qemu-system-i386 >/dev/null 2>&1; then \
		echo "ERROR: qemu-system-i386 not found. Please install QEMU."; \
		exit 1; \
	fi

_check-bear:
	@if ! command -v bear >/dev/null 2>&1; then \
		echo "ERROR: bear not found. Please install it for compilation database support."; \
		exit 1; \
	fi
