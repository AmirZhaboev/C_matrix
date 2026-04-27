#include "s21_matrix.h"

void s21_just_sub(matrix_t *A, matrix_t *B, matrix_t *result) {
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
    }
  }
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (!A || !B || !result) return INCORRECT;

  int status = OK;

  if (A->rows != B->rows || A->columns != B->columns) {
    status = ERR_CALC;
  } else {
    status = s21_create_matrix(A->rows, A->columns, result);
    if (status == OK) {
      s21_just_sub(A, B, result);
    }
  }

  return status;
}
