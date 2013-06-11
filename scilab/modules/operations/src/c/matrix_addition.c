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

#include <stdio.h>
#include "matrix_addition.h"
#include "core_math.h"
#include "operations_tools.h"
#include <string.h>

int iAddRealIdentityToRealMatrix(double _dblReal1, double* _pdblReal2, int _iRows2, int _iCols2, double* _pdblRealOut)
{
    int i = 0;
    memcpy(_pdblRealOut, _pdblReal2, sizeof(double) * _iRows2 * _iCols2);
    for (i = 0 ; i < Min(_iRows2, _iCols2) ; i++)
    {
        _pdblRealOut[i * _iRows2 + i]	+= _dblReal1;
    }
    return 0;
}

int iAddRealIdentityToComplexMatrix(double _dblReal1, double* _pdblReal2, double* _pdblImg2, int _iRows2, int _iCols2, double* _pdblRealOut, double* _pdblImgOut)
{
    int i = 0;
    memcpy(_pdblRealOut,	_pdblReal2, sizeof(double) * _iRows2 * _iCols2);
    memcpy(_pdblImgOut,		_pdblImg2	, sizeof(double) * _iRows2 * _iCols2);
    for (i = 0 ; i < Min(_iRows2, _iCols2) ; i++)
    {
        _pdblRealOut[i * _iRows2 + i]	+= _dblReal1;
    }
    return 0;
}

int iAddComplexIdentityToRealMatrix(double _dblReal1, double _dblImg1, double* _pdblReal2, int _iRows2, int _iCols2, double* _pdblRealOut, double* _pdblImgOut)
{
    int i = 0;
    memcpy(_pdblRealOut,	_pdblReal2, sizeof(double) * _iRows2 * _iCols2);
    memset(_pdblImgOut,		0x00, sizeof(double) * _iRows2 * _iCols2);
    for (i = 0 ; i < Min(_iRows2, _iCols2) ; i++)
    {
        _pdblRealOut[i * _iRows2 + i]	+= _dblReal1;
        _pdblImgOut[i * _iRows2 + i]	+= _dblImg1;
    }
    return 0;
}

int iAddComplexIdentityToComplexMatrix(double _dblReal1, double _dblImg1, double* _pdblReal2, double* _pdblImg2, int _iRows2, int _iCols2, double* _pdblRealOut, double* _pdblImgOut)
{
    int i = 0;
    memcpy(_pdblRealOut,	_pdblReal2, sizeof(double) * _iRows2 * _iCols2);
    memcpy(_pdblImgOut,		_pdblImg2	, sizeof(double) * _iRows2 * _iCols2);
    for (i = 0 ; i < Min(_iRows2, _iCols2) ; i++)
    {
        _pdblRealOut[i * _iRows2 + i]	+= _dblReal1;
        _pdblImgOut[i * _iRows2 + i]	+= _dblImg1;
    }
    return 0;
}

int iAddRealScalarToRealMatrix(double _dblReal1, double* _pdblReal2, int* _piDims2, int _iDims2, double* _pdblRealOut)
{
    int i = 0;
    int iSize2 = GetSize(_piDims2, _iDims2);
    for (i = 0 ; i < iSize2 ; i++)
    {
        _pdblRealOut[i]	= _dblReal1	+ _pdblReal2[i];
    }
    return 0;
}

int iAddRealScalarToComplexMatrix(double _dblReal1, double* _pdblReal2, double* _pdblImg2, int* _piDims2, int _iDims2, double* _pdblRealOut, double *_pdblImgOut)
{
    iAddRealScalarToRealMatrix(_dblReal1, _pdblReal2, _piDims2, _iDims2, _pdblRealOut);
    iAddRealScalarToRealMatrix(0, _pdblImg2, _piDims2, _iDims2, _pdblImgOut);
    return 0;
}

int iAddComplexScalarToRealMatrix(double _dblReal1, double _dblImg1, double* _pdblReal2, int* _piDims2, int _iDims2, double* _pdblRealOut, double *_pdblImgOut)
{
    int i = 0;
    int iSize2 = GetSize(_piDims2, _iDims2);

    iAddRealScalarToRealMatrix(_dblReal1, _pdblReal2, _piDims2, _iDims2, _pdblRealOut);
    for (i = 0 ; i < iSize2 ; i++)
    {
        _pdblImgOut[i]	= _dblImg1;
    }
    return 0;
}

int iAddComplexScalarToComplexMatrix(double _dblReal1, double _dblImg1, double* _pdblReal2, double* _pdblImg2, int* _piDims2, int _iDims2, double* _pdblRealOut, double *_pdblImgOut)
{
    iAddRealScalarToRealMatrix(_dblReal1, _pdblReal2, _piDims2, _iDims2, _pdblRealOut);
    iAddRealScalarToRealMatrix(_dblImg1, _pdblImg2, _piDims2, _iDims2, _pdblImgOut);
    return 0;
}

