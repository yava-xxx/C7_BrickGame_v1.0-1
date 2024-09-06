#include "../../brick_game/tetris/tetris.h"

void draw_game(TetrisState *state) {
  clear();  // Очистка экрана

  // Отрисовка поля
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      if (state->field[i][j] == 1) {
        attron(COLOR_PAIR(1));
        mvprintw(i, j * 2, "[]");  // Отрисовка блока поля
        attroff(COLOR_PAIR(1));
      } else {
        mvprintw(i, j * 2, "  ");  // Пустое место
      }
    }
  }

  // Отрисовка текущей фигуры
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (state->currentFigure[i][j] == 1) {
        mvprintw(state->figureY + i, (state->figureX + j) * 2, "[]");
      }
    }
  }

  // Отображение счета и максимального счета
  mvprintw(0, FIELD_OFFSET_X, "Score: %d", state->score);
  mvprintw(1, FIELD_OFFSET_X, "Max: %s", state->maxScore > 0 ? "" : "-");
  if (state->maxScore > 0) {
    mvprintw(1, FIELD_OFFSET_X + 7, "%d", state->maxScore);
  }
  mvprintw(2, FIELD_OFFSET_X, "Level: %d", state->level);

  // Отрисовка следующей фигуры
  draw_next_figure(state);

  attron(COLOR_PAIR(2));
  mvprintw(5, FIELD_OFFSET_X, "Controls:");
  mvprintw(6, FIELD_OFFSET_X, "  v: Move Down Full");
  mvprintw(7, FIELD_OFFSET_X, "  ^: Rotate");
  mvprintw(8, FIELD_OFFSET_X, "  <: Move Left");
  mvprintw(9, FIELD_OFFSET_X, "  >: Move Right");
  mvprintw(10, FIELD_OFFSET_X, "  s: Move Down Boost");
  mvprintw(11, FIELD_OFFSET_X, "  p: Pause");
  mvprintw(12, FIELD_OFFSET_X, "  q: Quit");
  attroff(COLOR_PAIR(2));

  refresh();  // Обновление экрана
}

void draw_next_figure(TetrisState *state) {
  mvprintw(0, FIGURE_OFFSET_X, "Next:");

  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (state->nextFigure[i][j] == 1) {
        mvprintw(1 + i, FIGURE_OFFSET_X + j * 2, "[]");
      } else {
        mvprintw(1 + i, FIGURE_OFFSET_X + j * 2, "  ");
      }
    }
  }
}

void draw_start_screen() {
  clear();
  mvprintw(FIELD_HEIGHT / 2, FIELD_WIDTH - 6, "Press SPACE to Start");
  refresh();
}

void draw_game_over_screen(TetrisState *state) {
  clear();
  mvprintw(FIELD_HEIGHT / 2 - 1, FIELD_WIDTH - 8, "GAME OVER");
  mvprintw(FIELD_HEIGHT / 2, FIELD_WIDTH - 8, "SCORE = %d", state->score);

  mvprintw(FIELD_HEIGHT / 2 + 1, FIELD_WIDTH - 10, "PRESS q TO QUIT OR");
  mvprintw(FIELD_HEIGHT / 2 + 2, FIELD_WIDTH - 10, "SPACE TO PLAY AGAIN");

  refresh();
}

void draw_pause_screen() {
  mvprintw(FIELD_HEIGHT - 1, FIELD_WIDTH * 2 + 3, "PAUSE");
  refresh();
}

void updateCurrentState(TetrisState *state) {
  switch (state->state) {
    case START:
      draw_start_screen();
      break;
    case PLAYING:
      draw_game(state);
      break;
    case PAUSED:
      draw_pause_screen();
      break;
    case GAME_OVER:
      draw_game_over_screen(state);
      break;
    case EXIT:
      endwin();
      break;
  }
}

int main() {
  TetrisState state;

  // Инициализация ncurses
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  timeout(80);         // Ожидание ввода 80ms
  mousemask(0, NULL);  // Отключение обработки мыши
  start_color();
  init_pair(1, COLOR_BLACK, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  state.state = START;
  int timer = 0;
  int ch;
  while (state.state != EXIT) {
    ch = getch();
    userInput(&state, ch);
    updateCurrentState(&state);
    move_and_level_check(&state, &timer);
  }
  endwin();
  return 0;
}