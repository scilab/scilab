/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __MATRIX_LDIV__
#define __MATRIX_LDIV__

#include "operation.hxx"
#include "basic_functions.h"
#include "matrix_transpose.h"

EXTERN_OP int	iLeftDivisionOfRealMatrix(
			double *_pdblReal1,		int _iRows1,	int _iCols1,
			double *_pdblReal2,		int _iRows2,	int _iCols2,
			double *_pdblRealOut,	int _iRowsOut,	int _iColsOut, double *_pdblRcond);

EXTERN_OP int	iLeftDivisionOfComplexMatrix(
			double *_pdblReal1,		double *_pdblImg1,		int _iRows1,	int _iCols1,
			double *_pdblReal2,		double *_pdblImg2,		int _iRows2,	int _iCols2,
			double *_pdblRealOut,	double *_pdblImgOut,	int _iRowsOut,	int _iColsOut, double *_pdblRcond);


#endif /* __MATRIX_LDIV__ */
