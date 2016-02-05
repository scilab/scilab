/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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
/*--------------------------------------------------------------------------*/

#include "doublecomplex.h"

int schurSelect(types::Double** _pDblIn, types::Double** _pDblOut, bool _bIsComplex, bool _bIsDiscrete, bool _bIsContinu, ConfigVariable::EntryPointStr* pStrFunction);

double* allocDgeesWorkspace(int iCols, int* allocated);
doublecomplex* allocZgeesWorkspace(int iCols, int* allocated);
double* allocDggesWorkspace(int iCols, int* allocated);
doublecomplex* allocZggesWorkspace(int iCols, int* allocated);

typedef int (*schur_dgees_t)(double* real, double* complex);
typedef int (*schur_zgees_t)(doublecomplex* complex);
typedef int (*schur_dgges_t)(double* real, double* complex, double* beta);
typedef int (*schur_zgges_t)(doublecomplex* alpha, doublecomplex* beta);

extern void C2F(dgees)(char const jobVs[1], char const sort[1], schur_dgees_t select, int const* n, double* a, int const* ldA, int * sDim, double* wR, double* wI, double* VS, int const* ldVS, double* work, int const* lWork, int* bWork, int* info );

extern void C2F(zgees)(char const jobVs[1], char const sort[1],  schur_zgees_t select, int const* n, doublecomplex* a, int const* ldA, int * sDim, doublecomplex* w, doublecomplex* VS, int const* ldVS, doublecomplex* work, int const* lWork, double* rWork, int* bWork, int* info );

extern void C2F(dgges)(char const jobVsL[1], char const jobVsR[1], char const sort[1], schur_dgges_t select, int const* n, double* a, int const* ldA, double* b, int const* ldB, int* sDim, double* alphaR, double* alphaI, double* beta, double* VSL, int const* ldVSL, double* VSR, int const* ldVSR, double* work, int const* lWork, int* bWork, int* info );

extern void C2F(zgges)(char const jobVsL[1], char const jobVsR[1], char const sort[1],  schur_zgges_t select, int const* n, doublecomplex* a, int const* ldA, doublecomplex* b, int const* ldB, int* sDim, doublecomplex* alpha, doublecomplex* beta, doublecomplex* VSL, int const* ldVSL, doublecomplex* VSR, int const* ldVSR, doublecomplex* work, int const* lWork, double* rWork, int* bWork, int* info);


