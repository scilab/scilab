/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent LIARD
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

#include "sci_string_matrix.h"
#include "api_scilab.h"
#include "MALLOC.h"

struct string_matrix* create_string_matrix(int arg)
{
  struct string_matrix *mat = NULL;
  int *lengths = NULL;
  int *varaddr = NULL;
  int size;
  int i;

  mat = (struct string_matrix *) MALLOC(sizeof(struct string_matrix));
  getVarAddressFromPosition(pvApiCtx, arg, &varaddr);
  getMatrixOfString(pvApiCtx, varaddr, &mat->rows, &mat->cols, NULL, NULL);
  size = mat->rows * mat->cols;
  lengths = (int *) MALLOC(size * sizeof(int));
  mat->strings = (char **) MALLOC((size * sizeof(char *)));
  getMatrixOfString(pvApiCtx, varaddr, &mat->rows, &mat->cols, lengths, NULL);
  for (i = 0; i != size; ++i)
    mat->strings[i] = (char *)MALLOC((lengths[i] + 1) * sizeof(char));
  getMatrixOfString(pvApiCtx, varaddr, &mat->rows, &mat->cols, lengths, mat->strings);
  FREE(lengths);
  return mat;
}

void
destroy_string_matrix(struct string_matrix *mat)
{
  int i;

  for (i = 0; i != mat->rows * mat->cols; ++i) {
    FREE(mat->strings[i]);
    mat->strings[i] = NULL;
  }
  FREE(mat->strings);
  mat->strings = NULL;
  FREE(mat);
  mat = NULL;
}

char *
create_string(int arg)
{
  int rows, cols;
  char **strings;
  char *str;
  int *lengths = NULL;
  int *varaddr = NULL;

  getVarAddressFromPosition(pvApiCtx, arg, &varaddr);
  getMatrixOfString(pvApiCtx, varaddr, &rows, &cols, NULL, NULL);
  if (rows != 1 || cols != 1)
    return NULL; /* not a mere string */
  lengths = (int *) MALLOC(sizeof(int));
  strings = (char **) MALLOC(sizeof(char *));
  getMatrixOfString(pvApiCtx, varaddr, &rows, &cols, lengths, NULL);
  strings[0] = (char *)MALLOC((lengths[0] + 1) * sizeof(char));
  getMatrixOfString(pvApiCtx, varaddr, &rows, &cols, lengths, strings);
  str = *strings;
  FREE(lengths);
  FREE(strings);
  return str;
}

void
destroy_string(char *str)
{
  FREE(str);
}
