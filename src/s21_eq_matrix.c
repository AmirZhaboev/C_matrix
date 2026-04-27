#include "s21_matrix.h"

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  if (!A || !B || A->rows != B->rows || A->columns != B->columns) {
    return FAILURE;
  }

  int result = SUCCESS;

  for (int i = 0; i < A->rows && result == SUCCESS; i++) {
    for (int j = 0; j < A->columns; j++) {
      if (fabs(A->matrix[i][j] - B->matrix[i][j]) > EPSILON) {
        result = FAILURE;
        break;
      }
    }
  }

  return result;
}
