CROSS_COMPILE := $(HOME)/opt/cross/bin/i686-elf-
CC := $(CROSS_COMPILE)gcc
CXX := $(CROSS_COMPILE)g++
ASM := nasm

build: src/boot.asm src/kernel.cpp src/linker.ld | check-toolchain build-dir
	@echo "Building the kernel..."
	@$(MAKE) build-bin
	@$(MAKE) build-iso
	@echo "Build finished."

run: naos.iso | check-runtime
	@echo "Running naos..."
	@qemu-system-i386 -cdrom naos.iso

build-dir:
	@mkdir -p build
	@mkdir -p build/isodir/boot/grub

build-bin: 	src/boot.asm src/kernel.cpp src/linker.ld
	nasm -felf32 src/boot.asm -o build/boot.o
	$(CXX) -c src/kernel.cpp -o build/kernel.o  -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
	$(CC) -T src/linker.ld -o build/naos.bin -ffreestanding -O2 -nostdlib build/boot.o build/kernel.o -lgcc
	@if ! grub-file --is-x86-multiboot build/naos.bin; then \
	  echo "Kernel built succesfuly, but signature is corrupted."; \
	  exit 1; \
	fi

build-iso: src/grub.cfg build/naos.bin
	@mkdir -p build/isodir/boot/grub
	@cp build/naos.bin build/isodir/boot/naos.bin
	@cp src/grub.cfg build/isodir/boot/grub/grub.cfg
	grub-mkrescue -o naos.iso build/isodir

clean:
	@echo "Deleting artefacts..."
	@rm -rf build
	@echo "Finished."


check-toolchain:
	@if ! command -v grub-file >/dev/null 2>&1; then \
		echo "ERROR: grub not found. Please install grub 2."; \
		exit 1; \
	fi
	@if ! command -v xorriso >/dev/null 2>&1; then \
		echo "ERROR: xorriso not found. Please install it."; \
		exit 1; \
	fi
	@if ! command -v mformat >/dev/null 2>&1; then \
		echo "ERROR: mformat not found. Please install it."; \
		exit 1; \
	fi
	@if ! command -v $(CC) >/dev/null 2>&1; then \
		echo "ERROR: $(CC) not found. Please install i686-elf cross-compiler."; \
		echo "See: https://wiki.osdev.org/GCC_Cross-Compiler"; \
		exit 1; \
	fi
	@if ! command -v $(CXX) >/dev/null 2>&1; then \
		echo "ERROR: $(CXX) not found. Please add C++ support to your cross-compiler."; \
		exit 1; \
	fi
	@if ! command -v $(ASM) >/dev/null 2>&1; then \
		echo "ERROR: $(ASM) not found. Please install NASM."; \
		exit 1; \
	fi
	@if ! command -v $(CC) >/dev/null 2>&1; then \
		echo "ERROR: $(CC) not found. Please install i686-elf cross-compiler."; \
		echo "See: https://wiki.osdev.org/GCC_Cross-Compiler"; \
		exit 1; \
	fi

check-runtime:
	@if ! command -v qemu-system-i386 >/dev/null 2>&1; then \
		echo "ERROR: qemu not found. Please install qemu-system-i386."; \
		exit 1; \
	fi
