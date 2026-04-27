#include <check.h>

#include "../s21_matrix.h"

START_TEST(create_matrix) {
  const int rows = rand() % 100 + 1;
  const int cols = rand() % 100 + 1;
  matrix_t A = {0};
  s21_create_matrix(rows, cols, &A);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      A.matrix[i][j] = 0;
      ck_assert_double_eq_tol(0, A.matrix[i][j], 1e-07);
    }
  }
  ck_assert_int_eq(A.rows, rows);
  ck_assert_int_eq(A.columns, cols);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(create_no_rows) {
  const int rows = 0;
  const int cols = 10;
  matrix_t A = {0};
  ck_assert_int_eq(s21_create_matrix(rows, cols, &A), INCORRECT);
}
END_TEST

START_TEST(create_no_cols) {
  const int rows = 10;
  const int cols = 0;
  matrix_t A = {0};
  ck_assert_int_eq(s21_create_matrix(rows, cols, &A), INCORRECT);
}
END_TEST

START_TEST(create_incorrect) {
  int rows = -10;
  int columns = 10;
  matrix_t matrix = {0};
  int status = s21_create_matrix(rows, columns, &matrix);

  ck_assert_int_eq(status, INCORRECT);

  if (status == OK) s21_remove_matrix(&matrix);
}
END_TEST

START_TEST(create_null_result) {
  int rows = 5;
  int cols = 5;
  int res = s21_create_matrix(rows, cols, NULL);
  ck_assert_int_eq(res, INCORRECT);
}
END_TEST

double get_rand(double min, double max) {
  double val = (double)rand() / RAND_MAX;
  return min + val * (max - min);
}

START_TEST(eq) {
  const int rows = rand() % 100 + 1;
  const int cols = rand() % 100 + 1;
  matrix_t m = {0};
  s21_create_matrix(rows, cols, &m);
  matrix_t mtx = {0};
  s21_create_matrix(rows, cols, &mtx);

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      double rand_val = get_rand(DBL_MIN, DBL_MAX);
      m.matrix[i][j] = rand_val;
      mtx.matrix[i][j] = rand_val;
    }
  }
  ck_assert_int_eq(s21_eq_matrix(&m, &mtx), SUCCESS);
  s21_remove_matrix(&m);
  s21_remove_matrix(&mtx);
}
END_TEST

START_TEST(not_eq_1) {
  const int rows = rand() % 100 + 1;
  const int cols = rand() % 100 + 1;
  matrix_t m = {0};
  s21_create_matrix(rows, cols, &m);
  matrix_t mtx = {0};
  s21_create_matrix(rows, cols, &mtx);

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      m.matrix[i][j] = get_rand(DBL_MIN, DBL_MAX) + 1;
      mtx.matrix[i][j] = get_rand(DBL_MIN, DBL_MAX);
    }
  }
  ck_assert_int_eq(s21_eq_matrix(&m, &mtx), FAILURE);
  s21_remove_matrix(&m);
  s21_remove_matrix(&mtx);
}
END_TEST

START_TEST(not_eq_2) {
  const int rows1 = rand() % 100 + 1;
  const int cols1 = rand() % 100 + 1;
  matrix_t m = {0};
  s21_create_matrix(rows1, cols1, &m);
  const int rows2 = rand() % 100 + 1;
  const int cols2 = rand() % 100 + 1;
  matrix_t mtx = {0};
  s21_create_matrix(rows2, cols2, &mtx);

  for (int i = 0; i < rows1; ++i) {
    for (int j = 0; j < cols1; ++j) {
      m.matrix[i][j] = get_rand(DBL_MIN, DBL_MAX) + 1;
    }
  }
  for (int i = 0; i < rows2; ++i) {
    for (int j = 0; j < cols2; ++j) {
      mtx.matrix[i][j] = get_rand(DBL_MIN, DBL_MAX);
    }
  }
  ck_assert_int_eq(s21_eq_matrix(&m, &mtx), FAILURE);
  s21_remove_matrix(&m);
  s21_remove_matrix(&mtx);
}
END_TEST

START_TEST(sum_matrix) {
  const int rows = rand() % 100 + 1;
  const int cols = rand() % 100 + 1;
  matrix_t m = {0};
  s21_create_matrix(rows, cols, &m);
  matrix_t mtx = {0};
  s21_create_matrix(rows, cols, &mtx);
  matrix_t check = {0};
  s21_create_matrix(rows, cols, &check);

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      m.matrix[i][j] = get_rand(DBL_MIN, DBL_MAX);
      mtx.matrix[i][j] = get_rand(DBL_MIN, DBL_MAX);
      check.matrix[i][j] = m.matrix[i][j] + mtx.matrix[i][j];
    }
  }
  matrix_t res = {0};

  ck_assert_int_eq(s21_sum_matrix(&m, &mtx, &res), OK);
  ck_assert_int_eq(s21_eq_matrix(&check, &res), SUCCESS);
  s21_remove_matrix(&m);
  s21_remove_matrix(&mtx);
  s21_remove_matrix(&res);
  s21_remove_matrix(&check);
}
END_TEST

