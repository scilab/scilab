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

#include <string.h> // for memset fonction
#include "elem_common.h"
#include "matrix_multiplication.h"

int iMultiComplexMatrixByComplexMatrix(
		double *_pdblReal1,		double *_pdblImg1,	int _iRows1, int _iCols1,
		double *_pdblReal2,		double *_pdblImg2,	int _iRows2, int _iCols2,
		double *_pdblRealOut,	double *_pdblImgOut)
{
	double dblOne		= 1;
	double dblMinusOne	= -1;
	double dblZero		= 0;

	int iRowsOut	= _iRows1;

	//Cr <-  1*Ar*Br + 0*Cr
	F2C(dgemm)("N","N", &iRowsOut, &_iCols2, &_iCols1, &dblOne		, _pdblReal1	, &_iRows1, _pdblReal2	, &_iRows2, &dblZero	, _pdblRealOut	, &iRowsOut);
	//Cr <- -1*Ai*Bi + 1*Cr
	F2C(dgemm)("N","N", &iRowsOut, &_iCols2, &_iCols1, &dblMinusOne	, _pdblImg1		, &_iRows1, _pdblImg2	, &_iRows2, &dblOne		, _pdblRealOut	, &iRowsOut);
	//Ci <-  1*Ar*Bi + 0*Ci
	F2C(dgemm)("N","N", &iRowsOut, &_iCols2, &_iCols1, &dblOne		, _pdblReal1	, &_iRows1, _pdblImg2	, &_iRows2, &dblZero	, _pdblImgOut	, &iRowsOut);
	//Ci <-  1*Ai*Br + 1*Ci
	F2C(dgemm)("N","N", &iRowsOut, &_iCols2, &_iCols1, &dblOne		, _pdblImg1		, &_iRows1, _pdblReal2	, &_iRows2, &dblOne		, _pdblImgOut	, &iRowsOut);

	return 0;
}

int iMultiRealMatrixByRealMatrix(
		double *_pdblReal1,	int _iRows1, int _iCols1,
		double *_pdblReal2,	int _iRows2, int _iCols2,
		double *_pdblRealOut)
{
	double dblOne		= 1;
	double dblZero		= 0;

	C2F(dgemm)("n", "n", &_iRows1, &_iCols2, &_iCols1, &dblOne,
		_pdblReal1 , &_iRows1 ,
		_pdblReal2, &_iRows2, &dblZero,
		_pdblRealOut ,&_iRows1);
	return 0;
}

int iMultiRealMatrixByComplexMatrix(
		double *_pdblReal1,		int _iRows1, int _iCols1,
		double *_pdblReal2,		double *_pdblImg2, int _iRows2, int _iCols2,
		double *_pdblRealOut,	double *_pdblImgOut)
{
	double dblOne		= 1;
	double dblZero		= 0;
	iMultiRealMatrixByRealMatrix(_pdblReal1, _iRows1, _iCols1, _pdblReal2, _iRows2,  _iCols2, _pdblRealOut);
	C2F(dgemm)("n", "n", &_iRows1, &_iCols2, &_iCols1, &dblOne,
		_pdblReal1 , &_iRows1 ,
		_pdblImg2, &_iRows2, &dblZero,
		_pdblImgOut ,&_iRows1);

	return 0;
}

int iMultiComplexMatrixByRealMatrix(
		double *_pdblReal1,		double *_pdblImg1,	int _iRows1, int _iCols1,
		double *_pdblReal2,							int _iRows2, int _iCols2,
		double *_pdblRealOut,	double *_pdblImgOut)
{
	double dblOne		= 1;
	double dblZero		= 0;
	iMultiRealMatrixByRealMatrix(_pdblReal1, _iRows1, _iCols1, _pdblReal2, _iRows2,  _iCols2, _pdblRealOut);
	C2F(dgemm)("n", "n", &_iRows1, &_iCols2, &_iCols1, &dblOne,
		_pdblImg1 , &_iRows1 ,
		_pdblReal2, &_iRows2, &dblZero,
		_pdblImgOut ,&_iRows1);
	return 0;
}


int iMultiRealScalarByRealMatrix(
		double _dblReal1,
		double *_pdblReal2,	int _iRows2, int _iCols2,
		double *_pdblRealOut)
{
	int iOne	= 1;
	int iSize2	= _iRows2 * _iCols2;

	C2F(dcopy)(&iSize2, _pdblReal2, &iOne, _pdblRealOut, &iOne);
	C2F(dscal)(&iSize2, &_dblReal1, _pdblRealOut, &iOne);
	return 0;
}

int iMultiRealScalarByComplexMatrix(
		double _dblReal1,
		double *_pdblReal2,	double *_pdblImg2, int _iRows2, int _iCols2,
		double *_pdblRealOut, double *_pdblImgOut)
{
	int iOne	= 1;
	int iSize2	= _iRows2 * _iCols2;

	C2F(dcopy)(&iSize2, _pdblReal2, &iOne, _pdblRealOut, &iOne);
	C2F(dcopy)(&iSize2, _pdblImg2, &iOne, _pdblImgOut, &iOne);
	C2F(dscal)(&iSize2, &_dblReal1, _pdblRealOut, &iOne);
	C2F(dscal)(&iSize2, &_dblReal1, _pdblImgOut, &iOne);
	return 0;
}

