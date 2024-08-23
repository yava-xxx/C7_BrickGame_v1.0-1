#include "../../brick_game/tetris/tetris.h"

void draw_game(TetrisState *state) {
  clear();  // Очистка экрана
  // Отрисовка поля
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      if (state->field[i][j] == 1) {
        mvprintw(i, j * 2, "[]");  // Отрисовка блока поля
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

  // Отображение счета
  mvprintw(0, FIELD_WIDTH * 2 + 3, "Score: %d", state->score);

  // Отрисовка следующей фигуры
  draw_next_figure(state);
}

void draw_next_figure(TetrisState *state) {
  mvprintw(1, FIELD_WIDTH * 2 + 3, "Next:");

  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (state->nextFigure[i][j] == 1) {
        mvprintw(2 + i, (FIELD_WIDTH * 2 + 3) + j * 2, "[]");
      } else {
        mvprintw(2 + i, (FIELD_WIDTH * 2 + 3) + j * 2, "  ");
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

  // Разделяем сообщение на две строки для корректного отображения
  mvprintw(FIELD_HEIGHT / 2 + 1, FIELD_WIDTH - 10, "PRESS q TO QUIT OR");
  mvprintw(FIELD_HEIGHT / 2 + 2, FIELD_WIDTH - 10, "SPACE TO PLAY AGAIN");

  refresh();
}

void draw_pause_screen() {
  clear();
  mvprintw(FIELD_HEIGHT / 2, FIELD_WIDTH - 6, "PAUSE");
  refresh();
}

int main() {
  TetrisState state;

  // Инициализация ncurses
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  timeout(80);  // Ожидание ввода 80ms

  // Отключение обработки мышиных событий
  mousemask(0, NULL);

  draw_start_screen();
  int ch;
  while ((ch = getch()) != ' ') {
    if (ch ==
        'q') {  // Добавляем выход из игры при нажатии 'q' на начальном экране
      endwin();
      return 0;
    }
  }

  init_game(&state);

  int timer = 0;
  bool gameOver = false;
  bool paused = false;

  while (1) {
    if (gameOver) {
      draw_game_over_screen(&state);
      ch = getch();
      if (ch == 'q') {
        break;  // Выход из игры на экране завершения
      } else if (ch == ' ') {
        init_game(&state);
        gameOver = false;
      }
      continue;
    }

    if (paused) {
      draw_pause_screen();
      ch = getch();
      if (ch == 'q') {
        break;  // Выход из игры в режиме паузы
      } else if (ch == 'p') {
        paused = false;
      }
      continue;
    }

    draw_game(&state);
    ch = getch();

    switch (ch) {
      case 'q':
      case 'Q':
        endwin();
        return 0;
      case 'p':
      case 'P':
        paused = true;
        break;
      case KEY_LEFT:
        move_figure_horizontal(&state, -1);
        break;
      case KEY_RIGHT:
        move_figure_horizontal(&state, 1);
        break;
      case KEY_UP:
        rotate_figure(&state);
        break;
      case KEY_DOWN:
        if (!state.isFigureFixed) {
          hard_drop(&state);
        }
        break;
    }

    if (timer++ >= 10) {
      // Проверяем, может ли фигура двигаться вниз
      if (!move_figure_down(&state)) {
        // Если не может - фиксируем фигуру
        if (!state.isFigureFixed) {
          fix_figure(&state);  // Фиксируем фигуру на поле
          state.isFigureFixed = true;
        }

        // Проверяем линии
        check_lines(&state);

        // Спавним новую фигуру, если старая зафиксирована
        if (state.isFigureFixed) {
          if (!spawn_current_figure(&state)) {
            gameOver = true;  // Игра заканчивается, если спавн невозможен
          } else {
            state.isFigureFixed =
                false;  // Флаг сбрасывается после успешного спавна
          }
        }
      }
      timer = 0;
    }
  }
  endwin();
  return 0;
}