START_TEST(sum_matrix_1) {
  matrix_t A, B, R, R2;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);
  s21_create_matrix(1, 1, &R2);

  A.matrix[0][0] = 10.75;
  B.matrix[0][0] = 4.25;

  R2.matrix[0][0] = 15.0;

  s21_sum_matrix(&A, &B, &R);

  ck_assert(fabs(R.matrix[0][0] - R2.matrix[0][0]) < 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
  s21_remove_matrix(&R2);
}
END_TEST

START_TEST(sum_matrix_2) {
  matrix_t A, B, R, R2;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &B);
  s21_create_matrix(3, 3, &R2);

  A.matrix[0][0] = 1.1;
  A.matrix[0][1] = 2.2;
  A.matrix[0][2] = 3.3;
  A.matrix[1][0] = 4.4;
  A.matrix[1][1] = 5.5;
  A.matrix[1][2] = 6.6;
  A.matrix[2][0] = 7.7;
  A.matrix[2][1] = 8.8;
  A.matrix[2][2] = 9.9;

  B.matrix[0][0] = 9.9;
  B.matrix[0][1] = 8.8;
  B.matrix[0][2] = 7.7;
  B.matrix[1][0] = 6.6;
  B.matrix[1][1] = 5.5;
  B.matrix[1][2] = 4.4;
  B.matrix[2][0] = 3.3;
  B.matrix[2][1] = 2.2;
  B.matrix[2][2] = 1.1;

  R2.matrix[0][0] = 11.0;
  R2.matrix[0][1] = 11.0;
  R2.matrix[0][2] = 11.0;
  R2.matrix[1][0] = 11.0;
  R2.matrix[1][1] = 11.0;
  R2.matrix[1][2] = 11.0;
  R2.matrix[2][0] = 11.0;
  R2.matrix[2][1] = 11.0;
  R2.matrix[2][2] = 11.0;

  s21_sum_matrix(&A, &B, &R);

  ck_assert_int_eq(s21_eq_matrix(&R, &R2), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
  s21_remove_matrix(&R2);
}
END_TEST

START_TEST(sum_incorrect_matrix) {
  matrix_t A, B, R;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(2, 1, &B);

  A.matrix[0][0] = 100.0;
  B.matrix[0][0] = 200.0;

  int res = s21_sum_matrix(&A, &B, &R);

  ck_assert_int_eq(res, ERR_CALC);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(sum_result_null) {
  matrix_t A = {0}, B = {0};
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);

  A.matrix[0][0] = 1.0;
  B.matrix[0][0] = 2.0;

  int status = s21_sum_matrix(&A, &B, NULL);
  ck_assert_int_eq(status, INCORRECT);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(sub_test_1) {
  matrix_t A, B, R, R2;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);
  s21_create_matrix(1, 1, &R2);

  A.matrix[0][0] = 5.0;
  B.matrix[0][0] = 3.5;

  R2.matrix[0][0] = 1.5;

  s21_sub_matrix(&A, &B, &R);

  ck_assert(fabs(R.matrix[0][0] - R2.matrix[0][0]) < 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
  s21_remove_matrix(&R2);
}
END_TEST

START_TEST(sub_test_2) {
  matrix_t A, B, R, R2;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &B);
  s21_create_matrix(3, 3, &R2);

  A.matrix[0][0] = 10.0;
  A.matrix[0][1] = 9.0;
  A.matrix[0][2] = 8.0;
  A.matrix[1][0] = 7.0;
  A.matrix[1][1] = 6.0;
  A.matrix[1][2] = 5.0;
  A.matrix[2][0] = 4.0;
  A.matrix[2][1] = 3.0;
  A.matrix[2][2] = 2.0;

  B.matrix[0][0] = 1.0;
  B.matrix[0][1] = 2.0;
  B.matrix[0][2] = 3.0;
  B.matrix[1][0] = 4.0;
  B.matrix[1][1] = 5.0;
  B.matrix[1][2] = 6.0;
  B.matrix[2][0] = 7.0;
  B.matrix[2][1] = 8.0;
  B.matrix[2][2] = 9.0;

  R2.matrix[0][0] = 9.0;
  R2.matrix[0][1] = 7.0;
  R2.matrix[0][2] = 5.0;
  R2.matrix[1][0] = 3.0;
  R2.matrix[1][1] = 1.0;
  R2.matrix[1][2] = -1.0;
  R2.matrix[2][0] = -3.0;
  R2.matrix[2][1] = -5.0;
  R2.matrix[2][2] = -7.0;

  s21_sub_matrix(&A, &B, &R);

  ck_assert_int_eq(s21_eq_matrix(&R, &R2), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
  s21_remove_matrix(&R2);
}
END_TEST

START_TEST(sub_test_incorrect_matrix) {
  matrix_t A, B, R;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(2, 1, &B);

  A.matrix[0][0] = 11.0;
  B.matrix[0][0] = 22.0;

  int res = s21_sub_matrix(&A, &B, &R);

  ck_assert_int_eq(res, ERR_CALC);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(sub_null) {
  matrix_t *A = NULL;
  matrix_t *B = NULL;
  matrix_t *R = NULL;
  int res = s21_sub_matrix(A, B, R);
  ck_assert_int_eq(res, INCORRECT);
}
END_TEST
START_TEST(mult_number_matrix) {
  const int rows = rand() % 50 + 1;
  const int cols = rand() % 50 + 1;
  matrix_t m = {0};
  s21_create_matrix(rows, cols, &m);
  matrix_t check = {0};
  s21_create_matrix(rows, cols, &check);
  double mult_number = get_rand(-1e3, 1e3);

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      m.matrix[i][j] = get_rand(-100.0, 100.0);
      check.matrix[i][j] = m.matrix[i][j] * mult_number;
    }
  }

  matrix_t res = {0};
  ck_assert_int_eq(s21_mult_number(&m, mult_number, &res), OK);
  ck_assert_int_eq(s21_eq_matrix(&check, &res), SUCCESS);
  s21_remove_matrix(&m);
  s21_remove_matrix(&res);
  s21_remove_matrix(&check);
}
END_TEST

