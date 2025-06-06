CC=gcc
BUILD_DIR = ./build
SRC_DIR = ./src
PROJ_NAME = workspacerd


CFLAGS=-Wall             \
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

deploy:
	rm ~/dotfiles/script/workspacerd
	cp ./workspacerd ~/dotfiles/script

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*.o $(PROJ_NAME) $(BUILD_DIR)/$(PROJ_NAME)

