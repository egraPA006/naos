# Kernel library configuration

# Library source files
KLIB_SRCS := $(shell find src/libs/klib -name '*.cpp')

# Library objects
KLIB_OBJS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(KLIB_SRCS))