START_TEST(mult_number_test_1) {
  matrix_t A, R, R2;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &R2);

  A.matrix[0][0] = 3.5;
  R2.matrix[0][0] = -10.5;

  s21_mult_number(&A, -3, &R);

  ck_assert(fabs(R.matrix[0][0] - R2.matrix[0][0]) < 1e-7);
  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
  s21_remove_matrix(&R2);
}
END_TEST

START_TEST(mult_number_test_2) {
  matrix_t A, R, R2;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &R2);

  A.matrix[0][0] = 0.5;
  A.matrix[0][1] = 1.5;
  A.matrix[0][2] = 2.5;
  A.matrix[1][0] = 3.5;
  A.matrix[1][1] = 4.5;
  A.matrix[1][2] = 5.5;
  A.matrix[2][0] = 6.5;
  A.matrix[2][1] = 7.5;
  A.matrix[2][2] = 8.5;

  // Умножаем на 3
  R2.matrix[0][0] = 1.5;
  R2.matrix[0][1] = 4.5;
  R2.matrix[0][2] = 7.5;
  R2.matrix[1][0] = 10.5;
  R2.matrix[1][1] = 13.5;
  R2.matrix[1][2] = 16.5;
  R2.matrix[2][0] = 19.5;
  R2.matrix[2][1] = 22.5;
  R2.matrix[2][2] = 25.5;

  s21_mult_number(&A, 3, &R);

  ck_assert_int_eq(s21_eq_matrix(&R, &R2), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
  s21_remove_matrix(&R2);
}
END_TEST

START_TEST(mult_number_test_3) {
  matrix_t A, R, R2;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(2, 3, &R2);

  A.matrix[0][0] = 0.5;
  A.matrix[0][1] = 1.5;
  A.matrix[0][2] = 2.5;
  A.matrix[1][0] = 3.5;
  A.matrix[1][1] = 4.5;
  A.matrix[1][2] = 5.5;

  R2.matrix[0][0] = 1.0;
  R2.matrix[0][1] = 3.0;
  R2.matrix[0][2] = 5.0;
  R2.matrix[1][0] = 7.0;
  R2.matrix[1][1] = 9.0;
  R2.matrix[1][2] = 11.0;

  s21_mult_number(&A, 2, &R);

  ck_assert_int_eq(s21_eq_matrix(&R, &R2), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
  s21_remove_matrix(&R2);
}
END_TEST

START_TEST(null_mult_num) {
  matrix_t *B = NULL;
  matrix_t *R = NULL;
  double num = 42.0;
  int res = s21_mult_number(B, num, R);
  ck_assert_int_eq(res, INCORRECT);
}
END_TEST

START_TEST(mult_matrix) {
  const int rows = rand() % 100 + 1;
  const int cols = rand() % 100 + 1;
  matrix_t m = {0};
  s21_create_matrix(rows, cols, &m);
  matrix_t mtx = {0};
  s21_create_matrix(cols, rows, &mtx);

  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j) m.matrix[i][j] = get_rand(-10e10, 10e10);

  for (int i = 0; i < cols; ++i)
    for (int j = 0; j < rows; ++j) mtx.matrix[i][j] = get_rand(-10e10, 10e10);

  matrix_t check = {0};
  s21_create_matrix(m.rows, mtx.columns, &check);

  for (int i = 0; i < m.rows; ++i) {
    for (int j = 0; j < mtx.columns; ++j) {
      check.matrix[i][j] = 0;
      for (int k = 0; k < m.columns; ++k) {
        check.matrix[i][j] += m.matrix[i][k] * mtx.matrix[k][j];
      }
    }
  }

  matrix_t res = {0};
  ck_assert_double_eq(s21_mult_matrix(&m, &mtx, &res), OK);
  ck_assert_int_eq(s21_eq_matrix(&check, &res), SUCCESS);

  s21_remove_matrix(&m);
  s21_remove_matrix(&mtx);
  s21_remove_matrix(&res);
  s21_remove_matrix(&check);
}
END_TEST

