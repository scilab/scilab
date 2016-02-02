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

#ifndef __TRANSPOSE_H__
#define __TRANSPOSE_H__

#include "doublecomplex.h"

#include "dynlib_ast.h"

EXTERN_AST void vTransposeRealMatrix(
    double *_pdblRealIn,
    int _iRowsIn, int _iColsIn,
    double *_pdblRealOut);

EXTERN_AST void vTransposeComplexMatrix(
    double *_pdblRealIn, double *_pdblImgIn,
    int _iRowsIn, int _iColsIn,
    double *_pdblRealOut, double *_pdblImgOut, int _iConjugate);

EXTERN_AST void vTransposeDoubleComplexMatrix(
    doublecomplex *_poIn,
    int _iRowsIn, int _iColsIn,
    doublecomplex *_poOut, int _iConjugate);

EXTERN_AST void vPretransposeRealMatrix(
    double *_pdblRealIn,
    int _iRowsIn, int _iColsIn,
    double *_pdblRealOut);

EXTERN_AST void vPretransposeComplexMatrix(
    double *_pdblRealIn, double *_pdblImgIn,
    int _iRowsIn, int _iColsIn,
    double *_pdblRealOut, double *_pdblImgOut);

#endif /* __TRANSPOSE_H__ */
