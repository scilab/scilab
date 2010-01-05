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

#ifndef _STRING_MATRIX_
#define _STRING_MATRIX_

#include "dynlib_statistics.h" 
/**
 * End-developer oriented string matrix
 */
struct string_matrix {
  int rows;
  int cols;
  char **strings;
};

/**
 * Retrieve string matrix from scilab function call
 * @warning allocates required memory (to be freed with destroy_string_matrix)
 * @param[in] arg is the index of the argument on the scilab function call line
 * @return pointer on string_matrix structure holding the strings
 */
struct string_matrix *create_string_matrix(int arg);

/**
 * Free string matrix
 * @param[in] pointer on the string_matrix to be freed
 */
void destroy_string_matrix(struct string_matrix *mat);

/**
 * Retrieve a mere string (ie 1x1 matrix of string) from scilab function call
 * @warning allocates required memory (to be freed with destroy_string)
 * @param[in] arg is the index of the argument on the scilab function call line
 * @return pointer on string_matrix structure holding the strings
 */
STATISTICS_IMPEXP char *create_string(int arg);

/**
 * Free string
 * @param[in] str string to be freed
 */
STATISTICS_IMPEXP void destroy_string(char *str);

#endif /* _STRING_MATRIX_ */
