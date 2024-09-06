#include "tetris.h"

void init_game(TetrisState *state) {
  // Инициализация поля
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      if (i == 0 || j == 0 || j == FIELD_WIDTH - 1 || i == FIELD_HEIGHT - 1) {
        state->field[i][j] = 1;  // Ограждение поля
      } else {
        state->field[i][j] = 0;
      }
    }
  }
  state->figureX = FIELD_WIDTH / 2 - FIGURE_SIZE / 2;
  state->figureY = 1;  // начальная позиция
  state->score = 0;    // Инициализация счета нулем
  state->level = 1;    // Ставим 1 уровень
  srand(time(NULL));

  // Генерация следующей фигуры и установка её как текущей
  generate_next_figure(state);
  spawn_current_figure(state);

  // Загрузка максимального счета
  load_max_score(state);
}

// Функция для загрузки максимального счета из файла
void load_max_score(TetrisState *state) {
  FILE *file = fopen(SCORE_FILE, "r");
  if (file) {
    if (fscanf(file, "%d", &state->maxScore) != 1) {
      state->maxScore = 0;  // Если файл пустой или данные некорректны
    }
    fclose(file);
  } else {
    state->maxScore =
        0;  // Файл не существует, устанавливаем начальное значение
    save_max_score(state);  // Создаем файл и записываем начальное значение
  }
}

// Функция для сохранения максимального счета в файл
void save_max_score(TetrisState *state) {
  FILE *file = fopen(SCORE_FILE, "w");
  if (file) {
    fprintf(file, "%d", state->maxScore);
    fclose(file);
  } else {
    perror("Error opening file for writing");
  }
}

// Генерация следующей фигуры
void generate_next_figure(TetrisState *state) {
  int figures[7][4][4] = {
      {{2, 2, 2, 2}, {2, 1, 1, 2}, {2, 1, 1, 2}, {2, 2, 2, 2}},  // square
      {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},  // line
      {{1, 0, 0, 2}, {1, 1, 1, 2}, {0, 0, 0, 2}, {2, 2, 2, 2}},  // leftCorner
      {{0, 0, 1, 2}, {1, 1, 1, 2}, {0, 0, 0, 2}, {2, 2, 2, 2}},  // rightCorner
      {{1, 1, 0, 2}, {0, 1, 1, 2}, {0, 0, 0, 2}, {2, 2, 2, 2}},  // leftSnake
      {{0, 1, 1, 2}, {1, 1, 0, 2}, {0, 0, 0, 2}, {2, 2, 2, 2}},  // rightSnake
      {{0, 1, 0, 2}, {1, 1, 1, 2}, {0, 0, 0, 2}, {2, 2, 2, 2}}  // centralFigure
  };
  // Генерация новой следующей фигуры
  int index = rand() % 7;
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      state->nextFigure[i][j] = figures[index][i][j];
    }
  }
}
// Спавн текущей фигуры из следующей
bool spawn_current_figure(TetrisState *state) {
  // Копируем следующую фигуру как текущую
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      state->currentFigure[i][j] = state->nextFigure[i][j];
    }
  }
  // Сброс состояния фиксации фигуры
  state->isFigureFixed = false;
  // Определение ширины текущей фигуры
  int figureWidth = get_figure_width(state->currentFigure);
  // Определение начальной позиции по горизонтали
  state->figureX = get_random_start_position(figureWidth);
  state->figureY = 1;
  // Проверка на возможность размещения новой фигуры
  for (int y = 0; y < FIGURE_SIZE; y++) {
    for (int x = 0; x < FIGURE_SIZE; x++) {
      if (state->currentFigure[y][x] == 1 &&
          state->field[state->figureY + y][state->figureX + x] == 1) {
        return false;  // Фигура не может быть размещена, игра окончена
      }
    }
  }
  // Генерация следующей фигуры для будущего
  generate_next_figure(state);
  return true;  // Новая фигура успешно размещена
}

// Определение ширины фигуры
int get_figure_width(int figure[FIGURE_SIZE][FIGURE_SIZE]) {
  int width = 0;
  for (int j = 0; j < FIGURE_SIZE; j++) {
    for (int i = 0; i < FIGURE_SIZE; i++) {
      if (figure[i][j] == 1) {
        width = j + 1;
        break;
      }
    }
  }
  return width;
}

int get_random_start_position(int figureWidth) {
  // Учитываем границы поля
  int return_val;
  int minX = 1;
  int maxX = FIELD_WIDTH - figureWidth - 1;
  // Если фигура шире доступного диапазона, начинаем с минимальной позиции
  if (maxX < minX) {
    return_val = minX;
  }
  // Генерация случайного числа в допустимом диапазоне]
  return_val = minX + rand() % (maxX - minX + 1);
  return return_val;
}

// Функция для фиксации фигуры на поле
void fix_figure(TetrisState *state) {
  for (int y = 0; y < FIGURE_SIZE; y++) {
    for (int x = 0; x < FIGURE_SIZE; x++) {
      if (state->currentFigure[y][x] == 1) {
        state->field[state->figureY + y][state->figureX + x] = 1;
      }
    }
  }
}

