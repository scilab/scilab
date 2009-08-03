/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
