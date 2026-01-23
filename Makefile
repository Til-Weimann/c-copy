# Makefile created with consultation from ChatGPT

CC=gcc
CFLAGS = -Wall -Wextra -Iinclude
TEST_CFLAGS = -Wall -Wextra -Werror -Iinclude -DTESTING

BUILD_DIR = build
APP_DIR = $(BUILD_DIR)/c-copy
TEST_DIR = $(BUILD_DIR)/tests

SRC_FILES = $(wildcard src/*.c)
TEST_FILES = $(wildcard tests/*.c)

.PHONY: all test clean

all: $(APP_DIR)

$(APP_DIR): $(SRC_FILES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@

test: $(TEST_DIR)
	./$(TEST_DIR)
	
$(TEST_DIR): $(TEST_FILES) $(filter-out src/c-copy.c, $(SRC_FILES)) | $(BUILD_DIR)
	$(CC) $(TEST_CFLAGS) $^ -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)