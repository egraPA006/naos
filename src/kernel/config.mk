# Kernel module configuration

# Kernel source files
KERNEL_SRCS := $(shell find src/kernel -name '*.cpp')

# Kernel objects
KERNEL_OBJS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(KERNEL_SRCS))