# DCRMS Project Makefile
# Comprehensive build system with proper dependency management

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

# Source files - Explicit list to ensure correct order
SOURCES = \
	$(SRC_DIR)/Credentials.cpp \
	$(SRC_DIR)/Logger.cpp \
	$(SRC_DIR)/Node.cpp \
	$(SRC_DIR)/ClientNode.cpp \
	$(SRC_DIR)/RegistryManager.cpp \
	$(SRC_DIR)/RoutingEngine.cpp \
	$(SRC_DIR)/AdminConsole.cpp \
	$(SRC_DIR)/main.cpp

OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS := $(addprefix $(OBJ_DIR)/, $(notdir $(OBJECTS)))
TARGET = $(BIN_DIR)/dcrms

# Phony targets
.PHONY: all directories build run clean distclean help

# Default target
all: directories build

# Create necessary directories
directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(LOGS_DIR)
	@mkdir -p $(DATA_DIR)
	@echo "✓ Directories created"

# Build the executable
build: directories $(TARGET)
	@echo "✓ Build complete: $(TARGET)"

# Link object files into executable
$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)..."
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^
	@echo "✓ Executable created successfully"

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $(notdir $<)..."
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the executable
run: build
	@echo "Starting DCRMS..."
	@./$(TARGET)

# Clean object files and binary
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "✓ Clean complete"

# Full cleanup including logs and data
distclean: clean
	@echo "Performing full cleanup..."
	@rm -rf $(LOGS_DIR)/* $(DATA_DIR)/*
	@echo "✓ Full cleanup complete"

# Help target
help:
	@echo "DCRMS - Build System"
	@echo "===================="
	@echo "Available targets:"
	@echo "  make              - Build the project (default)"
	@echo "  make build        - Build the project"
	@echo "  make run          - Build and run the executable"
	@echo "  make clean        - Remove build artifacts"
	@echo "  make distclean    - Remove all generated files"
	@echo "  make help         - Display this help message"
	@echo ""
	@echo "Example usage:"
	@echo "  make              # Build"
	@echo "  make run          # Build and run"
	@echo "  make clean        # Clean build"
