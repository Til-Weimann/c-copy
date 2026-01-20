# make a proper makefile with different targets and tests

# Made with some consultation from ChatGPT (Give me a basic example of how a C Makefile could be used to run tests, including where the tests would be saved. + follow up questions)

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


#c-copy:
#	$(CC) ./include/c-copy-headers.h ./src/progress.c ./src/jobqueue.c ./src/fileutils.c ./src/c-copy.c -o c-copy.exe
