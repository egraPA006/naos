# Choose architecture
ARCH ?= x86

include config.mk

include src/arch/$(ARCH)/config.mk

include src/kernel/config.mk
include src/libs/klib/config.mk
ISO_IMAGE := naos.iso

ALL_OBJS := $(CRTI_OBJ) $(CRTBEGIN_OBJ) \
            $(ARCH_OBJS) \
            $(KERNEL_OBJS) \
            $(KLIB_OBJS) \
            $(CRTEND_OBJ) $(CRTN_OBJ)

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

build: $(ISO_IMAGE) | _check-toolchain _check-crt
	@echo $(ISO_IMAGE)

run: $(ISO_IMAGE)
	@echo "Running NAOS..."
	@qemu-system-$(QEMU_ARCH) $(QEMU_FLAGS)

lsp: | _check-bear
	bear -- make build

clean:
	@echo "Cleaning build artefacts..."
	@rm -rf $(BUILD_DIR) $(ISO_IMAGE) .cache/ compile_commands.json
	@echo "Done."

_ensure_build_dir:
	@mkdir -p $(BUILD_DIR)

$(ISO_IMAGE): $(KERNEL_BIN) $(GRUB_CFG) | $(GRUB_DIR)
	@echo "Creating ISO image..."
	@cp $(KERNEL_BIN) $(BOOT_DIR)/naos.bin
	@cp $(GRUB_CFG) $(GRUB_DIR)/grub.cfg
	grub-mkrescue -o $@ $(ISO_DIR)

$(KERNEL_BIN): $(ALL_OBJS) $(LINKER_SCRIPT)
	@echo "Linking kernel..."
	$(LD) $(LDFLAGS) $(ALL_OBJS) -o $@
	@if ! grub-file --is-$(ARCH)-multiboot $@; then \
		echo "Error: Kernel is not a valid Multiboot image"; \
		exit 1; \
	fi

# Pattern rules for building object files
$(BUILD_DIR)/%.o: src/%.cpp | _ensure_build_dir
	@echo "compiling file"
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(CRTI_OBJ): $(CRTI_SRC) | _ensure_build_dir
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS) $< -o $@

$(CRTN_OBJ): $(CRTN_SRC) | _ensure_build_dir
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS) $< -o $@

$(BOOT_OBJ): $(BOOT_SRC) | _ensure_build_dir
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS) $< -o $@

$(GRUB_DIR): | $(BOOT_DIR)
	@mkdir -p $@

$(BOOT_DIR): | $(ISO_DIR)
	@mkdir -p $@

$(ISO_DIR): | $(BUILD_DIR)
	@mkdir -p $@

_check-crt:
	@if [ ! -f "$(CRTBEGIN_OBJ)" ]; then \
        echo "ERROR: crtbegin.o not found in toolchain"; \
        exit 1; \
    fi
	@if [ ! -f "$(CRTEND_OBJ)" ]; then \
        echo "ERROR: crtend.o not found in toolchain"; \
        exit 1; \
    fi

_check-toolchain:
	@for tool in grub-file xorriso mformat $(CXX) $(AS); do \
		if ! command -v $$tool >/dev/null 2>&1; then \
			case $$tool in \
				$(CXX)) echo "ERROR: C++ support missing in cross-compiler";; \
				*) echo "ERROR: $$tool not found. Please install it.";; \
			esac; \
			exit 1; \
		fi; \
	done
_check-runtime:
	@if ! command -v qemu-system-$(QEMU_ARCH) >/dev/null 2>&1; then \
		echo "ERROR: qemu-system-$(QEMU_ARCH) not found. Please install QEMU."; \
		exit 1; \
	fi
_check-bear:
	@if ! command -v bear >/dev/null 2>&1; then \
		echo "ERROR: bear not found. Please install it for compilation database support."; \
		exit 1; \
	fi