/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2008 - INRIA - Antoine ELIAS <antoine.elias@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __DOUBLE_H__
#define __DOUBLE_H__


#include "basic_functions.h"
#include "stack-c.h"
#include "../../core/src/c/parse.h"
#include <stdio.h>


/*Multiplication*/
int matmult();
int matldiv();
int matrdiv();

extern double C2F(dlange)();
extern C2F(dlacpy)();
extern C2F(dgetrf)();
extern C2F(dgecon)();
extern C2F(dgetrs)();
extern C2F(dgelsy1)();

extern double C2F(zlange)();
extern C2F(zlacpy)();
extern C2F(zgetrf)();
extern C2F(zgecon)();
extern C2F(zgetrs)();
extern C2F(zgelsy1)();


/*Tools to convert memory matrix storage from C to Z and Z to C*/
doublecomplex* oGetDoubleComplexFromPointer(double *_pdblReal, double *_pdblImg, int _iSize);
void vGetPointerFromDoubleComplex(doublecomplex *_poComplex, int _iSize, double *_pdblReal, double *_pdblImg);
void vFreeDoubleComplexFromPointer(doublecomplex *_poComplex);

void vTransposeRealMatrix(
			double *_pdblRealIn, 
			int _iRowsIn,			int _iColsIn, 
			double *_pdblRealOut);

void vTransposeComplexMatrix(
			double *_pdblRealIn,	double *_pdblImgIn, 
			int _iRowsIn,			int _iColsIn, 
			double *_pdblRealOut,	double *_pdblImgOut);

void vTransposeDoubleComplexMatrix(
			doublecomplex *_poIn, 
			int _iRowsIn,			int _iColsIn, 
			doublecomplex *_poOut);

void vPretransposeRealMatrix(
			double *_pdblRealIn, 
			int _iRowsIn,			int _iColsIn, 
			double *_pdblRealOut);

void vPretransposeComplexMatrix(
			double *_pdblRealIn,	double *_pdblImgIn, 
			int _iRowsIn,			int _iColsIn, 
			double *_pdblRealOut,	double *_pdblImgOut);

int	iLeftDivisionOfRealMatrix(
			double *_pdblReal1,		int _iRows1,	int _iCols1,
			double *_pdblReal2,		int _iRows2,	int _iCols2,
			double *_pdblRealOut,	int _iRowsOut,	int _iColsOut);

int	iRightDivisionOfRealMatrix(
			double *_pdblReal1,		int _iRows1,	int _iCols1,
			double *_pdblReal2,		int _iRows2,	int _iCols2,
			double *_pdblRealOut,	int _iRowsOut,	int _iColsOut);

int	iLeftDivisionOfComplexMatrix(
			double *_pdblReal1,		double *_pdblImg1,		int _iRows1,	int _iCols1,
			double *_pdblReal2,		double *_pdblImg2,		int _iRows2,	int _iCols2,
			double *_pdblRealOut,	double *_pdblImgOut,	int _iRowsOut,	int _iColsOut);

int	iRightDivisionOfComplexMatrix(
			double *_pdblReal1,		double *_pdblImg1,		int _iRows1,	int _iCols1,
			double *_pdblReal2,		double *_pdblImg2,		int _iRows2,	int _iCols2,
			double *_pdblRealOut,	double *_pdblImgOut,	int _iRowsOut,	int _iColsOut);

int iRightDivisionRealMatrixByRealMatrix(
			double *_pdblReal1, int _iInc1, 
			double *_pdblReal2, int _iInc2, 
			double *_pdblRealOut, int _iIncOut, int _iSize);

int iRightDivisionComplexByReal(
			double _dblReal1, double _dblImg1, 
			double _dblReal2, 
			double *_pdblRealOut, double *_pdblImgOut);

int iRightDivisionComplexMatrixByRealMatrix(
			double *_pdblReal1,		double *_pdblImg1,		int _iInc1, 
			double *_pdblReal2,								int _iInc2, 
			double *_pdblRealOut,	double *_pdblImgOut,	int _iIncOut, int _iSize);

int iRightDivisionRealByComplex(
			double _dblReal1, 
			double _dblReal2, double _dblImg2, 
			double *_pdblRealOut, double *_pdblImgOut);

int iRightDivisionRealMatrixByComplexMatrix(
			double *_pdblReal1,								int _iInc1, 
			double *_pdblReal2,		double *_pdblImg2,		int _iInc2, 
			double *_pdblRealOut,	double *_pdblImgOut,	int _iIncOut, int _iSize);

int iRightDivisionComplexByComplex(
			double _dblReal1, double _dblImg1, 
			double _dblReal2, double _dblImg2, 
			double *_pdblRealOut, double *_pdblImgOut);

int iRightDivisionComplexMatrixByComplexMatrix(
			double *_pdblReal1,		double *_pdblImg1,		int _iInc1, 
			double *_pdblReal2,		double *_pdblImg2,		int _iInc2, 
			double *_pdblRealOut,	double *_pdblImgOut,	int _iIncOut, int _iSize);

#endif //__DOUBLE_H__
