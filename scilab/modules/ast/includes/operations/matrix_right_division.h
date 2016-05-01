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
