/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
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

#ifndef __ASSEMBLEEIGENVECTORS__
#define __ASSEMBLEEIGENVECTORS__

#include "doublecomplex.h"
int assembleEigenvectorsSourceToTarget(int iRows, double * eigenvaluesImg,
                                       double * EVRealSource,
                                       double * EVRealTarget, double * EVImgTarget);
int assembleEigenvectorsInPlace(int iRows, double * eigenvaluesImg,
                                double * EVReal, double * EVImg);
int assembleComplexEigenvaluesFromDoublePointer(int iRows, double * EVRealSource, double * EVImgSource,
        double * EVRealTarget, double * EVImgTarget);
int assembleComplexEigenvaluesFromDoubleComplexPointer(int iRows, doublecomplex * EVSource,
        double * EVRealTarget, double * EVImgTarget);
int assembleEigenvaluesFromDoublePointer(int iRows, double * EVSource, double * EVTarget);
#endif /* !__ASSEMBLEEIGENVECTORS__ */