START_TEST(mult_incorrect_matrix_1) {
  matrix_t m1 = {0};
  matrix_t m2 = {0};
  s21_create_matrix(5, 5, &m1);
  s21_create_matrix(6, 6, &m2);

  matrix_t result = {0};
  int mul_code = s21_mult_matrix(&m1, &m2, &result);
  ck_assert_int_eq(mul_code, ERR_CALC);

  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(mult_incorrect_matrix_2) {
  matrix_t m1 = {0};
  matrix_t m2 = {0};
  s21_create_matrix(5, 5, &m1);
  s21_create_matrix(6, 6, &m2);
  m1.columns = -100;

  matrix_t result = {0};
  int mul_code = s21_mult_matrix(&m1, &m2, &result);
  ck_assert_int_eq(mul_code, ERR_CALC);

  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(mult_matrix_test_1) {
  matrix_t A, B, R, R2;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);
  s21_create_matrix(1, 1, &R2);

  A.matrix[0][0] = -1;
  B.matrix[0][0] = 2;

  R2.matrix[0][0] = -2;

  s21_mult_matrix(&A, &B, &R);

  ck_assert(fabs(R.matrix[0][0] - R2.matrix[0][0]) < 1e-7);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
  s21_remove_matrix(&R2);
}
END_TEST

START_TEST(mult_matrix_test_2) {
  matrix_t A, B, R, R2;
  s21_create_matrix(3, 2, &A);
  s21_create_matrix(2, 3, &B);
  s21_create_matrix(3, 3, &R2);

  A.matrix[0][0] = 0;
  A.matrix[0][1] = 1;
  A.matrix[1][0] = 3;
  A.matrix[1][1] = 4;
  A.matrix[2][0] = 6;
  A.matrix[2][1] = 7;

  B.matrix[0][0] = 9;
  B.matrix[0][1] = 8;
  B.matrix[0][2] = 7;
  B.matrix[1][0] = 6;
  B.matrix[1][1] = 5;
  B.matrix[1][2] = 4;

  R2.matrix[0][0] = 6;
  R2.matrix[0][1] = 5;
  R2.matrix[0][2] = 4;
  R2.matrix[1][0] = 51;
  R2.matrix[1][1] = 44;
  R2.matrix[1][2] = 37;
  R2.matrix[2][0] = 96;
  R2.matrix[2][1] = 83;
  R2.matrix[2][2] = 70;

  s21_mult_matrix(&A, &B, &R);

  ck_assert_int_eq(s21_eq_matrix(&R, &R2), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
  s21_remove_matrix(&R2);
}
END_TEST

START_TEST(mult_matrix_test_null) {
  matrix_t *A = NULL, *B = NULL, *R = NULL;
  ck_assert_int_eq(s21_mult_matrix(A, B, R), INCORRECT);
}
END_TEST

START_TEST(transpose_test_1) {
  matrix_t A, B, R;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);

  A.matrix[0][0] = -7.5;
  B.matrix[0][0] = -7.5;

  s21_transpose(&A, &R);
  ck_assert_int_eq(s21_eq_matrix(&R, &B), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(transpose_test_2) {
  matrix_t A, B, R;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &B);

  A.matrix[0][0] = 10.1;
  A.matrix[0][1] = 20.2;
  A.matrix[0][2] = 30.3;
  A.matrix[1][0] = 40.4;
  A.matrix[1][1] = 50.5;
  A.matrix[1][2] = 60.6;
  A.matrix[2][0] = 70.7;
  A.matrix[2][1] = 80.8;
  A.matrix[2][2] = 90.9;

  s21_transpose(&A, &R);

  B.matrix[0][0] = 10.1;
  B.matrix[1][0] = 20.2;
  B.matrix[2][0] = 30.3;
  B.matrix[0][1] = 40.4;
  B.matrix[1][1] = 50.5;
  B.matrix[2][1] = 60.6;
  B.matrix[0][2] = 70.7;
  B.matrix[1][2] = 80.8;
  B.matrix[2][2] = 90.9;

  ck_assert_int_eq(s21_eq_matrix(&R, &B), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(transpose_incorrect) {
  matrix_t A, B;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);
  A.columns = -1;

  int ret = s21_transpose(&A, &B);
  ck_assert_int_eq(ret, INCORRECT);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);

  ck_assert_int_eq(s21_transpose(NULL, NULL), INCORRECT);
}
END_TEST

START_TEST(transpose_matrix) {
  const int rows = rand() % 100 + 1;
  const int cols = rand() % 100 + 1;
  matrix_t m = {0};
  s21_create_matrix(rows, cols, &m);

  matrix_t check = {0};
  s21_create_matrix(cols, rows, &check);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      double rand_val = get_rand(-10e10, 10e10);
      m.matrix[i][j] = rand_val;
      check.matrix[j][i] = rand_val;
    }
  }

  matrix_t res = {0};
  ck_assert_int_eq(s21_transpose(&m, &res), OK);
  ck_assert_int_eq(s21_eq_matrix(&check, &res), SUCCESS);

  s21_remove_matrix(&m);
  s21_remove_matrix(&res);
  s21_remove_matrix(&check);
}
END_TEST

