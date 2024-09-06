CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror
BUILD_DIR = ./build
TETRIS_SRC = ./brick_game/tetris/backend.c
CLI_SRC = ./gui/cli/frontend.c
OS := $(shell uname)

ifeq ($(OS), Linux)
	MATH = -lm
	LIBS = -lcheck -lncurses -lrt -lpthread -lsubunit $(MATH) -std=gnu99
else
	MATH = 
	LIBS = -lncurses -I$(shell brew --prefix check)/include
endif

all: install

$(BUILD_DIR)/tetris: $(TETRIS_SRC) $(CLI_SRC)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/tetris $(TETRIS_SRC) $(CLI_SRC) $(LIBS)
	

install: $(BUILD_DIR)/tetris
	touch $(BUILD_DIR)/score.txt
	echo 0 > $(BUILD_DIR)/score.txt

uninstall:
	rm -rf ./build

dvi:
	open Doxyfile

dist:
	tar -czf tetris.tar.gz ./ 

test: 
	$(CC) $(CFLAGS) brick_game/tetris/backend.c tests/tetris_tests.c -o test $(LIBS)
	rm -f *.o

gcov_report: test
	$(CC) $(CFLAGS) -fprofile-arcs -ftest-coverage brick_game/tetris/backend.c tests/tetris_tests.c -o test $(LIBS)
	./test
	lcov -t "test_s21_tetris" -o s21_test.info -c -d .
	genhtml -o report s21_test.info
	rm -f *.o
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.txt
	rm -f test

clean:
	rm -f *.o *.txt
	rm -f *.tar.gz
	rm -f test
	rm -f s21_test.info
	rm -f *.gcda
	rm -f *.gcno
	rm -rf report
