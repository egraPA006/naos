KLIB_SRCS := $(shell find src/libs/klib -name '*.cpp')
KLIB_OBJS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(KLIB_SRCS))