# DCRMS Project Makefile

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pthread -I./include
LDFLAGS = -pthread

# Directories
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
LOGS_DIR = logs
DATA_DIR = data

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
TARGET = $(BIN_DIR)/dcrms

# Targets
all: directories build

directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR) $(LOGS_DIR) $(DATA_DIR)

build: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^
	@echo "Build complete: $(TARGET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

distclean: clean
	rm -rf $(LOGS_DIR)/* $(DATA_DIR)/*

.PHONY: all directories build run clean distclean