START_TEST(compliment_test_0) {
  matrix_t B, R;
  s21_create_matrix(1, 1, &B);
  B.matrix[0][0] = 5;
  ck_assert_int_eq(s21_calc_complements(&B, &R), OK);
  ck_assert_int_eq(R.matrix[0][0], 1);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(compliment_test_1) {
  matrix_t A, B, R;
  s21_create_matrix(3, 3, &B);
  s21_create_matrix(3, 3, &A);
  B.matrix[0][0] = 1;
  B.matrix[0][1] = 2;
  B.matrix[0][2] = 3;
  B.matrix[1][0] = 0;
  B.matrix[1][1] = 4;
  B.matrix[1][2] = 2;
  B.matrix[2][0] = 5;
  B.matrix[2][1] = 2;
  B.matrix[2][2] = 1;

  A.matrix[0][0] = 0;
  A.matrix[0][1] = 10;
  A.matrix[0][2] = -20;
  A.matrix[1][0] = 4;
  A.matrix[1][1] = -14;
  A.matrix[1][2] = 8;
  A.matrix[2][0] = -8;
  A.matrix[2][1] = -2;
  A.matrix[2][2] = 4;

  int code = s21_calc_complements(&B, &R);
  ck_assert_int_eq(code, OK);
  ck_assert_int_eq(s21_eq_matrix(&R, &A), SUCCESS);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(compliment_test_2) {
  matrix_t A, B, R;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &B);

  A.matrix[0][0] = 5;
  A.matrix[0][1] = -1;
  A.matrix[0][2] = 1;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 3;
  A.matrix[1][2] = 4;
  A.matrix[2][0] = 1;
  A.matrix[2][1] = 0;
  A.matrix[2][2] = 3;

  s21_calc_complements(&A, &R);

  B.matrix[0][0] = 9;
  B.matrix[0][1] = -2;
  B.matrix[0][2] = -3;
  B.matrix[1][0] = 3;
  B.matrix[1][1] = 14;
  B.matrix[1][2] = -1;
  B.matrix[2][0] = -7;
  B.matrix[2][1] = -18;
  B.matrix[2][2] = 17;

  ck_assert_int_eq(s21_eq_matrix(&R, &B), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(compliment_test_not_square) {
  matrix_t B, R;
  s21_create_matrix(2, 3, &B);

  B.matrix[0][0] = 1;
  B.matrix[0][1] = 2;
  B.matrix[0][2] = 3;
  B.matrix[1][0] = 0;
  B.matrix[1][1] = 4;
  B.matrix[1][2] = 2;

  int code = s21_calc_complements(&B, &R);

  ck_assert_int_eq(code, ERR_CALC);
  s21_remove_matrix(&B);
  // s21_remove_matrix(&R);
}
END_TEST

START_TEST(compliment_test_1x1) {
  matrix_t A, B, R;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);

  A.matrix[0][0] = 5;

  s21_calc_complements(&A, &R);

  B.matrix[0][0] = 1;

  ck_assert_int_eq(s21_eq_matrix(&R, &B), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(null_compliments) {
  matrix_t *B = NULL;
  matrix_t *R = NULL;
  int res = s21_calc_complements(B, R);
  ck_assert_int_eq(res, INCORRECT);
}
END_TEST

START_TEST(s21_calc_compl_1) {
  matrix_t A, B, C;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &C);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 0.0;
  A.matrix[1][1] = 4.0;
  A.matrix[1][2] = 2.0;
  A.matrix[2][0] = 5.0;
  A.matrix[2][1] = 2.0;
  A.matrix[2][2] = 1.0;
  s21_calc_complements(&A, &B);
  C.matrix[0][0] = 0.0;
  C.matrix[0][1] = 10.0;
  C.matrix[0][2] = -20.0;
  C.matrix[1][0] = 4.0;
  C.matrix[1][1] = -14.0;
  C.matrix[1][2] = 8.0;
  C.matrix[2][0] = -8.0;
  C.matrix[2][1] = -2.0;
  C.matrix[2][2] = 4.0;
  int res = s21_eq_matrix(&B, &C);
  ck_assert_int_eq(res, SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&C);
}
END_TEST

START_TEST(s21_calc_compl_2) {
  matrix_t a = {0};
  s21_create_matrix(3, 3, &a);
  matrix_t res = {0};
  s21_create_matrix(3, 3, &res);
  matrix_t m;
  res.matrix[0][0] = 0;
  res.matrix[0][1] = 10;
  res.matrix[0][2] = -20;
  res.matrix[1][0] = 4;
  res.matrix[1][1] = -14;
  res.matrix[1][2] = 8;
  res.matrix[2][0] = -8;
  res.matrix[2][1] = -2;
  res.matrix[2][2] = 4;

  a.matrix[0][0] = 1;
  a.matrix[0][1] = 2;
  a.matrix[0][2] = 3;
  a.matrix[1][0] = 0;
  a.matrix[1][1] = 4;
  a.matrix[1][2] = 2;
  a.matrix[2][0] = 5;
  a.matrix[2][1] = 2;
  a.matrix[2][2] = 1;
  s21_calc_complements(&a, &m);
  ck_assert_int_eq(s21_eq_matrix(&res, &m), SUCCESS);
  s21_remove_matrix(&m);
  s21_remove_matrix(&a);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(empty) {
  matrix_t a = {0};
  matrix_t b = {0};
  ck_assert_int_eq(s21_calc_complements(&a, &b), INCORRECT);
}
END_TEST

START_TEST(size_mismatch) {
  matrix_t a = {0};
  matrix_t b = {0};
  s21_create_matrix(1, 2, &a);
  ck_assert_int_eq(s21_calc_complements(&a, &b), ERR_CALC);
  s21_remove_matrix(&a);
}
END_TEST

START_TEST(complements) {
  const int size = 3;
  matrix_t m = {0};
  s21_create_matrix(size, size, &m);
  m.matrix[0][0] = 1;
  m.matrix[0][1] = 2;
  m.matrix[0][2] = 3;
  m.matrix[1][1] = 4;
  m.matrix[1][2] = 2;
  m.matrix[2][0] = 5;
  m.matrix[2][1] = 2;
  m.matrix[2][2] = 1;

  matrix_t expected = {0};
  s21_create_matrix(size, size, &expected);
  expected.matrix[0][1] = 10;
  expected.matrix[0][2] = -20;
  expected.matrix[1][0] = 4;
  expected.matrix[1][1] = -14;
  expected.matrix[1][2] = 8;
  expected.matrix[2][0] = -8;
  expected.matrix[2][1] = -2;
  expected.matrix[2][2] = 4;

  matrix_t res = {0};
  s21_calc_complements(&m, &res);

  ck_assert_int_eq(s21_eq_matrix(&expected, &res), SUCCESS);
  s21_remove_matrix(&m);
  s21_remove_matrix(&res);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(determinant_test_single_value) {
  matrix_t A;
  double B, R = 3.14;
  s21_create_matrix(1, 1, &A);

  A.matrix[0][0] = 3.14;

  s21_determinant(&A, &B);
  ck_assert((fabs(B) - fabs(R)) < 1e-7);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_test_random_3x3) {
  matrix_t A;
  double B, R = 162.375;
  s21_create_matrix(3, 3, &A);

  A.matrix[0][0] = 2.5;
  A.matrix[0][1] = 1.0;
  A.matrix[0][2] = 0.5;
  A.matrix[1][0] = 4.0;
  A.matrix[1][1] = 2.2;
  A.matrix[1][2] = 3.3;
  A.matrix[2][0] = 1.1;
  A.matrix[2][1] = 4.4;
  A.matrix[2][2] = 5.5;

  s21_determinant(&A, &B);
  ck_assert((fabs(B) - fabs(R)) < 1e-7);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_check_custom_case_1) {
  matrix_t A;
  double B, R = 12;
  s21_create_matrix(3, 3, &A);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 0;
  A.matrix[1][1] = 1;
  A.matrix[1][2] = 4;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = 6;
  A.matrix[2][2] = 0;

  s21_determinant(&A, &B);
  ck_assert((fabs(B) - fabs(R)) < 1e-7);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(inverse_test_fractional_matrix) {
  matrix_t A, B, C;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &C);
  C.matrix[0][0] = 1.0;
  C.matrix[0][1] = 2.0;
  C.matrix[0][2] = 3.0;
  C.matrix[1][0] = 0.0;
  C.matrix[1][1] = 1.0;
  C.matrix[1][2] = 4.0;
  C.matrix[2][0] = 5.0;
  C.matrix[2][1] = 6.0;
  C.matrix[2][2] = 0.0;

  A.matrix[0][0] = -24;
  A.matrix[0][1] = 18;
  A.matrix[0][2] = 5;
  A.matrix[1][0] = 20;
  A.matrix[1][1] = -15;
  A.matrix[1][2] = -4;
  A.matrix[2][0] = -5;
  A.matrix[2][1] = 4;
  A.matrix[2][2] = 1;

  s21_inverse_matrix(&A, &B);

  int res = s21_eq_matrix(&B, &C);
  ck_assert_int_eq(res, SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&C);
}
END_TEST

START_TEST(inverse_test_classic_case) {
  matrix_t A, C;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &C);

  C.matrix[0][0] = 1.0;
  C.matrix[0][1] = 0.0;
  C.matrix[0][2] = 0.0;
  C.matrix[1][0] = 0.0;
  C.matrix[1][1] = 1.0;
  C.matrix[1][2] = 0.0;
  C.matrix[2][0] = 0.0;
  C.matrix[2][1] = 0.0;
  C.matrix[2][2] = 1.0;

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 0.0;
  A.matrix[0][2] = 0.0;
  A.matrix[1][0] = 0.0;
  A.matrix[1][1] = 1.0;
  A.matrix[1][2] = 0.0;
  A.matrix[2][0] = 0.0;
  A.matrix[2][1] = 0.0;
  A.matrix[2][2] = 1.0;

  matrix_t B;
  s21_inverse_matrix(&A, &B);
  int res = s21_eq_matrix(&B, &C);
  ck_assert_int_eq(res, SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&C);
}
END_TEST

START_TEST(determinant_check_2x2_case) {
  matrix_t A;
  double B, R = -10;
  s21_create_matrix(2, 2, &A);

  A.matrix[0][0] = 3;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 5;
  A.matrix[1][1] = 4;

  s21_determinant(&A, &B);

  ck_assert((fabs(B) - fabs(R)) < 1e-7);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_mixed_signs_3x3) {
  matrix_t A;
  double B, R = -60;
  s21_create_matrix(3, 3, &A);

  A.matrix[0][0] = 2;
  A.matrix[0][1] = 3;
  A.matrix[0][2] = 1;
  A.matrix[1][0] = 1;
  A.matrix[1][1] = 0;
  A.matrix[1][2] = 4;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = 2;
  A.matrix[2][2] = -1;

  s21_determinant(&A, &B);

  ck_assert((fabs(B) - fabs(R)) < 1e-7);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_fail_on_rectangular_matrix) {
  matrix_t A;
  double B;
  s21_create_matrix(3, 4, &A);
  int res = s21_determinant(&A, &B);
  ck_assert_int_eq(res, ERR_CALC);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_null_input) {
  matrix_t *B = NULL;
  double re = 0;
  int res = s21_determinant(B, &re);
  ck_assert_int_eq(res, INCORRECT);
}
END_TEST

START_TEST(determinant_zero_rows_case) {
  const int size = 5;
  matrix_t m = {0};
  s21_create_matrix(size, size, &m);

  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++) m.matrix[i][j] = i;

  double res = 0;
  int code = s21_determinant(&m, &res);
  ck_assert_double_eq_tol(res, 0, 1e-6);
  ck_assert_int_eq(code, OK);

  s21_remove_matrix(&m);
}
END_TEST

START_TEST(determinant_symmetric_case) {
  const int size = 4;
  matrix_t m = {0};
  s21_create_matrix(size, size, &m);

  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++) m.matrix[i][j] = i + j;

  double res = 0;
  int code = s21_determinant(&m, &res);
  ck_assert_double_eq_tol(res, 0, 1e-6);
  ck_assert_int_eq(code, OK);

  s21_remove_matrix(&m);
}
END_TEST

START_TEST(determinant_regular_3x3_case) {
  const int size = 3;
  matrix_t m = {0};
  s21_create_matrix(size, size, &m);

  m.matrix[0][0] = 3;
  m.matrix[0][1] = 0;
  m.matrix[0][2] = 2;
  m.matrix[1][0] = 2;
  m.matrix[1][1] = 0;
  m.matrix[1][2] = -2;
  m.matrix[2][0] = 0;
  m.matrix[2][1] = 1;
  m.matrix[2][2] = 1;

  double res = 0;
  int code = s21_determinant(&m, &res);
  ck_assert_double_eq_tol(res, 10, 1e-6);
  ck_assert_int_eq(code, OK);

  s21_remove_matrix(&m);
}
END_TEST

START_TEST(determinant_basic_2x2) {
  const int size = 2;
  matrix_t m = {0};
  s21_create_matrix(size, size, &m);

  m.matrix[0][0] = 4;
  m.matrix[0][1] = 3;
  m.matrix[1][0] = 6;
  m.matrix[1][1] = 3;

  double res = 0;
  int code = s21_determinant(&m, &res);
  ck_assert_double_eq_tol(res, -6, 1e-6);
  ck_assert_int_eq(code, OK);

  s21_remove_matrix(&m);
}
END_TEST

START_TEST(inverse_typical_3x3_case) {
  matrix_t m = {0}, expected = {0};
  s21_create_matrix(3, 3, &m);
  s21_create_matrix(3, 3, &expected);

  m.matrix[0][0] = 2;
  m.matrix[0][1] = 5;
  m.matrix[0][2] = 7;
  m.matrix[1][0] = 6;
  m.matrix[1][1] = 3;
  m.matrix[1][2] = 4;
  m.matrix[2][0] = 5;
  m.matrix[2][1] = -2;
  m.matrix[2][2] = -3;

  expected.matrix[0][0] = 1;
  expected.matrix[0][1] = -1;
  expected.matrix[0][2] = 1;
  expected.matrix[1][0] = -38;
  expected.matrix[1][1] = 41;
  expected.matrix[1][2] = -34;
  expected.matrix[2][0] = 27;
  expected.matrix[2][1] = -29;
  expected.matrix[2][2] = 24;

  matrix_t result = {0};
  int code = s21_inverse_matrix(&m, &result);

  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);
  ck_assert_int_eq(code, OK);

  s21_remove_matrix(&m);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(inverse_fail_not_square) {
  matrix_t m = {0}, result = {0};
  s21_create_matrix(2, 3, &m);
  int code = s21_inverse_matrix(&m, &result);
  ck_assert_int_eq(code, ERR_CALC);
  s21_remove_matrix(&m);
}
END_TEST

START_TEST(inverse_1x1_case) {
  matrix_t m = {0}, result = {0};
  s21_create_matrix(1, 1, &m);

  m.matrix[0][0] = 8.0;
  int code = s21_inverse_matrix(&m, &result);
  ck_assert_double_eq_tol(result.matrix[0][0], 1.0 / 8.0, 1e-6);
  ck_assert_int_eq(code, OK);

  s21_remove_matrix(&m);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(inverse_fail_zero_determinant) {
  matrix_t m = {0}, result = {0};
  s21_create_matrix(2, 2, &m);

  m.matrix[0][0] = 1;
  m.matrix[0][1] = 2;
  m.matrix[1][0] = 2;
  m.matrix[1][1] = 4;

  int code = s21_inverse_matrix(&m, &result);
  ck_assert_int_eq(code, ERR_CALC);
  s21_remove_matrix(&m);
}
END_TEST

START_TEST(inverse_null_input_test) {
  matrix_t m = {0}, result = {0};
  int code = s21_inverse_matrix(&m, &result);
  ck_assert_int_eq(code, INCORRECT);
}
END_TEST

Suite *s21_matrix_test(void) {
  Suite *suite = suite_create("s21_matrix");
  TCase *tCase = tcase_create("s21_matrix");

  tcase_add_test(tCase, create_matrix);
  tcase_add_test(tCase, create_no_rows);
  tcase_add_test(tCase, create_no_cols);
  tcase_add_test(tCase, create_incorrect);
  tcase_add_test(tCase, create_null_result);
  tcase_add_test(tCase, eq);
  tcase_add_test(tCase, not_eq_1);
  tcase_add_test(tCase, not_eq_2);
  tcase_add_test(tCase, sum_matrix);
  tcase_add_test(tCase, sum_matrix_1);
  tcase_add_test(tCase, sum_matrix_2);
  tcase_add_test(tCase, sum_incorrect_matrix);
  tcase_add_test(tCase, sum_result_null);
  tcase_add_test(tCase, sub_test_1);
  tcase_add_test(tCase, sub_test_2);
  tcase_add_test(tCase, sub_test_incorrect_matrix);
  tcase_add_test(tCase, sub_null);
  tcase_add_test(tCase, mult_number_matrix);
  tcase_add_test(tCase, mult_number_test_1);
  tcase_add_test(tCase, mult_number_test_2);
  tcase_add_test(tCase, mult_number_test_3);
  tcase_add_test(tCase, null_mult_num);
  tcase_add_test(tCase, mult_matrix);
  tcase_add_test(tCase, mult_incorrect_matrix_1);
  tcase_add_test(tCase, mult_incorrect_matrix_2);
  tcase_add_test(tCase, mult_matrix_test_1);
  tcase_add_test(tCase, mult_matrix_test_2);
  tcase_add_test(tCase, mult_matrix_test_null);
  tcase_add_test(tCase, transpose_test_1);
  tcase_add_test(tCase, transpose_test_2);
  tcase_add_test(tCase, transpose_incorrect);
  tcase_add_test(tCase, transpose_matrix);
  tcase_add_test(tCase, compliment_test_0);
  tcase_add_test(tCase, compliment_test_1);
  tcase_add_test(tCase, compliment_test_2);
  tcase_add_test(tCase, compliment_test_not_square);
  tcase_add_test(tCase, compliment_test_1x1);
  tcase_add_test(tCase, null_compliments);
  tcase_add_test(tCase, s21_calc_compl_2);
  tcase_add_test(tCase, s21_calc_compl_1);
  tcase_add_test(tCase, complements);
  tcase_add_test(tCase, empty);
  tcase_add_test(tCase, size_mismatch);
  tcase_add_test(tCase, determinant_test_single_value);
  tcase_add_test(tCase, determinant_test_random_3x3);
  tcase_add_test(tCase, determinant_check_2x2_case);
  tcase_add_test(tCase, determinant_check_custom_case_1);
  tcase_add_test(tCase, determinant_mixed_signs_3x3);
  tcase_add_test(tCase, determinant_fail_on_rectangular_matrix);
  tcase_add_test(tCase, determinant_null_input);
  tcase_add_test(tCase, inverse_test_fractional_matrix);
  tcase_add_test(tCase, inverse_test_classic_case);
  tcase_add_test(tCase, determinant_zero_rows_case);
  tcase_add_test(tCase, determinant_symmetric_case);
  tcase_add_test(tCase, determinant_regular_3x3_case);
  tcase_add_test(tCase, determinant_basic_2x2);
  tcase_add_test(tCase, inverse_typical_3x3_case);
  tcase_add_test(tCase, inverse_fail_not_square);
  tcase_add_test(tCase, inverse_1x1_case);
  tcase_add_test(tCase, inverse_fail_zero_determinant);
  tcase_add_test(tCase, inverse_null_input_test);

  suite_add_tcase(suite, tCase);

  return suite;
}

int main(void) {
  int count_failed;
  Suite *suite = s21_matrix_test();
  SRunner *sRunner = srunner_create(suite);
  srunner_set_fork_status(sRunner, CK_NOFORK);
  srunner_run_all(sRunner, CK_VERBOSE);
  count_failed = srunner_ntests_failed(sRunner);
  srunner_free(sRunner);
  return (count_failed == 0) ? 0 : 1;
}