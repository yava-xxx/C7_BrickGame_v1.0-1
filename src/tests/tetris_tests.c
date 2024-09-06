#include "tetris_tests.h"

// Функция для настройки состояния игры перед тестом
void setup_game(TetrisState *state, int figureX, int figureY, int score,
                int level) {
  init_game(state);
  state->figureX = figureX;
  state->figureY = figureY;
  state->score = score;
  state->level = level;
  state->state = PLAYING;
}

void field_line_fill(TetrisState *state, int j) {
  for (int i = 0; i < FIELD_WIDTH; i++) state->field[j][i] = 1;
}

void choose_figure(TetrisState *state, int index) {
  int figures[7][4][4] = {
      {{2, 2, 2, 2}, {2, 1, 1, 2}, {2, 1, 1, 2}, {2, 2, 2, 2}},  // square
      {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},  // line
      {{1, 0, 0, 2}, {1, 1, 1, 2}, {0, 0, 0, 2}, {2, 2, 2, 2}},  // leftCorner
      {{0, 0, 1, 2}, {1, 1, 1, 2}, {0, 0, 0, 2}, {2, 2, 2, 2}},  // rightCorner
      {{1, 1, 0, 2}, {0, 1, 1, 2}, {0, 0, 0, 2}, {2, 2, 2, 2}},  // leftSnake
      {{0, 1, 1, 2}, {1, 1, 0, 2}, {0, 0, 0, 2}, {2, 2, 2, 2}},  // rightSnake
      {{0, 1, 0, 2}, {1, 1, 1, 2}, {0, 0, 0, 2}, {2, 2, 2, 2}}  // centralFigure
  };
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      state->nextFigure[i][j] = figures[index][i][j];
    }
  }
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      state->currentFigure[i][j] = state->nextFigure[i][j];
    }
  }
}

// Тестовый случай для функции get_random_start_position
START_TEST(test_get_random_start_position) {
  // Проверка, что функция возвращает случайное число в допустимом диапазоне
  int minX = 1;
  int maxX = FIELD_WIDTH - 5;  // FIELD_WIDTH - 5 для исключения крайних границ
  for (int i = 0; i < 1000;
       i++) {  // Выполняем 1000 тестов для увеличения вероятности
               // покрытия всех возможных значений
    int result = get_random_start_position(FIELD_WIDTH - 2);
    ck_assert(result >= minX);
    ck_assert(result <= maxX);
  }
}
END_TEST

// Тестовый случай для функции get_figure_width
START_TEST(test_get_figure_width) {
  int figure[FIGURE_SIZE][FIGURE_SIZE] = {
      {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  int result = get_figure_width(figure);
  ck_assert(result == 4);
}
END_TEST

// Тестовый случай для функции spawn_current_figure
START_TEST(test_spawn_current_figure_failure) {
  TetrisState state;
  init_game(&state);
  setup_game(&state, 2, 1, 0, 1);
  // Проверка, что функция возвращает false, если следующая фигура не может быть
  // размещена
  field_line_fill(&state, 1);
  field_line_fill(&state, 2);
  bool result = spawn_current_figure(&state);
  ck_assert(!result);
}
END_TEST

START_TEST(test_spawn_current_figure_success) {
  TetrisState state;
  init_game(&state);
  // Проверка, что функция возвращает true, если следующая фигура может быть
  // размещена
  bool result = spawn_current_figure(&state);
  ck_assert(result);
}
END_TEST

// Тестовый случай для функции move_figure_down
START_TEST(test_move_figure_down_blocked) {
  TetrisState state;
  init_game(&state);
  setup_game(&state, 5, 4, 0, 1);
  field_line_fill(&state, 6);
  // Проверка, что функция возвращает false, если фигура не может двигаться вниз
  bool result = move_figure_down(&state);
  ck_assert(!result);
}
END_TEST

START_TEST(test_move_figure_down_success) {
  TetrisState state;
  init_game(&state);
  // Проверка, что фигура движется вниз, если нет препятствий
  bool result = move_figure_down(&state);
  ck_assert(result);
  ck_assert(state.figureY == 2);
}
END_TEST

// Тестовый случай для функции move_figure_horizontal
START_TEST(test_move_figure_horizontal_blocked) {
  TetrisState state;
  init_game(&state);
  setup_game(&state, 5, 5, 0, 1);
  choose_figure(&state, 1);
  // Проверка, что функция не перемещает фигуру, если есть препятствие
  state.field[5][7] = 1;
  move_figure_horizontal(&state, 1);
  ck_assert(state.figureX == 5);
}
END_TEST

START_TEST(test_move_figure_horizontal_success) {
  TetrisState state;
  init_game(&state);
  setup_game(&state, 5, 5, 0, 1);
  choose_figure(&state, 1);
  // Проверка, что фигура перемещается горизонтально, если нет  препятствий
  move_figure_horizontal(&state, 1);
  ck_assert(state.figureX == 6);
}
END_TEST

// Тестовый случай для функции rotate_figure
START_TEST(test_rotate_figure_blocked) {
  TetrisState state;
  setup_game(&state, 11, 5, 0, 1);
  // Проверка, что функция не вращает фигуру, если после вращения будет
  // столкновение
  choose_figure(&state, 1);
  int tempFigure[FIGURE_SIZE][FIGURE_SIZE];
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      tempFigure[i][j] = state.currentFigure[i][j];
    }
  }
  rotate_figure(&state);
  // Проверяем, что фигура не изменилась после попытки вращения
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      ck_assert(state.currentFigure[i][j] == tempFigure[i][j]);
    }
  }
}
END_TEST

