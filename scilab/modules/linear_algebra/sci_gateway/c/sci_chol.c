/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2009 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "api_common.h"
#include "api_double.h"

#include <string.h>
#include <stdio.h>
#include "stack-c.h"
#include "gw_linear_algebra.h"
#include "Scierror.h"
#include "localization.h"

#include "chol.h"


int sci_chol(char *fname, int* _piKey)
{
	int *pArg1;
	int iCholProductResult = 0;
	int iRows	= 0;
	int iCols	= 0;
	int type;

	int iReal	= 0;
	int iImg	= 0;

	double *pdblReal		= NULL;
	double *pdblImg			= NULL;
	double *pdblReturnReal	= NULL;
	double *pdblReturnImg	= NULL;

	if( Rhs >= 1)
	{
		getVarAddressFromPosition(_piKey, 1, &pArg1);
		getVarType(_piKey, pArg1, &type);
		if (type != sci_matrix)
		{
			OverLoad(1);
			return 0;
		}

		getVarDimension(_piKey, pArg1, &iRows, &iCols);
		if(iRows != iCols)
		{
			/* We have to get the Rhs var even if we do not use it...
			otherwise Scilab stack turned upside-down... */
			getMatrixOfDouble(_piKey, pArg1, &iRows, &iCols, &pdblReal);
			Err = 1;
			Error(20);
			return 0;
		}

		if(iRows == 0)
		{
			double* pdblReturnReal;
			double* pdblReturnImg;
			/* We have to get the Rhs var even if we do not use it...
			otherwise Scilab stack turned upside-down... */
			getMatrixOfDouble(_piKey, pArg1, &iRows, &iCols, &pdblReal);
			if(isVarComplex(_piKey, pArg1))
			{
				allocComplexMatrixOfDouble(_piKey, Rhs + 1, iRows, iCols, &pdblReturnReal, &pdblReturnImg);
			}
			else
			{
				allocMatrixOfDouble(_piKey, Rhs + 1, iRows, iCols, &pdblReturnReal);
			}

			LhsVar(1) = Rhs + 1;
			return 0;
		}
		else if(iRows == -1) // manage eye case
			// TODO : eye must not be a particular case with dimensions.
		{
			getMatrixOfDouble(_piKey, pArg1, &iRows, &iCols, &pdblReal);
			if (pdblReal[0] <= 0)
			{
				/* Matrix must be positive definite */
				Error(29);
				return 0;
			}

			pdblReal[0]		= sqrt(pdblReal[0]);
			LhsVar(1) = 1;
			return 0;
		}

		if(isVarComplex(_piKey, pArg1))
		{
			doublecomplex *poData = NULL;
			getComplexZMatrixOfDouble(_piKey, pArg1, &iRows, &iCols, &poData);
			iCholProductResult = iComplexCholProduct(poData, iRows);
			allocComplexMatrixOfDouble(_piKey, Rhs + 1, iRows, iCols, &pdblReturnReal, &pdblReturnImg);
			vGetPointerFromDoubleComplex(poData, iRows * iCols, pdblReturnReal, pdblReturnImg);
			vFreeDoubleComplexFromPointer(poData);
		}
		else
		{
			getMatrixOfDouble(_piKey, pArg1, &iRows, &iCols, &pdblReal);
			allocMatrixOfDouble(_piKey, Rhs + 1, iRows, iCols, &pdblReturnReal);
			memcpy(pdblReturnReal, pdblReal, iRows * iCols * sizeof(double));
			iCholProductResult = iRealCholProduct(pdblReturnReal, iRows);
		}

		if (iCholProductResult > 0)
		{
			/* Matrix must be positive definite */
			Error(29);
			return 0;
		}

		LhsVar(1) = Rhs + 1;
	}
	return 0;
}

/*--------------------------------------------------------------------------*/