int iAddRealMatrixToRealMatrix(double* _pdblReal1, double* _pdblReal2, int* _piDims, int _iDims, double* _pdblRealOut)
{
    int i = 0;
    int iSize = GetSize(_piDims, _iDims);
    for (i = 0 ; i < iSize ; i++)
    {
        _pdblRealOut[i]	= _pdblReal1[i]	+ _pdblReal2[i];
    }
    return 0;
}

int iAddRealMatrixToComplexMatrix(double* _pdblReal1, double* _pdblReal2, double* _pdblImg2, int* _piDims, int _iDims, double* _pdblRealOut, double* _pdblImgOut)
{
    iAddRealMatrixToRealMatrix(_pdblReal1, _pdblReal2, _piDims, _iDims, _pdblRealOut);
    iAddRealScalarToRealMatrix(0, _pdblImg2, _piDims, _iDims, _pdblImgOut);
    return 0;
}

int iAddComplexMatrixToComplexMatrix(double* _pdblReal1, double* _pdblImg1, double* _pdblReal2, double* _pdblImg2, int* _piDims, int _iDims, double* _pdblRealOut, double* _pdblImgOut)
{
    iAddRealMatrixToRealMatrix(_pdblReal1, _pdblReal2, _piDims, _iDims, _pdblRealOut);
    iAddRealMatrixToRealMatrix(_pdblImg1, _pdblImg2, _piDims, _iDims, _pdblImgOut);
    return 0;
}

/* SinglePoly + SinglePoly*/
int iAddRealPolyToRealPoly(double* _pCoef1R, int _iRank1, double* _pCoef2R, int _iRank2, double* _pCoefOutR, int _iRanOut)
{
    int iRankMin			= Min(_iRank1, _iRank2);
    int iRankMax			= Max(_iRank1, _iRank2);
    int iRank					= 0;
    double* pCoefMaxR	= (_iRank1 > _iRank2) ? _pCoef1R : _pCoef2R;

    for (iRank = 0; iRank < iRankMin ; iRank++)
    {
        _pCoefOutR[iRank] = _pCoef1R[iRank] + _pCoef2R[iRank];
    }

    for (iRank = iRankMin ; iRank < iRankMax ; iRank++)
    {
        _pCoefOutR[iRank] = pCoefMaxR[iRank];
    }
    return 0;
}

int iAddRealPolyToComplexPoly(double* _pCoef1R, int _iRank1, double* _pCoef2R, double* _pCoef2I, int _iRank2, double* _pCoefOutR, double* _pCoefOutI, int _iRanOut)
{
    int iRankMin			= Min(_iRank1, _iRank2);
    int iRankMax			= Max(_iRank1, _iRank2);
    int iRank					= 0;
    double* pCoefMaxR	= (_iRank1 > _iRank2) ? _pCoef1R : _pCoef2R;
    double* pCoefMaxI	= NULL;

    for (iRank = 0; iRank < iRankMin ; iRank++)
    {
        _pCoefOutR[iRank] = _pCoef1R[iRank] + _pCoef2R[iRank];
        _pCoefOutI[iRank] = _pCoef2I[iRank];
    }

    if (_iRank1 > _iRank2)
    {
        for (iRank = iRankMin ; iRank < iRankMax ; iRank++)
        {
            _pCoefOutR[iRank] = pCoefMaxR[iRank];
            _pCoefOutI[iRank] = 0;
        }
    }
    else
    {
        for (iRank = iRankMin ; iRank < iRankMax ; iRank++)
        {
            _pCoefOutR[iRank] = pCoefMaxR[iRank];
            _pCoefOutI[iRank] = _pCoef2I[iRank];
        }
    }
    return 0;
}

int iAddComplexPolyToComplexPoly(double* _pCoef1R, double* _pCoef1I, int _iRank1, double* _pCoef2R, double* _pCoef2I, int _iRank2, double* _pCoefOutR, double* _pCoefOutI, int _iRanOut)
{
    int iRankMin			= Min(_iRank1, _iRank2);
    int iRankMax			= Max(_iRank1, _iRank2);
    int iRank					= 0;
    double* pCoefMaxR	= (_iRank1 > _iRank2) ? _pCoef1R : _pCoef2R;
    double* pCoefMaxI	= (_iRank1 > _iRank2) ? _pCoef1I : _pCoef2I;

    for (iRank = 0; iRank < iRankMin ; iRank++)
    {
        _pCoefOutR[iRank] = _pCoef1R[iRank] + _pCoef2R[iRank];
        _pCoefOutI[iRank] = _pCoef1I[iRank] + _pCoef2I[iRank];
    }

    for (iRank = iRankMin ; iRank < iRankMax ; iRank++)
    {
        _pCoefOutR[iRank] = pCoefMaxR[iRank];
        _pCoefOutI[iRank] = pCoefMaxI[iRank];
    }
    return 0;
}
