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

#include "core_math.h"
#include "matrix_substraction.h"
#include "operations_tools.h"
#include "string.h"

//Matrix - eye
int iSubstractRealIdentityToRealMatrix(double _dblReal1, double* _pdblReal2, int _iRows2, int _iCols2, double* _pdblRealOut)
{
    int i = 0;
    memcpy(_pdblRealOut, _pdblReal2, sizeof(double) * _iRows2 * _iCols2);
    for (i = 0 ; i < Min(_iRows2, _iCols2) ; i++)
    {
        _pdblRealOut[i * _iRows2 + i]	-= _dblReal1;
    }
    return 0;
}

int iSubstractRealIdentityToComplexMatrix(double _dblReal1, double* _pdblReal2, double* _pdblImg2, int _iRows2, int _iCols2, double* _pdblRealOut, double* _pdblImgOut)
{
    int i = 0;
    memcpy(_pdblRealOut, _pdblReal2, sizeof(double) * _iRows2 * _iCols2);
    memcpy(_pdblImgOut,		_pdblImg2	, sizeof(double) * _iRows2 * _iCols2);
    for (i = 0 ; i < Min(_iRows2, _iCols2) ; i++)
    {
        _pdblRealOut[i * _iRows2 + i]	-= _dblReal1;
    }
    return 0;
}

int iSubstractComplexIdentityToRealMatrix(double _dblReal1, double _dblImg1, double* _pdblReal2, int _iRows2, int _iCols2, double* _pdblRealOut, double* _pdblImgOut)
{
    int i = 0;
    memcpy(_pdblRealOut, _pdblReal2, sizeof(double) * _iRows2 * _iCols2);
    memset(_pdblImgOut,		0x00, sizeof(double) * _iRows2 * _iCols2);
    for (i = 0 ; i < Min(_iRows2, _iCols2) ; i++)
    {
        _pdblRealOut[i * _iRows2 + i]	-= _dblReal1;
        _pdblImgOut[i * _iRows2 + i]	-= _dblImg1;
    }
    return 0;
}

int iSubstractComplexIdentityToComplexMatrix(double _dblReal1, double _dblImg1, double* _pdblReal2, double* _pdblImg2, int _iRows2, int _iCols2, double* _pdblRealOut, double* _pdblImgOut)
{
    int i = 0;
    memcpy(_pdblRealOut,	_pdblReal2, sizeof(double) * _iRows2 * _iCols2);
    memcpy(_pdblImgOut,		_pdblImg2	, sizeof(double) * _iRows2 * _iCols2);
    for (i = 0 ; i < Min(_iRows2, _iCols2) ; i++)
    {
        _pdblRealOut[i * _iRows2 + i]	-= _dblReal1;
        _pdblImgOut[i * _iRows2 + i]	-= _dblImg1;
    }
    return 0;
}

//Scalar - Matrix
int iSubstractRealMatrixToRealScalar(double* _pdblReal1, int* _piDims1, int _iDims1, double _dblReal2, double* _pdblRealOut)
{
    int i = 0;
    int iSize1 = GetSize(_piDims1, _iDims1);
    for (i = 0 ; i < iSize1 ; i++)
    {
        _pdblRealOut[i] = _dblReal2 - _pdblReal1[i];
    }
    return 0;
}

int iSubstractComplexMatrixToRealScalar(double* _pdblReal1, double* _pdblImg1, int* _piDims1, int _iDims1, double _dblReal2, double* _pdblRealOut, double* _pdblImgOut)
{
    int i = 0;
    int iSize1 = GetSize(_piDims1, _iDims1);
    for (i = 0 ; i < iSize1 ; i++)
    {
        _pdblRealOut[i] = _dblReal2 - _pdblReal1[i];
        _pdblImgOut[i]	= - _pdblImg1[i];
    }
    return 0;
}

int iSubstractRealMatrixToComplexScalar(double* _pdblReal1, int* _piDims1, int _iDims1, double _dblReal2, double _dblImg2, double* _pdblRealOut, double* _pdblImgOut)
{
    int i = 0;
    int iSize1 = GetSize(_piDims1, _iDims1);
    for (i = 0 ; i < iSize1 ; i++)
    {
        _pdblRealOut[i] = _dblReal2 - _pdblReal1[i];
        _pdblImgOut[i]	= _dblImg2;
    }
    return 0;
}

