#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  if (rows <= 0 || columns <= 0 || result == NULL) {
    return INCORRECT;
  }

  int status = OK;
  result->rows = rows;
  result->columns = columns;
  result->matrix = calloc(rows, sizeof(double *));

  if (result->matrix == NULL) {
    status = ERR_CALC;
  } else {
    for (int i = 0; i < rows; i++) {
      result->matrix[i] = calloc(columns, sizeof(double));
      if (result->matrix[i] == NULL) {
        s21_remove_matrix(result);
        status = ERR_CALC;
        break;
      }
    }
  }

  return status;
}
