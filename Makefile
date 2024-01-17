CC=gcc
BUILD_DIR := ./build
SRC_DIR := ./src

CFLAGS=-Og               \
	   -g                \
	   -Wall             \
	   -Werror           \
	   -std=c99          \
	   -D_DEFAULT_SOURCE \


workspacer: ./src/workspacer.c
	@mkdir -p $(BUILD_DIR)
	$(CC) -o $(BUILD_DIR)/workspacer $(SRC_DIR)/workspacer.c $(CFLAGS)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) 

