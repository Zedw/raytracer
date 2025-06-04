# Compiler and SDL config can be overridden for cross-compilation
CXX ?= g++
SDL_CONFIG ?= sdl2-config
CXXFLAGS ?= -std=c++17 $(shell $(SDL_CONFIG) --cflags)
LDFLAGS ?= $(shell $(SDL_CONFIG) --libs)
SRCS = $(wildcard src/*.cpp)
IMGUI_SRCS = $(wildcard external/imgui/*.cpp) \
            $(wildcard external/imgui/backends/*.cpp)
INCLUDES = -Iinclude -Iexternal/imgui -Iexternal/imgui/backends
TARGET = raytracer.exe

all: $(TARGET) run copy-release

$(TARGET): $(SRCS) $(IMGUI_SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ $(LDFLAGS) -o $@

run: $(TARGET)
	./$(TARGET)

copy-release: $(TARGET)
	mkdir -p releases
	cp $(TARGET) releases/

clean:
	rm -f $(TARGET)

.PHONY: all run copy-release clean