START_TEST(test_rotate_figure_success) {
  TetrisState state;
  init_game(&state);
  setup_game(&state, 5, 5, 0, 1);
  choose_figure(&state, 1);
  // Проверка, что фигура вращается, если после вращения нет столкновения
  int tempFigure[FIGURE_SIZE][FIGURE_SIZE];
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      tempFigure[i][j] = state.currentFigure[i][j];
    }
  }
  rotate_figure(&state);
  bool result = true;
  // Проверяем, что фигура изменилась после успешного вращения
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (state.currentFigure[i][j] != tempFigure[i][j]) {
        result = false;
        break;
      }
    }
  }
  ck_assert(!result);
}
END_TEST

// Тестовый случай для функции check_lines
START_TEST(test_check_lines_empty) {
  TetrisState state;
  setup_game(&state, FIELD_WIDTH / 2 - FIGURE_SIZE / 2, 1, 0, 1);
  // Проверка, что функция не удаляет линии, если они пустые
  check_lines(&state);
  for (int i = 1; i < FIELD_HEIGHT - 1; i++) {
    for (int j = 1; j < FIELD_WIDTH - 1; j++) {
      ck_assert(state.field[i][j] == 0);
    }
  }
}
END_TEST

START_TEST(test_check_lines_full1) {
  TetrisState state;
  init_game(&state);
  // Проверка, что функция удаляет полные линии и сдвигает остальные вниз
  field_line_fill(&state, 20);
  check_lines(&state);
  for (int i = 1; i < FIELD_HEIGHT - 1; i++) {
    for (int j = 1; j < FIELD_WIDTH - 1; j++) {
      ck_assert(state.field[i][j] == 0);
    }
  }
}
END_TEST

START_TEST(test_check_lines_full2) {
  TetrisState state;
  init_game(&state);
  // Проверка, что функция удаляет полные линии и сдвигает остальные вниз
  field_line_fill(&state, 20);
  field_line_fill(&state, 19);
  check_lines(&state);
  for (int i = 1; i < FIELD_HEIGHT - 1; i++) {
    for (int j = 1; j < FIELD_WIDTH - 1; j++) {
      ck_assert(state.field[i][j] == 0);
    }
  }
}
END_TEST

START_TEST(test_check_lines_full3) {
  TetrisState state;
  init_game(&state);
  // Проверка, что функция удаляет полные линии и сдвигает остальные вниз
  field_line_fill(&state, 20);
  field_line_fill(&state, 19);
  field_line_fill(&state, 18);
  check_lines(&state);
  for (int i = 1; i < FIELD_HEIGHT - 1; i++) {
    for (int j = 1; j < FIELD_WIDTH - 1; j++) {
      ck_assert(state.field[i][j] == 0);
    }
  }
}
END_TEST

