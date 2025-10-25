CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -std=c11
LDFLAGS = 

SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include
TARGET = $(BUILD_DIR)/cryptool

SOURCES = $(shell find $(SRC_DIR) -name '*.c')
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

SRC_SUBDIRS = $(shell find $(SRC_DIR) -type d)
BUILD_SUBDIRS = $(SRC_SUBDIRS:$(SRC_DIR)%=$(BUILD_DIR)%)

all: build

restart: rebuild
	@echo "\nRunning cryptool..."
	./$(TARGET) --help

build: create_dirs $(TARGET)

create_dirs:
	mkdir -p $(BUILD_SUBDIRS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean build

run: build
	./$(TARGET)

help:
	@echo "Available targets:"
	@echo "  build   - Build the cryptool utility"
	@echo "  clean   - Remove build files"
	@echo "  rebuild - Clean and rebuild"
	@echo "  run     - Build and run the program"
	@echo "  restart - Clean, rebuild and run with --help"

install:
	sudo cp $(TARGET) /usr/local/bin/cryptool

.PHONY: all build clean rebuild run restart create_dirs help install
