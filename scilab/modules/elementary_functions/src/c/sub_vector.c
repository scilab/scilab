/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "basic_functions.h"

/*Substract two vectors*/
void vDless(int _iNbElem, double* _pdblIn1, double* _pdblIn2, int _iIncIn1, int _iIncIn2, double* _pdblOut)
{
	int iIndex1			= 0;
	double *pdblTemp	= NULL;

	pdblTemp = (double*)malloc(sizeof(double) * _iNbElem);
	for(iIndex1 = 0 ; iIndex1 < _iNbElem ; iIndex1++)
		pdblTemp[iIndex1] = -_pdblIn2[iIndex1 * _iIncIn2];

	vDadd(_iNbElem, _pdblIn1, pdblTemp, _iIncIn1, 1, _pdblOut);
	free(pdblTemp);
}

