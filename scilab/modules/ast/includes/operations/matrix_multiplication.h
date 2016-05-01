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

#ifndef __MATRIX_MULT__
#define __MATRIX_MULT__

#include "dynlib_ast.h"

EXTERN_AST int iMultiComplexMatrixByComplexMatrix(
    double *_pdblReal1,		double *_pdblImg1, int _iRows1, int _iCols1,
    double *_pdblReal2,		double *_pdblImg2, int _iRows2, int _iCols2,
    double *_pdblRealOut,	double *_pdblImgOut);

EXTERN_AST int iMultiRealMatrixByRealMatrix(
    double *_pdblReal1,	int _iRows1, int _iCols1,
    double *_pdblReal2,	int _iRows2, int _iCols2,
    double *_pdblRealOut);

EXTERN_AST int iMultiRealMatrixByComplexMatrix(
    double *_pdblReal1,		int _iRows1, int _iCols1,
    double *_pdblReal2,		double *_pdblImg2, int _iRows2, int _iCols2,
    double *_pdblRealOut,	double *_pdblImgOut);

EXTERN_AST int iMultiComplexMatrixByRealMatrix(
    double *_pdblReal1,		double *_pdblImg1,	int _iRows1, int _iCols1,
    double *_pdblReal2,		int _iRows2, int _iCols2,
    double *_pdblRealOut,	double *_pdblImgOut);

EXTERN_AST int iMultiRealScalarByRealMatrix(
    double _dblReal1,
    double *_pdblReal2,	int _iRows2, int _iCols2,
    double *_pdblRealOut);

EXTERN_AST int iMultiRealScalarByComplexMatrix(
    double _dblReal1,
    double *_pdblReal2,	double *_pdblImg2, int _iRows2, int _iCols2,
    double *_pdblRealOut, double *_pdblImgOut);

EXTERN_AST int iMultiComplexScalarByRealMatrix(
    double _dblReal1,		double _dblImg1,
    double *_pdblReal2,		int _iRows2, int _iCols2,
    double *_pdblRealOut,	double *_pdblImgOut);

EXTERN_AST int iMultiComplexScalarByComplexMatrix(
    double _dblReal1,		double _dblImg1,
    double *_pdblReal2,		double *_pdblImg2, int _iRows2, int _iCols2,
    double *_pdblRealOut,	double *_pdblImgOut);


/*polynomial multiplications*/
EXTERN_AST int iMultiScilabPolynomByScilabPolynom(
    double *_pdblReal1,	int _iRank1,
    double *_pdblReal2,	int _iRank2,
    double *_pdblRealOut, int _iRankOut);

EXTERN_AST int iMultiComplexPolyByScilabPolynom(
    double *_pdblReal1,	double *_pdblImg1, int _iRank1,
    double *_pdblReal2,	int _iRank2,
    double *_pdblRealOut, double *_pdblImgOut, int _iRankOut);

EXTERN_AST int iMultiScilabPolynomByComplexPoly(
    double *_pdblReal1,	int _iRank1,
    double *_pdblReal2,	double *_pdblImg2, int _iRank2,
    double *_pdblRealOut, double *_pdblImgOut, int _iRankOut);

EXTERN_AST int iMultiComplexPolyByComplexPoly(
    double *_pdblReal1,	double *_pdblImg1, int _iRank1,
    double *_pdblReal2,	double *_pdblImg2, int _iRank2,
    double *_pdblRealOut, double *_pdblImgOut, int _iRankOut);

EXTERN_AST int iDotMultiplyRealMatrixByRealMatrix(
    double* _pdblReal1,
    double* _pdblReal2,
    double* _pdblRealOut, int _iRowsOut, int _iColsOut);

EXTERN_AST int iDotMultiplyRealMatrixByComplexMatrix(
    double* _pdblReal1,
    double* _pdblReal2, double* _pdblImg2,
    double* _pdblRealOut, double* _pdblImgOut, int _iRowsOut, int _iColsOut);

EXTERN_AST int iDotMultiplyComplexMatrixByRealMatrix(
    double* _pdblReal1, double* _pdblImg1,
    double* _pdblReal2,
    double* _pdblRealOut, double* _pdblImgOut, int _iRowsOut, int _iColsOut);

EXTERN_AST int iDotMultiplyComplexMatrixByComplexMatrix(
    double* _pdblReal1, double* _pdblImg1,
    double* _pdblReal2, double* _pdblImg2,
    double* _pdblRealOut, double* _pdblImgOut, int _iRowsOut, int _iColsOut);

#endif /* __MATRIX_MULT__ */
