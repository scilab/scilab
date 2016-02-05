/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2009 - DIGITEO - Bernard HUGUENEY
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
#ifndef SAS_SCHUR_H
#define SAS_SCHUR_H

#include "doublecomplex.h"


typedef int (*zgges_select_t)(doublecomplex const* alpha, doublecomplex const* beta);
typedef int (*dgges_select_t)(double const* alphaReal, double const* alphaImg, double const* beta);
typedef int (*zgees_select_t)(doublecomplex const* w);
typedef int (*dgees_select_t)(double const* wReal, double const* wImg);

typedef union schur_function_ptr
{
    void (*none)(void);
    zgges_select_t zgges_select;
    dgges_select_t dgges_select;
    zgees_select_t zgees_select;
    dgees_select_t dgees_select;
} schur_select_type;



/*
 * computes Schur decomposition of matrix . Does not (yet) handle pencil or ordered variants using external functions !
 *
 * in/out data :
 *
 * @param pData1 double[complex]* in/out data of the real or complex ('z' format) arg1 matrix. output : schur
 * @param pData2 double[complex]* in/out data of the real or complex ('z' format) arg2 matrix.
 * NULL if only one argument matrix. output: generalized schur form of arg2.
 *
 * in :
 *
 * @param iCols int in : nb of rows/cols of the matrices
 * @param complexArgs int (boolean semantics) in : indicates wheter the argument[s] matrix is|are complex or real
 *
 * out data :
 *
 * @param pLhsOpt1 double[complex]* out : result of the optional lhs : [|left] schur vectors for [1|2] argument[|s].
 * NULL if optional lhs are not to be computed
 * @param pLhsOpt2 double[complex]* out : result of the optional right schur vectors (only used with 2 arguments).
 *
 * @return error code : 0 if OK, -1 if MALLOC failure, LAPACK error code otherwise.
 *
 */
int iSchurM(double* pData1, double* pData2, int iCols, int complexArgs, double* pLhsOpt1, double* pLhsOpt2, schur_select_type pSelectFun, int* pDim);






extern int zb02ow(doublecomplex const* alpha, doublecomplex const* beta);
extern int zb02ox(doublecomplex const* alpha, doublecomplex const* beta);

extern int sb02ow(double const* alphaReal, double const* alphaImg, double const* beta);
extern int sb02ox(double const* alphaReal, double const* alphaImg, double const* beta);

extern int zb02mv(doublecomplex const* w);
extern int zb02mw(doublecomplex const* w);

extern int sb02mv(double const* wReal, double const* wImg);
extern int sb02mw(double const* wReal, double const* wImg);

#endif
