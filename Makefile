# Compiler and SDL config can be overridden for cross-compilation
CXX ?= g++
SDL_CONFIG ?= sdl2-config
CXXFLAGS ?= -std=c++17 $(shell $(SDL_CONFIG) --cflags)
LDFLAGS ?= $(shell $(SDL_CONFIG) --libs)
SRCS = $(wildcard src/*.cpp)
TARGET = raytracer.exe

all: $(TARGET) run copy-release

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) $(LDFLAGS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

copy-release: $(TARGET)
	mkdir -p releases
	cp $(TARGET) releases/

clean:
	rm -f $(TARGET)

.PHONY: all run copy-release clean
