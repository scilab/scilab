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

#ifndef __MATRIC_POWER_H__
#define __MATRIC_POWER_H__

#include "operation.hxx"

//Scalar ^ Scalar
EXTERN_OP int iPowerRealScalarByRealScalar(
		double _dblReal1, 
		double _dblReal2, 
		double* _pdblRealOut, double* _pdblImgOut, int *_piComplex);

EXTERN_OP int iPowerRealScalarByComplexScalar(
		double _dblReal1, 
		double _dblReal2, double _dblImg2, 
		double* _pdblRealOut, double* _pdblImgOut);

EXTERN_OP int iPowerComplexScalarByRealScalar(
		double _dblReal1, double _dblImg1, 
		double _dblReal2, 
		double* _pdblRealOut, double* _pdblImgOut);

EXTERN_OP int iPowerComplexScalarByComplexScalar(
		double _dblReal1, double _dblImg1, 
		double _dblReal2, double _dblImg2, double* _pdblRealOut, double* _pdblImgOut);

//Scalar ^ Matrix
EXTERN_OP int iPowerRealScalarByRealMatrix(
		double _dblReal1,
		double* _pdblReal2, int _iRows2, int _iCols2,
		double* _pdblRealOut,	double* _pdblImgOut, int *_iComplex);

EXTERN_OP int iPowerComplexScalarByRealMatrix(
		double _dblReal1, double _dblImg1,
		double* _pdblReal2, int _iRows2, int _iCols2,
		double* _pdblRealOut,	double* _pdblImgOut);

EXTERN_OP int iPowerRealScalarByComplexMatrix(
		double _dblReal1,
		double* _pdblReal2, double* _pdblImg2, int _iRows2, int _iCols2,
		double* _pdblRealOut,	double* _pdblImgOut);

EXTERN_OP int iPowerComplexScalarByComplexMatrix(
		double _dblReal1, double _dblImg1,
		double* _pdblReal2, double* _pdblImg2, int _iRows2, int _iCols2,
		double* _pdblRealOut,	double* _pdblImgOut);

//Square Matrix ^ Scalar 
EXTERN_OP int iPowerRealSquareMatrixByRealScalar(
		double* _pdblReal1, int _iRows1, int _iCols1,
		double _dblReal2,
		double* _pdblRealOut,	double* _pdblImgOut, int *_iComplex);

EXTERN_OP int iPowerRealSquareMatrixByComplexScalar(
		double* _pdblReal1, int _iRows1, int _iCols1,
		double _dblReal2, double _dblImg2,
		double* _pdblRealOut,	double* _pdblImgOut);

EXTERN_OP int iPowerComplexSquareMatrixByRealScalar(
		double* _pdblReal1, double* _pdblImg1, int _iRows1, int _iCols1,
		double _dblReal2,
		double* _pdblRealOut,	double* _pdblImgOut);

EXTERN_OP int iPowerComplexSquareMatrixByComplexScalar(
		double* _pdblReal1, double* _pdblImg1, int _iRows1, int _iCols1,
		double _dblReal2, double _dblImg2,
		double* _pdblRealOut,	double* _pdblImgOut);

//Matrix ^ Matrix
EXTERN_OP int iPowerRealMatrixByRealMatrix(
		double* _pdblReal1, int _iRows1, int _iCols1,
		double* _pdblReal2,
		double* _pdblRealOut,	double* _pdblImgOut);

EXTERN_OP int iPowerRealMatrixByComplexMatrix(
		double* _pdblReal1, int _iRows1, int _iCols1,
		double* _pdblReal2, double* _pdblImg2, 
		double* _pdblRealOut,	double* _pdblImgOut);

EXTERN_OP int iPowerComplexMatrixByRealMatrix(
		double* _pdblReal1, double* _pdblImg1, int _iRows1, int _iCols1,
		double* _pdblReal2, 
		double* _pdblRealOut,	double* _pdblImgOut);

EXTERN_OP int iPowerComplexMatrixByComplexMatrix(
		double* _pdblReal1, double* _pdblImg1, int _iRows1, int _iCols1,
		double* _pdblReal2, double* _pdblImg2, 
		double* _pdblRealOut,	double* _pdblImgOut);



#endif /* __MATRIC_POWER_H__ */
