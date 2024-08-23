CC = gcc
CFLAGS = -std=c11 -Wall -Wextra
SRC_DIR = .
BUILD_DIR = $(SRC_DIR)/build
BIN_DIR = bin
TETRIS_SRC = $(SRC_DIR)/brick_game/tetris/backend.c
CLI_SRC = $(SRC_DIR)/gui/cli/frontend.c

ifeq ($(OS),Linux)
	MATH = -lm
	LIBS = -lncurses -lrt -lpthread -lsubunit $(MATH) -std=gnu99
else
	MATH = 
	LIBS = -lncurses -I$(shell brew --prefix check)/include
endif

.PHONY: all install uninstall clean dvi dist test gcov_report

all: $(BUILD_DIR)/tetris

$(BUILD_DIR)/tetris: $(TETRIS_SRC) $(CLI_SRC)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/tetris $(TETRIS_SRC) $(CLI_SRC) $(LIBS)

install: $(BUILD_DIR)/tetris
	mkdir -p /usr/local/bin
	cp $(BUILD_DIR)/tetris /usr/local/bin/tetris

uninstall:
	rm -rf /home/yava/distant/Brickgame/src/build
	rm -rf /usr/local/bin/tetris

clean:
	find . -type f -not -path "./$(BUILD_DIR)/*" -not -name "Makefile" -delete

dvi:
	@echo "Not implemented"

dist:
	@echo "Not implemented"

test:
	@echo "Not implemented"

gcov_report:
	@echo "Not implemented"