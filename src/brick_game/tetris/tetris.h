#ifndef TETRIS_H
#define TETRIS_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define FIELD_WIDTH 12  // Уменьшаем ширину поля в 2 раза
#define FIELD_HEIGHT 22
#define FIGURE_SIZE 4

typedef struct {
  int field[FIELD_HEIGHT][FIELD_WIDTH];
  int currentFigure[FIGURE_SIZE][FIGURE_SIZE];
  int nextFigure[FIGURE_SIZE][FIGURE_SIZE];  // Для хранения следующей фигуры
  int figureX;
  int figureY;
  int score;
  bool isFigureFixed;  // Новый флаг для отслеживания фиксации фигуры
} TetrisState;

void init_game(TetrisState *state);
void draw_game(TetrisState *state);
void draw_next_figure(TetrisState *state);  // Для отрисовки следующей фигуры
void generate_next_figure(
    TetrisState *state);  // Отдельная функция для генерации следующей фигуры
bool spawn_current_figure(
    TetrisState *state);  // Спавн текущей фигуры из nextFigure
void move_figure_down_full(TetrisState *state);
void move_figure_horizontal(TetrisState *state, int dx);
void rotate_figure(TetrisState *state);
void check_lines(TetrisState *state);
bool move_figure_down(TetrisState *state);
void hard_drop(TetrisState *state);
void fix_figure(TetrisState *state);

#endif
