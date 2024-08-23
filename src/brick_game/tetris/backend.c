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
  state->figureY = 1;
  state->score = 0;  // Инициализация счета нулем
  srand(time(NULL));

  // Генерация следующей фигуры и установка её как текущей
  generate_next_figure(state);
  spawn_current_figure(state);
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

  // Устанавливаем начальную позицию фигуры
  state->figureX = FIELD_WIDTH / 2 - FIGURE_SIZE / 2;
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

  // Генерируем следующую фигуру для будущего
  generate_next_figure(state);

  return true;  // Новая фигура успешно размещена
}

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

  for (int i = 1; i < FIELD_HEIGHT - 1;
       i++) {  // FIELD_HEIGHT-1 чтобы не учитывать нижнюю границу
    bool lineFull = true;
    for (int j = 1; j < FIELD_WIDTH - 1;
         j++) {  // Проверяем все колонки, кроме границ
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