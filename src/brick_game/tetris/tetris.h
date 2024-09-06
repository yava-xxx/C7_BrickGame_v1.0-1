#ifndef TETRIS_H
#define TETRIS_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FIELD_WIDTH 12
#define FIELD_HEIGHT 22
#define FIGURE_SIZE 4
#define SCORE_FILE "score.txt"
#define FIELD_OFFSET_X \
  (FIELD_WIDTH * 2 + 10)  // Отступ для текста справа от поля
#define FIGURE_OFFSET_X (FIELD_OFFSET_X + 15)  // Отступ для следующей фигуры
typedef enum {
  START,
  PLAYING,
  PAUSED,
  GAME_OVER,
  EXIT
} GameState;  // Все возможные состояния игры (КА)

typedef struct {
  int field[FIELD_HEIGHT][FIELD_WIDTH];
  int currentFigure[FIGURE_SIZE][FIGURE_SIZE];
  int nextFigure[FIGURE_SIZE][FIGURE_SIZE];
  int figureX;
  int figureY;
  int score;
  bool isFigureFixed;
  int maxScore;
  int level;
  GameState state;
} TetrisState;

void updateCurrentState(TetrisState *state);
void init_game(TetrisState *state);
void draw_game(TetrisState *state);
void draw_next_figure(TetrisState *state);
void generate_next_figure(TetrisState *state);
bool spawn_current_figure(TetrisState *state);
void move_figure_down_full(TetrisState *state);
void move_figure_horizontal(TetrisState *state, int dx);
void rotate_figure(TetrisState *state);
void check_lines(TetrisState *state);
bool move_figure_down(TetrisState *state);
void hard_drop(TetrisState *state);
void fix_figure(TetrisState *state);
void save_max_score(TetrisState *state);
void load_max_score(TetrisState *state);
int get_random_start_position(int figureWidth);
int get_figure_width(int figure[FIGURE_SIZE][FIGURE_SIZE]);
void userInput(TetrisState *state, int ch);
void move_and_level_check(TetrisState *state, int *timer);

#endif