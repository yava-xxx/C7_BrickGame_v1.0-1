#ifndef TETRISTEST_H
#define TETRISTEST_H

#include <check.h>

#include "../brick_game/tetris/tetris.h"

void choose_figure(TetrisState *state, int index);
void field_line_fill(TetrisState *state, int j);
void setup_game(TetrisState *state, int figureX, int figureY, int score,
                int level);

#endif