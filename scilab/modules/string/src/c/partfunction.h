/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
#ifndef __PARTFUNCTION_H__
#define __PARTFUNCTION_H__

#include "dynlib_string.h"

/**
* part : extraction of strings
* @param[in] stringInput : matrix of strings
* @param[in] m : col of stringInput
* @param[in] n : row of stringInput
* @param[in] vectInput : row vector
* @param[in] row : row of vectInput
* @return matrix of strings (same size as stringInput)
*/
STRING_IMPEXP char **partfunction(char** stringInput, int m, int n, int *vectInput, int row);

STRING_IMPEXP wchar_t **partfunctionW(wchar_t** _pwstStringInput, int _iRows, int _iCols, int *_piVectInput, int _iVectSize);

#endif /* __PARTFUNCTION_H__ */
/*---------------------------------------------------------------------------*/
