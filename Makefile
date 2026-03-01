# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -fPIC -g -O0 -I$(INCLUDE_DIR)
CFLAGS_RELEASE = -Wall -Wextra -Werror -fPIC -O2 -I$(INCLUDE_DIR)

# Directories
SRC_DIR = src
INCLUDE_DIR = src/include
EXAMPLES_DIR = examples
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
LIB_DIR = $(BUILD_DIR)/lib
BIN_DIR = $(BUILD_DIR)/bin

# Library name
LIB_NAME = cds
STATIC_LIB = $(LIB_DIR)/lib$(LIB_NAME).a
SHARED_LIB = $(LIB_DIR)/lib$(LIB_NAME).so

# Source files
# $(filter-out pattern…,text) return words in 'text' that DO NOT match 'pattern'
SRC_FILES = $(filter-out $(SRC_DIR)/main.c, $(wildcard $(SRC_DIR)/*.c))
# $(patsubst pattern, replacement, text) replace 'pattern' with 'replacement' in 'text'
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Example files
# $(wildcard pattern…) get a list of files that match the pattern.
EXAMPLE_SOURCES = $(wildcard $(EXAMPLES_DIR)/*.c)
EXAMPLE_BINARIES = $(patsubst $(EXAMPLES_DIR)/%.c, $(BIN_DIR)/%, $(EXAMPLE_SOURCES))

# Phony targets
.PHONY: all clean release debug examples help

# Default target
all: debug examples

# Debug build (default CFLAGS with -g -O0)
debug: $(STATIC_LIB) $(SHARED_LIB) examples

# Release build (optimized, no debug symbols)
release: CFLAGS = $(CFLAGS_RELEASE)
release: clean $(STATIC_LIB) $(SHARED_LIB) examples

# '$@' represents the target ($(BUILD_DIR)) here.
$(BUILD_DIR) $(OBJ_DIR) $(LIB_DIR) $(BIN_DIR):
	@mkdir -p $@

# Compile source files to object files
# '|' represents order-only prerequisite. Those that are to the right of |
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create static library
# '$^' represents all the dependencies of a target, excluding duplicates.
$(STATIC_LIB): $(OBJ_FILES) | $(LIB_DIR)
	@echo "Creating static library: $@"
	ar rcs $@ $^

# Create shared library
$(SHARED_LIB): $(OBJ_FILES) | $(LIB_DIR)
	@echo "Creating shared library: $@"
	$(CC) -shared -o $@ $^

# Build examples
examples: $(EXAMPLE_BINARIES)

# '$<' represents the first prerequisite of a rule $(BIN_DIR)/%.c
$(BIN_DIR)/%: $(EXAMPLES_DIR)/%.c $(STATIC_LIB) | $(BIN_DIR)
	@echo "Building example: $@"
	$(CC) $(CFLAGS) $< -o $@ -L$(LIB_DIR) -l$(LIB_NAME)

# Clean build directory
clean:
	@echo "Cleaning build directory..."
	@rm -rf $(BUILD_DIR)

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build debug libraries and examples (default)"
	@echo "  debug      - Build with debug symbols (-g -O0)"
	@echo "  release    - Build optimized release version (-O2)"
	@echo "  examples   - Build example binaries only"
	@echo "  clean      - Remove build directory"
	@echo "  help       - Show this help message"
