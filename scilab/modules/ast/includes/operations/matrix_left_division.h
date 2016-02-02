/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
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

#ifndef __MATRIX_LDIV__
#define __MATRIX_LDIV__

#include "dynlib_ast.h"

EXTERN_AST int	iLeftDivisionOfRealMatrix(
    double *_pdblReal1,		int _iRows1,	int _iCols1,
    double *_pdblReal2,		int _iRows2,	int _iCols2,
    double *_pdblRealOut,	int _iRowsOut,	int _iColsOut, double *_pdblRcond);

EXTERN_AST int	iLeftDivisionOfComplexMatrix(
    double *_pdblReal1,		double *_pdblImg1,		int _iRows1,	int _iCols1,
    double *_pdblReal2,		double *_pdblImg2,		int _iRows2,	int _iCols2,
    double *_pdblRealOut,	double *_pdblImgOut,	int _iRowsOut,	int _iColsOut, double *_pdblRcond);


#endif /* __MATRIX_LDIV__ */
