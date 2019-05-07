/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
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

#ifndef __MATRIX_DIV_H__
#define __MATRIX_DIV_H__

#include "basic_functions.h"
#include "doublecomplex.h"
#include "matrix_transpose.h"
#include "matrix_left_division.h"
#include "matrix_right_division.h"

void matrixIsTriangular(double *_pdblReal, double *_pdblImg, int _iRows, int _iCols, int *bUpper, int *bLower);
extern double C2F(dasum) (int *, double *, int *);

#endif //!__MATRIX_DIV_H__
