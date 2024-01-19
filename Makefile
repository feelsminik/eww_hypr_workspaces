CC=gcc
BUILD_DIR = ./build
SRC_DIR = ./src
PROJ_NAME = workspacerd


CFLAGS=-Og               \
	   -g                \
	   -Wall             \
	   -Werror           \
	   -std=c99          \
	   -D_DEFAULT_SOURCE \
	   -Iinclude         \
	   -lcjson           \

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

$(PROJ_NAME): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*.o $(PROJ_NAME)

