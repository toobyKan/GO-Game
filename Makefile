CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS = -lSDL2
INCLUDES = -I./headers
SRC = main.cpp src/board.cpp src/sdl_graphics_renderer.cpp src/game_logic.cpp
OBJ = build/main.o build/board.o build/sdl_graphics_renderer.o build/game_logic.o

# The target executable
main: $(OBJ)
	$(CXX) $(OBJ) $(LDFLAGS) -o main

# Compile object files
build/main.o: main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c main.cpp -o build/main.o

build/board.o: src/board.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c src/board.cpp -o build/board.o

build/sdl_graphics_renderer.o: src/sdl_graphics_renderer.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c src/sdl_graphics_renderer.cpp -o build/sdl_graphics_renderer.o
	
build/game_logic.o: src/game_logic.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c src/game_logic.cpp -o build/game_logic.o

# Clean the build directory
clean:
	rm -f build/*.o main
