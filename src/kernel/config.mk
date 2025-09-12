KERNEL_SRCS := $(shell find src/kernel -name '*.cpp')
KERNEL_OBJS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(KERNEL_SRCS))