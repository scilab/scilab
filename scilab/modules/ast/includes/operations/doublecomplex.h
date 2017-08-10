/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Michael BAUDIN
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

#ifndef __DOUBLECOMPLEX_H__
#define __DOUBLECOMPLEX_H__

#include "dynlib_ast.h"
/**
* structure double complex
* r real part
* i imag part
*/
typedef struct
{
    double r, i;
} doublecomplex;

/*Tools to convert memory matrix storage from C to Z and Z to C*/
// vGetPointerFromDoubleComplex : set the real and imaginary part of an target array from a source doublecomplex array
EXTERN_AST void vGetPointerFromDoubleComplex(const doublecomplex *_poComplex, int _iSize, double *_pdblReal, double *_pdblImg);
// oGetDoubleComplexFromPointer : returns a target doublecomplex constructed from the source real and imaginary parts.
EXTERN_AST doublecomplex* oGetDoubleComplexFromPointer(double *_pdblReal, double *_pdblImg, int _iSize);
// vFreeDoubleComplexFromPointer free a pointer allocated by oGetDoubleComplexFromPointer
EXTERN_AST void vFreeDoubleComplexFromPointer(doublecomplex *_poComplex);


#endif /* __DOUBLECOMPLEX_H__ */