int iMultiComplexScalarByRealMatrix(
		double _dblReal1,		double _dblImg1,
		double *_pdblReal2,		int _iRows2, int _iCols2,
		double *_pdblRealOut,	double *_pdblImgOut)
{
	int iOne	= 1;
	int iSize2	= _iRows2 * _iCols2;

	C2F(dcopy)(&iSize2, _pdblReal2, &iOne, _pdblRealOut, &iOne);
	C2F(dcopy)(&iSize2, _pdblReal2, &iOne, _pdblImgOut, &iOne);
	C2F(dscal)(&iSize2, &_dblReal1, _pdblRealOut, &iOne);
	C2F(dscal)(&iSize2, &_dblImg1,	_pdblImgOut, &iOne);
	return 0;
}

int iMultiComplexScalarByComplexMatrix(
		double _dblReal1,		double _dblImg1,
		double *_pdblReal2,		double *_pdblImg2, int _iRows2, int _iCols2,
		double *_pdblRealOut,	double *_pdblImgOut)
{
	int iOne	= 1;
	int iSize2	= _iRows2 * _iCols2;
	doublecomplex *pDataIn1 =
		oGetDoubleComplexFromPointer(&_dblReal1, &_dblImg1, 1);
	doublecomplex *pDataIn2 =
		oGetDoubleComplexFromPointer(_pdblReal2,_pdblImg2, _iRows2 * _iCols2);

	C2F(zscal)(&iSize2, pDataIn1, pDataIn2, &iOne);

	vGetPointerFromDoubleComplex(pDataIn2,iSize2, _pdblRealOut, _pdblImgOut);
	vFreeDoubleComplexFromPointer(pDataIn2);
	vFreeDoubleComplexFromPointer(pDataIn1);
	return 0;
}


/* (a1 + a2 * %s + ... ) * (a1 + a2 * %s + ... )*/
EXTERN_OP int iMultiRealPolyByRealPoly(
		double *_pdblReal1,	int _iRank1,
		double *_pdblReal2,	int _iRank2,
		double *_pdblRealOut, int _iRankOut)
{
	int i1		= 0;
	int i2		= 0;

	memset(_pdblRealOut, 0x00, _iRankOut * sizeof(double));
	for(i1 = 0 ; i1 < _iRank1 ; i1++)
	{
		for(i2 = 0 ; i2 < _iRank2 ; i2++)
		{
			_pdblRealOut[i1 + i2] += _pdblReal1[i1] * _pdblReal2[i2];
		}
	}
	return 0;
}

/* ((a1 +ib1) + (a2+ib2) * %s + ... ) (a1 + a2 * %s + ... ) */
EXTERN_OP int iMultiComplexPolyByRealPoly(
		double *_pdblReal1,	double *_pdblImg1, int _iRank1,
		double *_pdblReal2,	int _iRank2,
		double *_pdblRealOut, double *_pdblImgOut, int _iRankOut)
{
	int i1		= 0;
	int i2		= 0;

	memset(_pdblRealOut	, 0x00, _iRankOut * sizeof(double));
	memset(_pdblImgOut	, 0x00, _iRankOut * sizeof(double));
	for(i1 = 0 ; i1 < _iRank1 ; i1++)
	{
		for(i2 = 0 ; i2 < _iRank2 ; i2++)
		{
			_pdblRealOut[i1 + i2] += _pdblReal1[i1] * _pdblReal2[i2];
			_pdblImgOut[i1 + i2]	+= _pdblImg1[i1] * _pdblReal2[i2];
		}
	}
	return 0;
}

/* (a1 + a2 * %s + ... ) * ((a1 +ib1) + (a2+ib2) * %s + ... )*/
EXTERN_OP int iMultiRealPolyByComplexPoly(
		double *_pdblReal1,	int _iRank1,
		double *_pdblReal2,	double *_pdblImg2, int _iRank2,
		double *_pdblRealOut, double *_pdblImgOut, int _iRankOut)
{
	return iMultiComplexPolyByRealPoly(
		_pdblReal2, _pdblImg2, _iRank2,
		_pdblReal1, _iRank1,
		_pdblRealOut, _pdblImgOut, _iRankOut);

}

/* ((a1 +ib1) + (a2+ib2) * %s + ... ) * ((a1 +ib1) + (a2+ib2) * %s + ... ) */
EXTERN_OP int iMultiComplexPolyByComplexPoly(
		double *_pdblReal1,	double *_pdblImg1, int _iRank1,
		double *_pdblReal2,	double *_pdblImg2, int _iRank2,
		double *_pdblRealOut, double *_pdblImgOut, int _iRankOut)
{
	int i1		= 0;
	int i2		= 0;

	memset(_pdblRealOut	, 0x00, _iRankOut * sizeof(double));
	memset(_pdblImgOut	, 0x00, _iRankOut * sizeof(double));
	for(i1 = 0 ; i1 < _iRank1 ; i1++)
	{
		for(i2 = 0 ; i2 < _iRank2 ; i2++)
		{
			_pdblRealOut[i1 + i2] += _pdblReal1[i1] * _pdblReal2[i2]	- _pdblImg1[i1] * _pdblImg2[i2];
			_pdblImgOut[i1 + i2]	+= _pdblImg1[i1] * _pdblReal2[i2]		+ _pdblImg2[i2] * _pdblReal1[i1];
		}
	}
	return 0;
}
