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
#include <string.h>
#include "gw_elementary_functions.h"
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

	int iMode						= 0;

	CheckRhs(1,2);
	CheckLhs(1,2);

	iRet = getVarAddressFromPosition(1, &piAddr, _piKey);
	if(iRet)
	{
		return 1;
	}

	if(getVarType(piAddr) != sci_matrix)
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
			iRet = allocMatrixOfDouble(Rhs + 2, 1, 1, &pdblRealRet, _piKey);
			if(iRet)
			{
				return 1;
			}
		}

		iRet = allocMatrixOfDouble(Rhs + 1, 1, 1, &pdblRealRet, _piKey);
		if(iRet)
		{
			return 1;
		}

		LhsVar(1) = Rhs + 1;
		PutLhsVar();
	}

	if(isVarComplex(piAddr))
	{
		iRet = getComplexMatrixOfDouble(piAddr, &iRows, &iCols, &pdblReal, &pdblImg);
	}
	else
	{
		iRet = getMatrixOfDouble(piAddr, &iRows, &iCols, &pdblReal);
	}

	return 0;
}
/*-----------------------------------------------------------------------------------*/

