/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Antoine ELIAS
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
#include "basic_functions.h"

/*
purpose
    computes the product of the entries of a matrix according to flag
syntax
    subroutine dmprod(flag,a,na,m,n,v,nv)
    double precision a(na,n),v(*)
    integer na,n,m,nv
    integer flag
parameters
    flag : indicates operation to perform
           0 : returns in v(1) the product of all entries of a
           1 : returns in v(j) the product of jth column of a
           2 : returns in v(i) the product of ith row of a
    a    : array containing the a matrix
    na   : a matrix leading dimension
    m    : a matrix row dimension
    n    : a matrix column dimension
    v    : array containing the result, may be confused with a row or
           a column of the a matrix
           if flag==0 size(v)>=1
           if flag==1 size(v)>=n*nv
           if flag==1 size(v)>=m*nv
    nv   : increment between to consecutive entries ov v
*/
void vDmProd(int _iMode, double* _pdblIn, int _iLeadDim, int _iRows, int _iCols, double* _pdblOut, int _iInc)
{
    int iIndex = 0;
    int iLoop = 0;
    switch (_iMode)
    {
        case 0 :
            _pdblOut[0] = 1;
            vDvmul(_iRows * _iCols, _pdblIn, _pdblOut, 1, 0, _pdblOut);
            break;
        case 1 :
        {
            for (iLoop = 0 ; iLoop < _iCols ; iLoop++)
            {
                _pdblOut[iIndex] = 1;
                vDvmul(_iRows, _pdblIn + _iRows * iLoop, _pdblOut + iIndex, 1, 0, _pdblOut + iIndex);
                iIndex += _iInc;
            }
        }
        break;
        case 2 :
        {
            for (iLoop = 0 ; iLoop < _iRows ; iLoop++)
            {
                _pdblOut[iIndex] = 1;
                vDvmul(_iCols, _pdblIn + iLoop, _pdblOut + iIndex, _iRows, 0, _pdblOut + iIndex);
                iIndex += _iInc;
            }
        }
        break;
    }
}

/*
purpose
    computes the product of the entries of a complex matrix according to flag
syntax
    subroutine wmprod(flag,ar,ai,na,m,n,vr,vi,nv)
    double precision ar(na,n),ai(na,n),vr(*),vi(*)
    integer na,n,m,nv
    integer flag
parameters
    flag : indicates operation to perform
           0 : returns in v(1) the product of all entries of a
           1 : returns in v(j) the product of jth column of a
           2 : returns in v(i) the product of ith row of a
    a    : array containing the a matrix
    na   : a matrix leading dimension
    m    : a matrix row dimension
    n    : a matrix column dimension
    v    : array containing the result,
           vr (resp vi) may be confused with a row or
           a column of the ar (resp ai) matrix
           if flag==0 size(v)>=1
           if flag==1 size(v)>=n*nv
           if flag==1 size(v)>=m*nv
    nv   : increment between to consecutive entries ov v
*/
void vWDmProd(int _iMode, double* _pdblIn1, double* _pdblIn2, int _iLeadDim, int _iRows, int _iCols, double* _pdblOut1, double* _pdblOut2, int _iInc)
{
    int iIndex = 0;
    int iLoop = 0;
    switch (_iMode)
    {
        case 0 :
            _pdblOut1[iIndex] = 1;
            _pdblOut2[iIndex] = 0;
            for (iLoop = 0 ; iLoop < _iCols ; iLoop++)
            {
                vWvmul(_iRows, _pdblIn1 + _iRows * iLoop, _pdblIn2 + _iRows * iLoop, _pdblOut1 + iIndex, _pdblOut2 + iIndex, 1, 0, _pdblOut1 + iIndex, _pdblOut2 + iIndex);
            }
            break;
        case 1 :
            for (iLoop = 0 ; iLoop < _iCols ; iLoop++)
            {
                _pdblOut1[iIndex] = 1;
                _pdblOut2[iIndex] = 0;
                vWvmul(_iRows, _pdblIn1 + _iRows * iLoop, _pdblIn2 + _iRows * iLoop, _pdblOut1 + iIndex, _pdblOut2 + iIndex, 1, 0, _pdblOut1 + iIndex, _pdblOut2 + iIndex);
                iIndex += _iInc;
            }
            break;
        case 2 :
            for (iLoop = 0 ; iLoop < _iRows ; iLoop++)
            {
                _pdblOut1[iIndex] = 1;
                _pdblOut2[iIndex] = 0;
                vWvmul(_iCols, _pdblIn1 + iLoop, _pdblIn2 + iLoop, _pdblOut1 + iIndex, _pdblOut2 + iIndex, _iRows, 0, _pdblOut1 + iIndex, _pdblOut2 + iIndex);
                iIndex += _iInc;
            }
            break;
    }
}