// Проверка и удаление полных линий
void check_lines(TetrisState *state) {
  int linesCleared = 0;
  for (int i = 1; i < FIELD_HEIGHT - 1; i++) {
    bool lineFull = true;
    for (int j = 1; j < FIELD_WIDTH - 1; j++) {
      if (state->field[i][j] == 0) {
        lineFull = false;
        break;
      }
    }
    if (lineFull) {
      linesCleared++;
      for (int k = i; k > 1; k--) {  // Сдвигаем все строки сверху вниз
        for (int j = 1; j < FIELD_WIDTH - 1; j++) {
          state->field[k][j] = state->field[k - 1][j];
        }
      }
    }
  }
  // Обновление счета
  switch (linesCleared) {
    case 1:
      state->score += 100;
      break;
    case 2:
      state->score += 300;
      break;
    case 3:
      state->score += 700;
      break;
    case 4:
      state->score += 1500;
      break;
  }
  // Повышаем уровень
  if (state->score / 600 > state->level - 1 && state->level < 10) {
    state->level++;
  }
  // Сохраняем максимальный счет
  if (state->score > state->maxScore) {
    state->maxScore = state->score;
    save_max_score(state);
  }
}

void hard_drop(TetrisState *state) {
  // Пока фигура может двигаться вниз, сдвигаем её
  while (move_figure_down(state)) {
    // move_figure_down возвращает true, если фигура продолжает движение
  }
}

// Функция для перемещения фигуры вниз
bool move_figure_down(TetrisState *state) {
  bool collision = false;

  // Проверяем, может ли фигура двигаться вниз
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (state->currentFigure[i][j] == 1 &&
          state->field[state->figureY + i + 1][state->figureX + j] == 1) {
        collision = true;
        break;
      }
    }
  }
  if (!collision) {
    state->figureY++;
  }

  return !collision;
}

// Перемещение фигуры по горизонтали
void move_figure_horizontal(TetrisState *state, int dx) {
  bool collision = false;

  // Проверка на возможность перемещения
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (state->currentFigure[i][j] == 1 &&
          state->field[state->figureY + i][state->figureX + j + dx] == 1) {
        collision = true;
        break;
      }
    }
  }
  // Если нет столкновения, перемещаем фигуру
  if (!collision) {
    state->figureX += dx;
  }
}

// Вращение фигуры
void rotate_figure(TetrisState *state) {
  int tempFigure[FIGURE_SIZE][FIGURE_SIZE];

  // Копируем текущую фигуру
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      tempFigure[i][j] = state->currentFigure[i][j];
    }
  }

  // Вращаем копию
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      state->currentFigure[j][FIGURE_SIZE - 1 - i] = tempFigure[i][j];
    }
  }

  // Проверка на столкновение после вращения
  bool collision = false;
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (state->currentFigure[i][j] == 1 &&
          state->field[state->figureY + i][state->figureX + j] == 1) {
        collision = true;
        break;
      }
    }
  }

  // Если столкновение обнаружено, откатываем вращение
  if (collision) {
    for (int i = 0; i < FIGURE_SIZE; i++) {
      for (int j = 0; j < FIGURE_SIZE; j++) {
        state->currentFigure[i][j] = tempFigure[i][j];
      }
    }
  }
}

// Обработка ввода пользователя
void userInput(TetrisState *state, int ch) {
  if (state->state == START) {
    if (ch == ' ') {
      init_game(state);
      state->state = PLAYING;
    } else if (ch == 'q') {
      state->state = EXIT;
    }
  } else if (state->state == GAME_OVER) {
    if (ch == ' ') {
      init_game(state);
      state->state = PLAYING;
    } else if (ch == 'q') {
      state->state = EXIT;
    }
  } else if (state->state == PAUSED) {
    if (ch == 'p' || ch == 'P') {
      state->state = PLAYING;
    } else if (ch == 'q') {
      state->state = EXIT;
    }
  } else if (state->state == PLAYING) {
    switch (ch) {
      case 'q':
      case 'Q':
        state->state = EXIT;
        break;
      case 'p':
      case 'P':
        state->state = PAUSED;
        break;
      case KEY_LEFT:
        move_figure_horizontal(state, -1);
        break;
      case KEY_RIGHT:
        move_figure_horizontal(state, 1);
        break;
      case KEY_UP:
        rotate_figure(state);
        break;
      case KEY_DOWN:
        if (!state->isFigureFixed) {
          hard_drop(state);
        }
        break;
      case 's':
        if (!state->isFigureFixed) {
          move_figure_down(state);
        }
        break;
    }
  }
}

// Кадровка игры
void move_and_level_check(TetrisState *state, int *timer) {
  // Период ожидания обновляется в зависимости от уровня
  int delay =
      12 - (state->level - 1);  // Уменьшение задержки на 50 мс за уровень
  if (delay <= 2) {
    delay = 2;  // Минимальная задержка
  }
  if (state->state == PLAYING && (*timer)++ >= delay) {
    // Проверяем, может ли фигура двигаться вниз
    if (!move_figure_down(state)) {
      // Если не может - фиксируем фигуру
      if (!state->isFigureFixed) {
        fix_figure(state);  // Фиксируем фигуру на поле
        state->isFigureFixed = true;
      }
      // Проверяем линии
      check_lines(state);
      // Спавним новую фигуру, если старая зафиксирована
      if (state->isFigureFixed) {
        if (!spawn_current_figure(state)) {
          state->state =
              GAME_OVER;  // Игра заканчивается, если спавн невозможен
        } else {
          state->isFigureFixed =
              false;  // Флаг сбрасывается после успешного спавна
        }
      }
    }
    (*timer) = 0;  // Сбрасываем таймер
  }
}