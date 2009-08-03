/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/ 
#include "gw_elementary_functions.h"
#include "stack-c.h"
#include "basic_functions.h"
#include "api_scilab.h"
#include "Scierror.h"


int C2F(sci_nearfloat) (char *fname,unsigned long fname_len)
{
	int i;
	int iRet						= 0;

	int* piAddr1				= NULL;
	int iRows1					= 0;
	int iCols1					= 0;
	int* piLen					= NULL;
	char **pstData			= NULL;

	int* piAddr2				= NULL;
	int iRows2					= 0;
	int iCols2					= 0;
	double *pdblReal		= NULL;

	double dblMode			= 0;

	double *pdblRealRet	= NULL;
	

	CheckRhs(2,2);
	CheckLhs(1,1);

	iRet = getVarAddressFromPosition(1, &piAddr1);
	if(iRet)
	{
		return 1;
	}

	iRet = getVarAddressFromPosition(2, &piAddr2);
	if(iRet)
	{
		return 1;
	}

	if(getVarType(piAddr1) != sci_strings)
	{
		Err = 1;
		Error(55);
		return 0;
	}

	if(getVarType(piAddr2) != sci_matrix)
	{
		Err = 2;
		Error(53);
		return 0;
	}

	iRet = getMatrixOfString(piAddr1, &iRows1, &iCols1, NULL, NULL);
	if(iRet)
	{
		return 1;
	}

	if(iRows1 != 1 || iCols1 != 1)
	{
		return 1;
	}
	
	piLen = (int*)malloc(sizeof(int) * iRows1 * iCols1);
	iRet = getMatrixOfString(piAddr1, &iRows1, &iCols1, piLen, NULL);
	if(iRet)
	{
		return 1;
	}

	pstData	= (char**)malloc(sizeof(char*) * iRows1 * iCols1);
	for(i = 0 ; i < iRows1 * iCols1 ; i++)
	{
		pstData[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+1 for null termination
	}

	iRet = getMatrixOfString(piAddr1, &iRows1, &iCols1, piLen, pstData);
	if(iRet)
	{
		return 1;
	}

	iRet = getMatrixOfDouble(piAddr2, &iRows2, &iCols2, &pdblReal);
	if(iRet)
	{
		return 1;
	}

	if(strcmp(pstData[0], "succ") == 0)
	{
		dblMode = 1.0;
	}
	else if(strcmp(pstData[0], "pred") == 0)
	{
		dblMode = -1.0;
	}
	else
	{
		Error(999);
		return 0;
	}

	iRet = allocMatrixOfDouble(Rhs + 1, iRows2, iCols2, &pdblRealRet);
	if(iRet)
	{
		return 1;
	}

	for(i = 0 ; i < iRows2 * iCols2 ; i++)
	{
		pdblRealRet[i] = dblNearFloat(pdblReal[i], dblMode);
	}

	free(piLen);
	for(i = 0 ; i < iRows1 * iCols1 ; i++)
	{
		free(pstData[i]);
	}
	free(pstData);

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
