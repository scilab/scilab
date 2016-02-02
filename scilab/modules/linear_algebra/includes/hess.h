/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY
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
#ifndef HESS_H
#define HESS_H
#include "dynlib_linear_algebra.h"

LINEAR_ALGEBRA_IMPEXP int iHessM(double * pData, int iCols, int complexArg, double* pH);
LINEAR_ALGEBRA_IMPEXP int iHess(double* pData, int iCols, int complexArg, double* pH, double* pTau, double* pWork, int workSize);

#endif
