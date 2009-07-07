/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Michael BAUDIN
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __DOUBLECOMPLEX_H__
#define __DOUBLECOMPLEX_H__

#include "operation.hxx"
/**
* structure double complex
* r real part
* i imag part
*/
typedef struct { double r, i; } doublecomplex;

/*Tools to convert memory matrix storage from C to Z and Z to C*/
// vGetPointerFromDoubleComplex : set the real and imaginary part of an target array from a source doublecomplex array
EXTERN_OP void vGetPointerFromDoubleComplex(doublecomplex *_poComplex, int _iSize, double *_pdblReal, double *_pdblImg);
// oGetDoubleComplexFromPointer : returns a target doublecomplex constructed from the source real and imaginary parts.
EXTERN_OP doublecomplex* oGetDoubleComplexFromPointer(double *_pdblReal, double *_pdblImg, int _iSize);
// vFreeDoubleComplexFromPointer free a pointer allocated by oGetDoubleComplexFromPointer
EXTERN_OP void vFreeDoubleComplexFromPointer(doublecomplex *_poComplex);


#endif /* __DOUBLECOMPLEX_H__ */