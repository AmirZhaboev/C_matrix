#include "s21_matrix.h"

void s21_just_mult(matrix_t *A, matrix_t *B, matrix_t *result) {
  for (int i = 0; i < result->rows; i++) {
    for (int j = 0; j < result->columns; j++) {
      double sum = 0.0;
      for (int k = 0; k < A->columns; k++) {
        sum += A->matrix[i][k] * B->matrix[k][j];
      }
      result->matrix[i][j] = sum;
    }
  }
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int exit_status = INCORRECT;

  if (A != NULL && B != NULL) {
    if (A->columns == B->rows) {
      exit_status = s21_create_matrix(A->rows, B->columns, result);
      if (exit_status == OK) {
        s21_just_mult(A, B, result);
      }
    } else {
      exit_status = ERR_CALC;
    }
  }

  return exit_status;
}
