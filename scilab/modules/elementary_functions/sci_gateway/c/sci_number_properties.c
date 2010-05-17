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
#include "api_oldstack.h"

/*--------------------------------------------------------------------------*/
int sci_number_properties(char *fname, int* _piKey)
{
	SciErr sciErr;
	int i;
	int iRet				= 0;

	int iRows				= 0;
	int iCols				= 0;
	char **pstData	= NULL;
	int* piAddr			= NULL;

	double dblRet		= 0;
	int bRet				= 0;

	int bBoolFlag		= 0;

	CheckRhs(1,1);
	CheckLhs(1,1);

	sciErr = getVarAddressFromPosition(_piKey, 1, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	iRet = getAllocatedMatrixOfString(_piKey, piAddr, &iRows, &iCols, &pstData);
	if(iRet)
	{
		freeAllocatedMatrixOfString(iRows, iCols, pstData);
		return 1;
	}

	if(strcmp(pstData[0], "eps") == 0)
	{
		dblRet	= F2C(dlamch)("e",1L);
	}
	else if(strcmp(pstData[0], "huge") == 0)
	{
		dblRet	= F2C(dlamch)("oe",1L);
	}
	else if(strcmp(pstData[0], "tiny") == 0)
	{
		dblRet	= F2C(dlamch)("u",1L);
	}
	else if(strcmp(pstData[0], "radix") == 0)
	{
		dblRet	= F2C(dlamch)("b",1L);
	}
	else if(strcmp(pstData[0], "digits") == 0)
	{
		dblRet	= F2C(dlamch)("n",1L);
	}
	else if(strcmp(pstData[0], "minexp") == 0)
	{
		dblRet	= F2C(dlamch)("m",1L);
	}
	else if(strcmp(pstData[0], "maxexp") == 0)
	{
		dblRet	= F2C(dlamch)("l",1L);
	}
	else if(strcmp(pstData[0], "denorm") == 0)
	{
		bBoolFlag = 1;
		if(F2C(dlamch)("u",1L) / F2C(dlamch)("b",1L) > 0)
		{
			bRet	= 1;
		}
		else
		{
			bRet	= 0;
		}
	}
	else if(strcmp(pstData[0], "tiniest") == 0)
	{
		double dblRadix	= F2C(dlamch)("b",1L);
		dblRet	= F2C(dlamch)("u",1L);

		if(dblRet	/ dblRadix != 0)
		{//denormalised number are used
			int iDigits = (int)F2C(dlamch)("n",1L);
			for(i = 1 ; i < iDigits ; i++)
				dblRet	/= dblRadix;
		}
	}
	else
	{
		sciprint(_("%s: unknown property kind.\n"), fname);
		SciError(999);
		return 0;
	}

	if(bBoolFlag)
	{
		iRet = createScalarBoolean(_piKey, Rhs + 1, bRet);
	}
	else
	{
		iRet = createScalarDouble(_piKey, Rhs + 1, dblRet);
	}

	if(iRet)
	{
		return 1;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
