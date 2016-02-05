/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2011 - DIGITEO - Antoine Elias
*  Copyright (C) 2011 - DIGITEO - Cedric Delamarre
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

#include "matrix_kronecker.h"
#include "elem_common.h"

#include <stdio.h>
// Real .*. Real
void vKronR(double* _pdblDataIn1, int _iIncIn1, int _iRowsIn1, int _iColsIn1,
            double* _pdblDataIn2, int _iIncIn2, int _iRowsIn2, int _iColsIn2,
            double* _pdblDataOut, int _iIncOut)
{
    //int iOne    = 1;
    //int iLoop1  = 0;
    //int iLoop2  = 0;
    //int iLoop3  = 0;
    //int iIndex1 = - _iIncIn1;
    //int iIndex2 = - _iColsIn2;

    //for(iLoop1 = 0; iLoop1 < _iColsIn1; iLoop1++)
    //{
    //    int iIndex3 = 0; //Index a 1 ???
    //    iIndex1     += _iIncIn1;
    //    iIndex2     += _iColsIn2;
    //    for(iLoop2 = 0; iLoop2 < _iColsIn2; iLoop2++)
    //    {
    //        int iIndex4 = iIndex1;
    //        int iIndex5 = (iLoop2 + iIndex2) * _iIncOut;
    //        for(iLoop3 = 0; iLoop3 < _iRowsIn1; iLoop3++)
    //        {
    //            C2F(dcopy)(&_iRowsIn2, &_pdblDataIn2[iIndex3], &iOne, &_pdblDataOut[iIndex5], &iOne);
    //            //ddscals(&_pdblDataOut[iIndex5], _iRowsIn2, _pdblDataIn1[iIndex4], &_pdblDataOut[iIndex5]);
    //            C2F(dscal)(&_iRowsIn2, &_pdblDataIn1[iIndex4], &_pdblDataOut[iIndex5], &iOne);

    //            iIndex5 += _iRowsIn2;
    //            iIndex4++;
    //        }
    //        iIndex3 += _iIncIn2;
    //    }
    //}
    int iSize1 = _iRowsIn1 * _iColsIn1;
    int iSize2 = _iRowsIn2 * _iColsIn2;

    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    int idx = 0;

    for (i = 0 ; i < iSize1 ; i += _iRowsIn1)
    {
        int i2 = i + _iRowsIn1;
        for (j = 0 ; j < iSize2 ; j += _iRowsIn2)
        {
            int j2 = j + _iRowsIn2;
            for (k = i ; k < i2 ; k++)
            {
                //double c = _pdblDataIn1[k];
                for (l = j ; l < j2 ; l++)
                {
                    _pdblDataOut[idx] = _pdblDataIn1[k] * _pdblDataIn2[l];
                    idx++;
                }
            }
        }
    }
}

// Complex .*. Complex
void vKronC(double* _pdblRealIn1, double* _pdblImgIn1, int _iIncIn1, int _iRowsIn1, int _iColsIn1,
            double* _pdblRealIn2, double* _pdblImgIn2, int _iIncIn2, int _iRowsIn2, int _iColsIn2,
            double* _pdblRealOut, double* _pdblImgOut, int _iIncOut)
{
    //int iLoop1 = 0, iLoop2 = 0, iLoop3 = 0, iLoop4 = 0;
    //int iIndex1 = -_iIncIn1;
    //int iIndex2 = -_iColsIn2;
    //for(iLoop1 = 0; iLoop1 < _iColsIn1; iLoop1++)
    //{
    //    int iIndex3 = 0;
    //    iIndex1     += _iIncIn1;
    //    iIndex2     += _iColsIn2;
    //    for(iLoop2 = 0; iLoop2 < _iColsIn2; iLoop2++)
    //    {
    //        int iIndex4 = iIndex1;
    //        int iIndex5 = (iLoop2 + iIndex2) * _iIncOut;
    //        for(iLoop3 = 0; iLoop3 < _iRowsIn1; iLoop3++)
    //        {
    //            for(iLoop4 = 0; iLoop4 < _iRowsIn2; iLoop4++)
    //            {
    //                _pdblRealOut[iIndex5 + iLoop4] =
    //                    _pdblRealIn1[iIndex4] * _pdblRealIn2[iIndex3 + iLoop4] -
    //                    _pdblImgIn1[iIndex4] * _pdblImgIn2[iIndex3 + iLoop4];
    //                _pdblImgOut[iIndex5 + iLoop4] =
    //                    _pdblRealIn1[iIndex4] * _pdblImgIn2[iIndex3 + iLoop4] +
    //                    _pdblImgIn1[iIndex4] * _pdblRealIn2[iIndex3 + iLoop4];
    //            }
    //            iIndex5 += _iRowsIn2;
    //            iIndex4++;
    //        }
    //        iIndex3 += _iIncIn2;
    //    }
    //}
    int iSize1 = _iRowsIn1 * _iColsIn1;
    int iSize2 = _iRowsIn2 * _iColsIn2;

    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    int idx = 0;

    for (i = 0 ; i < iSize1 ; i += _iRowsIn1)
    {
        int i2 = i + _iRowsIn1;
        for (j = 0 ; j < iSize2 ; j += _iRowsIn2)
        {
            int j2 = j + _iRowsIn2;
            for (k = i ; k < i2 ; k++)
            {
                //double c = _pdblDataIn1[k];
                for (l = j ; l < j2 ; l++)
                {
                    _pdblRealOut[idx] = _pdblRealIn1[k] * _pdblRealIn2[l] - _pdblImgIn1[k] * _pdblImgIn2[l];
                    _pdblImgOut[idx] = _pdblRealIn1[k] * _pdblImgIn2[l] + _pdblImgIn1[k] * _pdblRealIn2[l];
                    idx++;
                }
            }
        }
    }
}

// convert : a => 1 ./ a
int conv_real_input(double* _pdblData, int _iSize)
{
    int i;
    for (i = 0 ; i < _iSize ; i++)
    {
        if (_pdblData[i] != 0)
        {
            _pdblData[i] = 1.0 / _pdblData[i];
        }
        else
        {
            return 1;
        }
    }
    return 0;
}

int conv_img_input(double* _pdblReal, double* _pdblImg, int _iSize)
{
    int i;
    for (i = 0 ; i < _iSize ; i++)
    {
        double dblR = _pdblReal[i];
        double dblI = _pdblImg[i];

        double dblTemp	= dblR * dblR + dblI * dblI;
        if (dblTemp != 0)
        {
            _pdblReal[i] = _pdblReal[i]	/ dblTemp;
            _pdblImg[i] = - _pdblImg[i] / dblTemp;
        }
        else
        {
            return 1;
        }
    }
    return 0;
}