START_TEST(test_check_lines_full) {
  TetrisState state;
  init_game(&state);
  // Проверка, что функция удаляет полные линии и сдвигает остальные вниз
  field_line_fill(&state, 20);
  field_line_fill(&state, 19);
  field_line_fill(&state, 18);
  field_line_fill(&state, 17);
  check_lines(&state);
  for (int i = 1; i < FIELD_HEIGHT - 1; i++) {
    for (int j = 1; j < FIELD_WIDTH - 1; j++) {
      ck_assert(state.field[i][j] == 0);
    }
  }
}
END_TEST

// Тестовый случай для функции hard_drop
START_TEST(test_hard_drop) {
  TetrisState state;
  setup_game(&state, 5, 5, 0, 1);
  choose_figure(&state, 6);
  // Проверка, что функция перемещает фигуру вниз до упора
  hard_drop(&state);
  ck_assert(state.figureY == 19);
}
END_TEST

// Тестовый случай для функции fix_figure
START_TEST(test_fix_figure) {
  TetrisState state;
  setup_game(&state, 5, 19, 0, 1);
  choose_figure(&state, 6);
  // Проверка, что функция фиксирует фигуру на полеЫ
  fix_figure(&state);
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (state.currentFigure[i][j] == 1) {
        ck_assert(state.field[state.figureY + i][state.figureX + j] == 1);
      }
    }
  }
}
END_TEST

// Тестовый случай для функции load_max_score
START_TEST(test_load_max_score) {
  TetrisState state;
  init_game(&state);
  // Проверка, что функция загружает максимальный счет из файла
  state.maxScore = 1000;
  save_max_score(&state);
  load_max_score(&state);
  ck_assert(state.maxScore == 1000);
}
END_TEST

// Тестовый случай для функции save_max_score
START_TEST(test_save_max_score) {
  TetrisState state;
  init_game(&state);
  // Проверка, что функция сохраняет максимальный счет в файл
  state.maxScore = 1000;
  save_max_score(&state);
  load_max_score(&state);
  ck_assert(state.maxScore == 1000);
}
END_TEST

// Тестовый случай для функции userInput
START_TEST(test_userInput_exit) {
  TetrisState state;
  setup_game(&state, FIELD_WIDTH / 2 - FIGURE_SIZE / 2, 1, 0, 1);
  // Проверка, что функция обрабатывает нажатие клавиши 'q' для выхода из игры
  userInput(&state, 'q');
  ck_assert(state.state == EXIT);
}
END_TEST

START_TEST(test_userInput_pause) {
  TetrisState state;
  setup_game(&state, FIELD_WIDTH / 2 - FIGURE_SIZE / 2, 1, 0, 1);
  // Проверка, что функция обрабатывает нажатие клавиши 'p' для паузы в игре
  userInput(&state, 'p');
  ck_assert(state.state == PAUSED);
}
END_TEST

// Тестовый случай для функции move_and_level_check
START_TEST(test_move_and_level_check) {
  TetrisState state;
  init_game(&state);
  setup_game(&state, 5, 17, 0, 10);
  choose_figure(&state, 1);
  int timer = 12;
  field_line_fill(&state, 1);
  move_and_level_check(&state, &timer);
  ck_assert(state.state == GAME_OVER);
}
END_TEST

START_TEST(test_move_and_level_check2) {
  TetrisState state;
  init_game(&state);
  setup_game(&state, 5, 17, 0, 10);
  choose_figure(&state, 1);
  int timer = 12;
  move_and_level_check(&state, &timer);
  ck_assert(state.isFigureFixed == false);
}
END_TEST

// Тестовый случай для функции userInput
START_TEST(test_userInput_q) {
  TetrisState state;
  init_game(&state);
  setup_game(&state, 5, 5, 1, 1);
  choose_figure(&state, 6);
  state.state = PLAYING;
  userInput(&state, 'q');
  ck_assert(state.state == EXIT);
  state.state = PLAYING;
  userInput(&state, 'Q');
  ck_assert(state.state == EXIT);
}
END_TEST

