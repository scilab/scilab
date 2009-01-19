/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2009 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "matrix_substraction.h"

//Scalar - Matrix
int iSubstractRealMatrixToRealScalar(double* _pdblReal1, int _iRows, int _iCols, double _dblReal2, double* _pdblRealOut)
{
	int i = 0;
	for(i = 0 ; i < _iRows * _iCols ; i++)
	{
		_pdblRealOut[i] = _dblReal2 - _pdblReal1[i];
	}
	return 0;
}

int iSubstractComplexMatrixToRealScalar(double* _pdblReal1, double* _pdblImg1, int _iRows, int _iCols, double _dblReal2, double* _pdblRealOut, double* _pdblImgOut)
{
	int i = 0;
	for(i = 0 ; i < _iRows * _iCols ; i++)
	{
		_pdblRealOut[i] = _dblReal2 - _pdblReal1[i];
		_pdblImgOut[i]	= - _pdblImg1[i];
	}
	return 0;
}

int iSubstractRealMatrixToComplexScalar(double* _pdblReal1, int _iRows, int _iCols, double _dblReal2, double _dblImg2, double* _pdblRealOut, double* _pdblImgOut)
{
	int i = 0;
	for(i = 0 ; i < _iRows * _iCols ; i++)
	{
		_pdblRealOut[i] = _dblReal2 - _pdblReal1[i];
		_pdblImgOut[i]	= _dblImg2;
	}
	return 0;
}

int iSubstractComplexMatrixToComplexScalar(double* _pdblReal1, double* _pdblImg1, int _iRows, int _iCols, double _dblReal2, double _dblImg2, double* _pdblRealOut, double* _pdblImgOut)
{
	int i = 0;
	for(i = 0 ; i < _iRows * _iCols ; i++)
	{
		_pdblRealOut[i] = _dblReal2 - _pdblReal1[i];
		_pdblImgOut[i]	= _dblImg2  - _pdblImg1[i];
	}
	return 0;
}

//Matrix - Scalar
int iSubstractRealScalarToRealMatrix(double _dblReal1, double* _pdblReal2, int _iRows, int _iCols, double* _pdblRealOut)
{
	int i = 0;
	for(i = 0 ; i < _iRows * _iCols ; i++)
	{
		_pdblRealOut[i] = _pdblReal2[i] - _dblReal1;
	}
	return 0;
}

int iSubstractComplexScalarToRealMatrix(double _dblReal1, double _dblImg1, double* _pdblReal2, int _iRows, int _iCols, double* _pdblRealOut, double* _pdblImgOut)
{
	int i = 0;
	for(i = 0 ; i < _iRows * _iCols ; i++)
	{
		_pdblRealOut[i] = _pdblReal2[i] - _dblReal1;
		_pdblImgOut[i]	= - _dblImg1;
	}
	return 0;
}

int iSubstractRealScalarToComplexMatrix(double _dblReal1, double* _pdblReal2, double* _pdblImg2, int _iRows, int _iCols, double* _pdblRealOut, double* _pdblImgOut)
{
	int i = 0;
	for(i = 0 ; i < _iRows * _iCols ; i++)
	{
		_pdblRealOut[i] = _pdblReal2[i] - _dblReal1;
		_pdblImgOut[i]	= _pdblImg2[i];
	}
	return 0;
}

int iSubstractComplexScalarToComplexMatrix(double _dblReal1, double _dblImg1, double* _pdblReal2, double* _pdblImg2, int _iRows, int _iCols, double* _pdblRealOut, double* _pdblImgOut)
{
	int i = 0;
	for(i = 0 ; i < _iRows * _iCols ; i++)
	{
		_pdblRealOut[i] = _pdblReal2[i] - _dblReal1;
		_pdblImgOut[i]	= _pdblImg2[i] - _dblImg1;
	}
	return 0;
}


//Matrix - Matrix
int iSubstractRealMatrixToRealMatrix(double* _pdblReal1, double* _pdblReal2, int _iRows, int _iCols, double* _pdblRealOut)
{
	int i = 0;
	for(i = 0 ; i < _iRows * _iCols ; i++)
	{
		_pdblRealOut[i] = _pdblReal2[i] - _pdblReal1[i];
	}
	return 0;
}

int iSubstractComplexMatrixToRealMatrix(double* _pdblReal1, double* _pdblImg1, double* _pdblReal2, int _iRows, int _iCols, double* _pdblRealOut, double* _pdblImgOut)
{
	int i = 0;
	for(i = 0 ; i < _iRows * _iCols ; i++)
	{
		_pdblRealOut[i] = _pdblReal2[i] - _pdblReal1[i];
		_pdblImgOut[i]	= - _pdblImg1[i];
	}
	return 0;
}

int iSubstractRealMatrixToComplexMatrix(double* _pdblReal1, double* _pdblReal2, double* _pdblImg2, int _iRows, int _iCols, double* _pdblRealOut, double* _pdblImgOut)
{
	int i = 0;
	for(i = 0 ; i < _iRows * _iCols ; i++)
	{
		_pdblRealOut[i] = _pdblReal2[i] - _pdblReal1[i];
		_pdblImgOut[i]	= _pdblImg2[i];
	}
	return 0;
}

int iSubstractComplexMatrixToComplexMatrix(double* _pdblReal1, double* _pdblImg1, double* _pdblReal2, double* _pdblImg2, int _iRows, int _iCols, double* _pdblRealOut, double* _pdblImgOut)
{
	int i = 0;
	for(i = 0 ; i < _iRows * _iCols ; i++)
	{
		_pdblRealOut[i] = _pdblReal2[i] - _pdblReal1[i];
		_pdblImgOut[i]	= _pdblImg2[i] - _pdblImg1[i];
	}
	return 0;
}


