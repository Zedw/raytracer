CXX = g++
CXXFLAGS = -std=c++17
LDFLAGS = -lSDL2
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