int iSubstractComplexMatrixToComplexScalar(double* _pdblReal1, double* _pdblImg1, int* _piDims1, int _iDims1, double _dblReal2, double _dblImg2, double* _pdblRealOut, double* _pdblImgOut)
{
    int i = 0;
    int iSize1 = GetSize(_piDims1, _iDims1);
    for (i = 0 ; i < iSize1 ; i++)
    {
        _pdblRealOut[i] = _dblReal2 - _pdblReal1[i];
        _pdblImgOut[i]	= _dblImg2  - _pdblImg1[i];
    }
    return 0;
}

//Matrix - Scalar
int iSubstractRealScalarToRealMatrix(double _dblReal1, double* _pdblReal2, int* _piDims2, int _iDims2, double* _pdblRealOut)
{
    int i = 0;
    int iSize2 = GetSize(_piDims2, _iDims2);
    for (i = 0 ; i < iSize2 ; i++)
    {
        _pdblRealOut[i] = _pdblReal2[i] - _dblReal1;
    }
    return 0;
}

int iSubstractComplexScalarToRealMatrix(double _dblReal1, double _dblImg1, double* _pdblReal2, int* _piDims2, int _iDims2, double* _pdblRealOut, double* _pdblImgOut)
{
    int i = 0;
    int iSize2 = GetSize(_piDims2, _iDims2);
    for (i = 0 ; i < iSize2 ; i++)
    {
        _pdblRealOut[i] = _pdblReal2[i] - _dblReal1;
        _pdblImgOut[i]	= - _dblImg1;
    }
    return 0;
}

int iSubstractRealScalarToComplexMatrix(double _dblReal1, double* _pdblReal2, double* _pdblImg2, int* _piDims2, int _iDims2, double* _pdblRealOut, double* _pdblImgOut)
{
    int i = 0;
    int iSize2 = GetSize(_piDims2, _iDims2);
    for (i = 0 ; i < iSize2 ; i++)
    {
        _pdblRealOut[i] = _pdblReal2[i] - _dblReal1;
        _pdblImgOut[i]	= _pdblImg2[i];
    }
    return 0;
}

int iSubstractComplexScalarToComplexMatrix(double _dblReal1, double _dblImg1, double* _pdblReal2, double* _pdblImg2, int* _piDims2, int _iDims2, double* _pdblRealOut, double* _pdblImgOut)
{
    int i = 0;
    int iSize2 = GetSize(_piDims2, _iDims2);
    for (i = 0 ; i < iSize2 ; i++)
    {
        _pdblRealOut[i] = _pdblReal2[i] - _dblReal1;
        _pdblImgOut[i]	= _pdblImg2[i] - _dblImg1;
    }
    return 0;
}


//Matrix - Matrix
int iSubstractRealMatrixToRealMatrix(double* _pdblReal1, double* _pdblReal2, int* _piDims, int _iDims, double* _pdblRealOut)
{
    int i = 0;
    int iSize = GetSize(_piDims, _iDims);
    for (i = 0 ; i < iSize ; i++)
    {
        _pdblRealOut[i] = _pdblReal2[i] - _pdblReal1[i];
    }
    return 0;
}

int iSubstractComplexMatrixToRealMatrix(double* _pdblReal1, double* _pdblImg1, double* _pdblReal2, int* _piDims, int _iDims, double* _pdblRealOut, double* _pdblImgOut)
{
    int i = 0;
    int iSize = GetSize(_piDims, _iDims);
    for (i = 0 ; i < iSize ; i++)
    {
        _pdblRealOut[i] = _pdblReal2[i] - _pdblReal1[i];
        _pdblImgOut[i]	= - _pdblImg1[i];
    }
    return 0;
}

int iSubstractRealMatrixToComplexMatrix(double* _pdblReal1, double* _pdblReal2, double* _pdblImg2, int* _piDims, int _iDims, double* _pdblRealOut, double* _pdblImgOut)
{
    int i = 0;
    int iSize = GetSize(_piDims, _iDims);
    for (i = 0 ; i < iSize ; i++)
    {
        _pdblRealOut[i] = _pdblReal2[i] - _pdblReal1[i];
        _pdblImgOut[i]	= _pdblImg2[i];
    }
    return 0;
}

int iSubstractComplexMatrixToComplexMatrix(double* _pdblReal1, double* _pdblImg1, double* _pdblReal2, double* _pdblImg2, int* _piDims, int _iDims, double* _pdblRealOut, double* _pdblImgOut)
{
    int i = 0;
    int iSize = GetSize(_piDims, _iDims);
    for (i = 0 ; i < iSize ; i++)
    {
        _pdblRealOut[i] = _pdblReal2[i] - _pdblReal1[i];
        _pdblImgOut[i]	= _pdblImg2[i] - _pdblImg1[i];
    }
    return 0;
}


