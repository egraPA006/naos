# Architecture-specific configuration for x86

CROSS_COMPILE ?= $(HOME)/opt/cross/bin/i686-elf-
CXX := $(CROSS_COMPILE)g++
AS := $(CROSS_COMPILE)as
LD := $(CC)

ARCH_SRCS := $(shell find src/arch -name '*.cpp')
BOOT_SRC := src/arch/x86/boot.S
LINKER_SCRIPT := src/boot/linker.ld
GRUB_CFG := src/boot/grub.cfg
CRTI_SRC := src/arch/x86/crti.S
CRTN_SRC := src/arch/x86/crtn.S


ARCH_OBJS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(ARCH_SRCS))
BOOT_OBJ := $(BUILD_DIR)/boot.o
CRTBEGIN_OBJ := $(shell $(CXX) $(CXXFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ := $(shell $(CXX) $(CXXFLAGS) -print-file-name=crtend.o)
CRTI_OBJ := $(BUILD_DIR)/arch/x86/crti.o
CRTN_OBJ := $(BUILD_DIR)/arch/x86/crtn.o

CFLAGS := -ffreestanding -O2 -Wall -Wextra -I src/include
CXXFLAGS := $(CFLAGS) -fno-exceptions -fno-rtti -mgeneral-regs-only -mno-sse -mno-mmx -mno-80387 -msoft-float
ASFLAGS :=
LDFLAGS := -T $(LINKER_SCRIPT) -ffreestanding -O2 -nostdlib -nostartfiles -lgcc

QEMU_FLAGS := $(QEMU_BASE_FLAGS) -cdrom $(ISO_IMAGE)
QEMU_ARCH := i386 