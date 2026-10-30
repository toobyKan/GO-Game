# Makefile

# Compiler
CC := g++

# Compiler flags
CFLAGS := -std=c++11 -Wall

# Directories
INCLUDE_DIR := headers
SRC_DIR := src
BUILD_DIR := build

# Source files
SRCS := main.cpp $(wildcard $(SRC_DIR)/*.cpp)

OBJS := $(BUILD_DIR)/main.o \
        $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(wildcard $(SRC_DIR)/*.cpp))

TARGET := main
TEST_TARGET := $(BUILD_DIR)/test_game_logic
LIBS := -lSDL2

all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

$(BUILD_DIR)/main.o: main.cpp
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ -I.

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ -I.

$(TEST_TARGET): test_game_logic.cpp src/board.cpp src/game_logic.cpp
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@ -I.

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all test clean