START_TEST(test_userInput_start) {
  TetrisState state;
  init_game(&state);
  setup_game(&state, 5, 5, 1, 1);
  choose_figure(&state, 6);
  state.state = START;
  userInput(&state, ' ');
  ck_assert(state.state == PLAYING);
  state.state = START;
  userInput(&state, 'q');
  ck_assert(state.state == EXIT);
}
END_TEST

START_TEST(test_userInput_gameover) {
  TetrisState state;
  init_game(&state);
  setup_game(&state, 5, 5, 1, 1);
  choose_figure(&state, 6);
  state.state = GAME_OVER;
  userInput(&state, ' ');
  ck_assert(state.state == PLAYING);
  state.state = GAME_OVER;
  userInput(&state, 'q');
  ck_assert(state.state == EXIT);
}
END_TEST

START_TEST(test_userInput_paused) {
  TetrisState state;
  init_game(&state);
  setup_game(&state, 5, 5, 1, 1);
  choose_figure(&state, 6);
  state.state = PAUSED;
  userInput(&state, 'p');
  ck_assert(state.state == PLAYING);
  state.state = PAUSED;
  userInput(&state, 'q');
  ck_assert(state.state == EXIT);
}
END_TEST

START_TEST(test_userInput_move) {
  TetrisState state;
  init_game(&state);
  setup_game(&state, 5, 5, 1, 1);
  choose_figure(&state, 6);
  state.state = PLAYING;
  userInput(&state, KEY_LEFT);
  ck_assert(state.figureX == 4);
  userInput(&state, KEY_RIGHT);
  ck_assert(state.figureX == 5);
  userInput(&state, KEY_UP);
  ck_assert(state.figureY == 5);
  userInput(&state, KEY_UP);
  userInput(&state, KEY_UP);
  userInput(&state, KEY_UP);
  userInput(&state, KEY_DOWN);
  ck_assert(state.figureY == 19);
}
END_TEST

// Suite для тестов
Suite *tetris_suite() {
  Suite *s = suite_create("Tetris");

  // Добавление тестовых случаев в Suite
  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_get_random_start_position);
  tcase_add_test(tc_core, test_get_figure_width);
  tcase_add_test(tc_core, test_spawn_current_figure_failure);
  tcase_add_test(tc_core, test_spawn_current_figure_success);
  tcase_add_test(tc_core, test_move_figure_down_blocked);
  tcase_add_test(tc_core, test_move_figure_down_success);
  tcase_add_test(tc_core, test_move_figure_horizontal_blocked);
  tcase_add_test(tc_core, test_move_figure_horizontal_success);
  tcase_add_test(tc_core, test_rotate_figure_blocked);
  tcase_add_test(tc_core, test_rotate_figure_success);
  tcase_add_test(tc_core, test_check_lines_empty);
  tcase_add_test(tc_core, test_check_lines_full1);
  tcase_add_test(tc_core, test_check_lines_full2);
  tcase_add_test(tc_core, test_check_lines_full3);
  tcase_add_test(tc_core, test_check_lines_full);
  tcase_add_test(tc_core, test_hard_drop);
  tcase_add_test(tc_core, test_fix_figure);
  tcase_add_test(tc_core, test_load_max_score);
  tcase_add_test(tc_core, test_save_max_score);
  tcase_add_test(tc_core, test_userInput_exit);
  tcase_add_test(tc_core, test_userInput_pause);
  tcase_add_test(tc_core, test_move_and_level_check);
  tcase_add_test(tc_core, test_move_and_level_check2);
  tcase_add_test(tc_core, test_userInput_q);
  tcase_add_test(tc_core, test_userInput_start);
  tcase_add_test(tc_core, test_userInput_gameover);
  tcase_add_test(tc_core, test_userInput_paused);
  tcase_add_test(tc_core, test_userInput_move);
  suite_add_tcase(s, tc_core);

  return s;
}

int main() {
  Suite *s = tetris_suite();
  SRunner *sr = srunner_create(s);
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_ENV);
  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}