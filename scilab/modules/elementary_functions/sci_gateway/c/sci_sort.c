/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*-----------------------------------------------------------------------------------*/ 
#include <stdlib.h>
#include <string.h>
#include "gw_elementary_functions.h"
#include "elementary_functions.h"
#include "stack-c.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "api_oldstack.h"

/*-----------------------------------------------------------------------------------*/
int sci_sort(char *fname, int* _piKey)
{
	int iRet						= 0;

	int iRows						= 0;
	int iCols						= 0;
	int* piAddr					= NULL;

	double* pdblReal		= NULL;
	double* pdblImg			= NULL;

	double* pdblRealRet = NULL;
	double* pdblImgRet	= NULL;

	int* piInd					= NULL;

	int iMode						= 0;

	CheckRhs(1,2);
	CheckLhs(1,2);

	iRet = getVarAddressFromPosition(1, &piAddr, _piKey);
	if(iRet)
	{
		return 1;
	}

	if(getVarType(piAddr) != sci_matrix || isVarComplex(piAddr))
	{
		OverLoad(1);
		return 0;
	}

	if(Rhs == 2)
	{
		int* piAddr2	= NULL;

		iRet = getVarAddressFromPosition(2, &piAddr2, _piKey);
		if(iRet)
		{
			return 1;
		}

		iRet = getProcessMode(piAddr2, piAddr, &iMode);
		if(iRet)
		{
			return 1;
		}
	}
	
	if(iMode == 2)
	{
		OverLoad(1);
		return 1;
	}

	iRet = getVarDimension(piAddr, &iRows, &iCols);
	if(iRet)
	{
		return 1;
	}

	if(iRows == 0 || iCols == 0)
	{
		if(Lhs == 2)
		{
			iRet = allocMatrixOfDouble(Rhs + 2, 0, 0, &pdblRealRet, _piKey);
			if(iRet)
			{
				return 1;
			}
		}

		iRet = allocMatrixOfDouble(Rhs + 1, 0, 0, &pdblRealRet, _piKey);
		if(iRet)
		{
			return 1;
		}

		LhsVar(1) = Rhs + 1;
		PutLhsVar();
	}

	iRet = getMatrixOfDouble(piAddr, &iRows, &iCols, &pdblReal);
	if(iRet)
	{
		return 1;
	}

	iRet = allocMatrixOfDouble(Rhs + 1, iRows, iCols, &pdblRealRet, _piKey);
	if(iRet)
	{
		return 1;
	}

	memcpy(pdblRealRet, pdblReal, sizeof(double) * iRows * iCols);

	piInd = malloc(sizeof(int) * iRows * iCols);
	switch(iMode)
	{
	case BY_ALL :
		{
			int iSize = iRows * iCols;
			C2F(dsort)(pdblRealRet, &iSize, piInd);
		}
		break;
	case BY_ROWS :
		{
			int i;
			int iSize = iRows * iCols;
			for(i = 0 ; i < iCols ; i++)
			{
				C2F(dsort)(&pdblRealRet[iRows * i], &iRows, &piInd[iRows * i]);
			}
		}
		break;
	}

	if(Lhs == 2)
	{
		iRet = createMatrixOfDoubleFromInteger(Rhs + 2, iRows, iCols, piInd, _piKey);
		if(iRet)
		{
			return 1;
		}
		LhsVar(2) = Rhs + 2;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}
/*-----------------------------------------------------------------------------------*/

