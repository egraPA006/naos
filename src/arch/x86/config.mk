# Architecture-specific configuration for x86

# Toolchain
CROSS_COMPILE ?= $(HOME)/opt/cross/bin/i686-elf-
CC := $(CROSS_COMPILE)gcc
CXX := $(CROSS_COMPILE)g++
AS := $(CROSS_COMPILE)as
LD := $(CC)

# Architecture-specific flags
ARCH_CXXFLAGS := -mno-sse -mno-mmx -mno-80387 -msoft-float -mgeneral-regs-only
ARCH_ASFLAGS :=

# Architecture-specific files
ARCH_CPP_SRCS := $(shell find src/arch/x86 -name '*.cpp')
ARCH_ASM_SRCS := src/arch/x86/boot.S \
                 src/arch/x86/crti.S \
                 src/arch/x86/crtn.S

# Architecture-specific objects
ARCH_OBJS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(ARCH_CPP_SRCS)) \
             $(patsubst src/%.S,$(BUILD_DIR)/%.o,$(ARCH_ASM_SRCS))

# CRT objects
CRTBEGIN_OBJ := $(shell $(CXX) $(CXXFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ := $(shell $(CXX) $(CXXFLAGS) -print-file-name=crtend.o)
CRTI_OBJ := $(BUILD_DIR)/arch/x86/crti.o
CRTN_OBJ := $(BUILD_DIR)/arch/x86/crtn.o

# Boot configuration
LINKER_SCRIPT := src/arch/x86/linker.ld
GRUB_CFG := src/arch/x86/grub.cfg

# QEMU configuration
QEMU_FLAGS := $(QEMU_BASE_FLAGS) -cdrom $(ISO_IMAGE)