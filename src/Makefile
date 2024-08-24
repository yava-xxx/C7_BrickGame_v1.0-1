CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror
BUILD_DIR = ./build
TETRIS_SRC = ./brick_game/tetris/backend.c
CLI_SRC = ./gui/cli/frontend.c

ifeq ($(OS),Linux)
	MATH = -lm
	LIBS = -lncurses -lrt -lpthread -lsubunit $(MATH) -std=gnu99
else
	MATH = 
	LIBS = -lncurses -I$(shell brew --prefix check)/include
endif

all: $(BUILD_DIR)/tetris

$(BUILD_DIR)/tetris: $(TETRIS_SRC) $(CLI_SRC)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/tetris $(TETRIS_SRC) $(CLI_SRC) $(LIBS)
	

install: $(BUILD_DIR)/tetris
	touch $(BUILD_DIR)/score.txt
	echo 0 > $(BUILD_DIR)/score.txt

uninstall:
	rm -rf /home/yava/distant/Brickgame/src/build

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