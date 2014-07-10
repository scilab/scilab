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

#ifndef __MATRIX_RDIV__
#define __MATRIX_RDIV__

#include "dynlib_ast.h"

EXTERN_AST int	iRightDivisionOfRealMatrix(
    double *_pdblReal1,		int _iRows1,	int _iCols1,
    double *_pdblReal2,		int _iRows2,	int _iCols2,
    double *_pdblRealOut,	int _iRowsOut,	int _iColsOut, double* _pdblRcond);

EXTERN_AST int	iRightDivisionOfComplexMatrix(
    double *_pdblReal1,		double *_pdblImg1,		int _iRows1,	int _iCols1,
    double *_pdblReal2,		double *_pdblImg2,		int _iRows2,	int _iCols2,
    double *_pdblRealOut,	double *_pdblImgOut,	int _iRowsOut,	int _iColsOut, double *_pdblRcond);

EXTERN_AST int iRightDivisionRealMatrixByRealMatrix(
    double *_pdblReal1, int _iInc1,
    double *_pdblReal2, int _iInc2,
    double *_pdblRealOut, int _iIncOut, int _iSize);

EXTERN_AST int iRightDivisionComplexByReal(
    double _dblReal1, double _dblImg1,
    double _dblReal2,
    double *_pdblRealOut, double *_pdblImgOut);

EXTERN_AST int iRightDivisionComplexMatrixByRealMatrix(
    double *_pdblReal1,		double *_pdblImg1,		int _iInc1,
    double *_pdblReal2,								int _iInc2,
    double *_pdblRealOut,	double *_pdblImgOut,	int _iIncOut, int _iSize);

EXTERN_AST int iRightDivisionRealByComplex(
    double _dblReal1,
    double _dblReal2, double _dblImg2,
    double *_pdblRealOut, double *_pdblImgOut);

EXTERN_AST int iRightDivisionRealMatrixByComplexMatrix(
    double *_pdblReal1,								int _iInc1,
    double *_pdblReal2,		double *_pdblImg2,		int _iInc2,
    double *_pdblRealOut,	double *_pdblImgOut,	int _iIncOut, int _iSize);

EXTERN_AST int iRightDivisionComplexByComplex(
    double _dblReal1, double _dblImg1,
    double _dblReal2, double _dblImg2,
    double *_pdblRealOut, double *_pdblImgOut);

EXTERN_AST int iRightDivisionComplexMatrixByComplexMatrix(
    double *_pdblReal1,		double *_pdblImg1,		int _iInc1,
    double *_pdblReal2,		double *_pdblImg2,		int _iInc2,
    double *_pdblRealOut,	double *_pdblImgOut,	int _iIncOut, int _iSize);


#endif /* __MATRIX_RDIV__ */
