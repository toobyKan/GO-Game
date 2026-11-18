CC := g++
CFLAGS := -std=c++11 -Wall
INCLUDE_DIR := headers
SRC_DIR := src
BUILD_DIR := build

rwildcard = $(foreach d,$(wildcard $(1)*/),$(call rwildcard,$(d),$(2))) $(wildcard $(1)$(2))
APP_SRCS := main.cpp $(call rwildcard,$(SRC_DIR)/,*.cpp)
APP_OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(APP_SRCS))

TARGET := $(BUILD_DIR)/main
TEST_TARGET := $(BUILD_DIR)/test_game_logic
GEOMETRY_TEST_TARGET := $(BUILD_DIR)/test_board_geometry
STATE_TEST_TARGET := $(BUILD_DIR)/test_state_manager
SESSION_TEST_TARGET := $(BUILD_DIR)/test_local_game_session
EDGE_GEOMETRY_TEST_TARGET := $(BUILD_DIR)/test_geometry_edge_cases
LIBS := -lSDL2 -lSDL2_ttf

all: $(TARGET)

$(TARGET): $(APP_OBJS)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

$(TEST_TARGET): tests/test_game_logic.cpp src/board.cpp src/game_logic.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ -I$(INCLUDE_DIR)

$(GEOMETRY_TEST_TARGET): tests/test_board_geometry.cpp src/board_geometry.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ -I$(INCLUDE_DIR)

$(STATE_TEST_TARGET): tests/test_state_manager.cpp src/states/state_manager.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ -I$(INCLUDE_DIR)

$(SESSION_TEST_TARGET): tests/test_local_game_session.cpp src/local_game_session.cpp src/board.cpp src/game_logic.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ -I$(INCLUDE_DIR)

$(EDGE_GEOMETRY_TEST_TARGET): tests/test_geometry_edge_cases.cpp src/board_geometry.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ -I$(INCLUDE_DIR)

test: $(TEST_TARGET) $(GEOMETRY_TEST_TARGET) $(STATE_TEST_TARGET) $(SESSION_TEST_TARGET) $(EDGE_GEOMETRY_TEST_TARGET)
	./$(TEST_TARGET)
	./$(GEOMETRY_TEST_TARGET)
	./$(STATE_TEST_TARGET)
	./$(SESSION_TEST_TARGET)
	./$(EDGE_GEOMETRY_TEST_TARGET)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all test clean