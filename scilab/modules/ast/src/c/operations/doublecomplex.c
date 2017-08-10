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

#include "elem_common.h"

void vFreeDoubleComplexFromPointer(doublecomplex *_poComplex)
{
    if (_poComplex != NULL)
    {
        free(_poComplex);
    }
}

/*
vGetPointerFromDoubleComplex --
  Set the target real and imaginary part of an array from a source doublecomplex array.
Arguments
  _poComplex : the source array
  _iSize : the number of elements to set
  _pdblReal, _pdblImg : the target array (real and imaginary parts)
*/
void vGetPointerFromDoubleComplex(const doublecomplex *_poComplex, int _iSize, double *_pdblReal, double *_pdblImg)
{
    int iIndex = 0;

    int iTwo	= 2;
    int iOne	= 1;
    double *pReal = (double*)&_poComplex[0].r;
    double *pImg = (double*)&_poComplex[0].i;

    if (_pdblReal != NULL && _pdblImg != NULL)
    {
        C2F(dcopy)(&_iSize, pReal, &iTwo, _pdblReal, &iOne);
        C2F(dcopy)(&_iSize, pImg, &iTwo, _pdblImg, &iOne);
    }
    else if (_pdblReal != NULL && _pdblImg == NULL)
    {
        C2F(dcopy)(&_iSize, pReal, &iTwo, _pdblReal, &iOne);
    }
    else if (_pdblReal == NULL && _pdblImg != NULL)
    {
        C2F(dcopy)(&_iSize, pImg, &iTwo, _pdblImg, &iOne);
    }
}
/*
oGetDoubleComplexFromPointer --
  Returns a target doublecomplex array constructed from the source real and imaginary parts.
  The real and imaginary parts can be NULL or not NULL :
  * if real part and imaginary part of source array are not NULL, the returned array is as expected,
  * if real part of source array is NULL and imaginary part is not NULL, the real part of the returned array is filled with zeros,
  * if real part of source array is not NULL and imaginary part is NULL, the imaginary part of the returned array is filled with zeros,
  * if both real and imaginary parts of source array are NULL, the returned array is NULL.
Arguments
  _pdblReal : the real part of the source array
  _pdblImg : the imaginary part of the source array
  _iSize : the size of the source array
*/
doublecomplex* oGetDoubleComplexFromPointer(double *_pdblReal, double *_pdblImg, int _iSize)
{
    int iIndex = 0;
    doublecomplex *poComplex = (doublecomplex*)malloc(sizeof(doublecomplex) * _iSize);
    int iTwo	= 2;
    int iOne	= 1;
    double *pReal = &poComplex[0].r;
    double *pImg = &poComplex[0].i;

    if (_pdblReal != NULL && _pdblImg != NULL)
    {

        C2F(dcopy)(&_iSize, _pdblReal, &iOne, pReal, &iTwo);
        C2F(dcopy)(&_iSize, _pdblImg, &iOne, pImg, &iTwo);
    }
    else if (_pdblReal != NULL && _pdblImg == NULL)
    {
        double ZERO = 0.;
        C2F(dcopy)(&_iSize, _pdblReal, &iOne, pReal, &iTwo);
        C2F(dset)(&_iSize, &ZERO, pImg, &iTwo);
    }
    else if (_pdblReal == NULL && _pdblImg != NULL)
    {
        double ZERO = 0.;
        C2F(dset)(&_iSize, &ZERO, pReal, &iTwo);
        C2F(dcopy)(&_iSize, _pdblImg, &iOne, pImg, &iTwo);
    }
    else
    {
        free(poComplex);
        return NULL;
    }
    return poComplex;
}
