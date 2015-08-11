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

//Scalar ^ Scalar
int iPowerRealScalarByRealScalar(
    double _dblReal1,
    double _dblReal2,
    double* _pdblRealOut, double* _pdblImgOut, int *_piComplex);

int iPowerRealScalarByComplexScalar(
    double _dblReal1,
    double _dblReal2, double _dblImg2,
    double* _pdblRealOut, double* _pdblImgOut);

int iPowerComplexScalarByRealScalar(
    double _dblReal1, double _dblImg1,
    double _dblReal2,
    double* _pdblRealOut, double* _pdblImgOut);

int iPowerComplexScalarByComplexScalar(
    double _dblReal1, double _dblImg1,
    double _dblReal2, double _dblImg2, double* _pdblRealOut, double* _pdblImgOut);

//Scalar ^ Matrix
int iPowerRealScalarByRealMatrix(
    double _dblReal1,
    double* _pdblReal2, int _iRows2, int _iCols2,
    double* _pdblRealOut,	double* _pdblImgOut, int *_iComplex);

int iPowerComplexScalarByRealMatrix(
    double _dblReal1, double _dblImg1,
    double* _pdblReal2, int _iRows2, int _iCols2,
    double* _pdblRealOut,	double* _pdblImgOut);

int iPowerRealScalarByComplexMatrix(
    double _dblReal1,
    double* _pdblReal2, double* _pdblImg2, int _iRows2, int _iCols2,
    double* _pdblRealOut,	double* _pdblImgOut);

int iPowerComplexScalarByComplexMatrix(
    double _dblReal1, double _dblImg1,
    double* _pdblReal2, double* _pdblImg2, int _iRows2, int _iCols2,
    double* _pdblRealOut,	double* _pdblImgOut);

//Square Matrix ^ Scalar
int iPowerRealSquareMatrixByRealScalar(
    double* _pdblReal1, int _iRows1, int _iCols1,
    double _dblReal2,
    double* _pdblRealOut,	double* _pdblImgOut, int *_iComplex);

int iPowerRealSquareMatrixByComplexScalar(
    double* _pdblReal1, int _iRows1, int _iCols1,
    double _dblReal2, double _dblImg2,
    double* _pdblRealOut,	double* _pdblImgOut);

int iPowerComplexSquareMatrixByRealScalar(
    double* _pdblReal1, double* _pdblImg1, int _iRows1, int _iCols1,
    double _dblReal2,
    double* _pdblRealOut,	double* _pdblImgOut);

int iPowerComplexSquareMatrixByComplexScalar(
    double* _pdblReal1, double* _pdblImg1, int _iRows1, int _iCols1,
    double _dblReal2, double _dblImg2,
    double* _pdblRealOut,	double* _pdblImgOut);

//Matrix ^ Matrix
int iPowerRealMatrixByRealMatrix(
    double* _pdblReal1, int _iRows1, int _iCols1,
    double* _pdblReal2,
    double* _pdblRealOut,	double* _pdblImgOut);

int iPowerRealMatrixByComplexMatrix(
    double* _pdblReal1, int _iRows1, int _iCols1,
    double* _pdblReal2, double* _pdblImg2,
    double* _pdblRealOut,	double* _pdblImgOut);

int iPowerComplexMatrixByRealMatrix(
    double* _pdblReal1, double* _pdblImg1, int _iRows1, int _iCols1,
    double* _pdblReal2,
    double* _pdblRealOut,	double* _pdblImgOut);

int iPowerComplexMatrixByComplexMatrix(
    double* _pdblReal1, double* _pdblImg1, int _iRows1, int _iCols1,
    double* _pdblReal2, double* _pdblImg2,
    double* _pdblRealOut,	double* _pdblImgOut);



#endif /* __MATRIC_POWER_H__ */
