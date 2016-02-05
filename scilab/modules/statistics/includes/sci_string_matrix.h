/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent LIARD
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#ifndef _STRING_MATRIX_
#define _STRING_MATRIX_

#include "dynlib_statistics.h"
/**
 * End-developer oriented string matrix
 */
struct string_matrix
{
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
struct string_matrix *create_string_matrix(void* pvApiCtx, int arg);

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
STATISTICS_IMPEXP char *create_string(void* pvApiCtx, int arg);

/**
 * Free string
 * @param[in] str string to be freed
 */
STATISTICS_IMPEXP void destroy_string(char *str);

#endif /* _STRING_MATRIX_ */
