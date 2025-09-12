# Architecture-specific configuration for x86

# Architecture-specific source files
ARCH_CPP_SRCS := $(shell find src/arch/x86/hal -name '*.cpp') \
                 $(shell find src/arch/x86/kernel -name '*.cpp')
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

# Architecture-specific flags
ARCH_CXXFLAGS := -mno-sse -mno-mmx -mno-80387 -msoft-float
ARCH_ASFLAGS :=

# Architecture-specific QEMU configuration
QEMU_FLAGS := -cdrom $(ISO_IMAGE) -serial stdio

# Boot configuration
BOOT_SRC := src/arch/x86/boot.S
LINKER_SCRIPT := src/arch/x86/linker.ld
GRUB_CFG := src/arch/x86/grub.cfg