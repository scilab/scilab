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
#include "sciprint.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_ones) (char *fname,unsigned long fname_len)
{
	int iRet					= 0;
	int iRows					= 0;
	int iCols					= 0;
	double *pdblReal	= 0;
	int* piAddr1			= NULL;
	int* piAddr2			= NULL;

	CheckLhs(1,1);


	if(Rhs > 2)
	{//call %hm_ones
		int iStart	= 1;
		int iRhs		= Rhs;
		int iLhs		= Lhs;

		SciString(&iStart,"%hm_ones", &iLhs, &iRhs);

		LhsVar(1) = 1;
		PutLhsVar();
		return 0;
	}
	else if(Rhs <= 0)
	{
		Rhs = 0;
		iRows = 1;
		iCols = 1;
	}
	else if(Rhs == 1)
	{
		iRet = getVarAddressFromPosition(1, &piAddr1);
		if(iRet)
		{
			return 1;
		}

		if(getVarType(piAddr1) > 10)//pas classe !
		{
			OverLoad(1);
			return 0;
		}
		iRet = getVarDimension(piAddr1, &iRows, &iCols);
		if(iRet)
		{
			return 1;
		}
	}
	else if(Rhs == 2)
	{
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

		iRet = getDimFromVar(piAddr1, &iRows);
		if(iRet)
		{
			return 1;
		}

		iRet = getDimFromVar(piAddr2, &iCols);
		if(iRet)
		{
			return 1;
		}
	}

	if(iRows == 0 || iCols == 0)
	{
		iRows = 0;
		iCols = 0;
	}

	iRet = allocMatrixOfDouble(Rhs + 1, iRows, iCols, &pdblReal);
	if(iRows != 0)
	{
		vDset(iRows * iCols, 1, pdblReal, 1);
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
