# Makefile

# Compiler
CC := g++

# Compiler flags
CFLAGS := -std=c++11 -Wall

# Directories
SRC_DIR := src
INCLUDE_DIR := includes
BUILD_DIR := build

# Source files
SRCS := main.cpp \
		$(wildcard $(SRC_DIR)/*.cpp)

# Header files
HEADERS := $(wildcard $(INCLUDE_DIR)/*.hpp)

# Object files
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Executable name
TARGET := main

# Libraries
LIBS := -lSDL2

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

# Compiling source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# PHONY targets to avoid conflicts with file names
.PHONY: all clean