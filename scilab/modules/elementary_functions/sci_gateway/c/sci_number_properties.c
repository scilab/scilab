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
#include "sciprint.h"
#include "localization.h"
#include "api_scilab.h"
#include "Scierror.h"


/*--------------------------------------------------------------------------*/
int C2F(sci_number_properties) (char *fname,unsigned long fname_len)
{
	int i;
	int iRet				= 0;

	int iRows				= 0;
	int iCols				= 0;
	int* piLen			= NULL;
	char **pstData	= NULL;
	int* piAddr			= NULL;

	double *pdblRet	= NULL;

	CheckRhs(1,1);
	CheckLhs(1,1);

	iRet = getVarAddressFromPosition(1, &piAddr);
	if(iRet)
	{
		return 1;
	}

	if(getVarType(piAddr) != sci_strings)
	{
		Err = 1;
		Error(55);
		return 0;
	}

	iRet = getMatrixOfString(piAddr, &iRows, &iCols, NULL, NULL);
	if(iRet)
	{
		return 1;
	}

	if(iRows != 1 || iCols != 1)
	{
		return 1;
	}
	
	piLen = (int*)malloc(sizeof(int) * iRows * iCols);
	iRet = getMatrixOfString(piAddr, &iRows, &iCols, piLen, NULL);
	if(iRet)
	{
		return 1;
	}

	pstData	= (char**)malloc(sizeof(char*) * iRows * iCols);
	for(i = 0 ; i < iRows * iCols ; i++)
	{
		pstData[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+1 for null termination
	}

	iRet = getMatrixOfString(piAddr, &iRows, &iCols, piLen, pstData);
	if(iRet)
	{
		return 1;
	}

	iRet = allocMatrixOfDouble(Rhs + 1, 1, 1, &pdblRet);
	//pReturnData = (double*)malloc(sizeof(double));

	if(strcmp(pstData[0], "eps") == 0)
		pdblRet[0]	= F2C(dlamch)("e",1L);
	else if(strcmp(pstData[0], "huge") == 0)
		pdblRet[0]	= F2C(dlamch)("oe",1L);
	else if(strcmp(pstData[0], "tiny") == 0)
		pdblRet[0]	= F2C(dlamch)("u",1L);
	else if(strcmp(pstData[0], "radix") == 0)
		pdblRet[0]	= F2C(dlamch)("b",1L);
	else if(strcmp(pstData[0], "digits") == 0)
		pdblRet[0]	= F2C(dlamch)("n",1L);
	else if(strcmp(pstData[0], "minexp") == 0)
		pdblRet[0]	= F2C(dlamch)("m",1L);
	else if(strcmp(pstData[0], "maxexp") == 0)
		pdblRet[0]	= F2C(dlamch)("l",1L);
	else if(strcmp(pstData[0], "denorm") == 0)
	{
		if(F2C(dlamch)("u",1L) / F2C(dlamch)("b",1L) > 0)
			pdblRet[0]	= 1;
		else
			pdblRet[0]	= 0;
	}
	else if(strcmp(pstData[0], "tiniest") == 0)
	{
		double dblRadix	= F2C(dlamch)("b",1L);
		pdblRet[0]	= F2C(dlamch)("u",1L);

		if(pdblRet[0] / dblRadix != 0)
		{//denormalised number are used
			int iDigits = (int)F2C(dlamch)("n",1L);
			for(i = 1 ; i < iDigits ; i++)
				pdblRet[0] /= dblRadix;
		}
	}
	else
	{
		sciprint(_("%s: unknown property kind.\n"), fname);
		Error(999);
		return 0;
	}

	free(piLen);
	for(i = 0 ; i < iRows * iCols ; i++)
	{
		free(pstData[i]);
	}
	free(pstData);

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
