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
int C2F(sci_frexp) (char *fname,unsigned long fname_len)
{
	int i;
	int iRet					= 0;
	int iRows					= 0;
	int iCols					= 0;

	int* piAddr				= NULL;


	double *pdblReal	= NULL;
	double *pdblCoef	= NULL;
	double *pdblExp		= NULL;
	
	CheckRhs(1,1);
	CheckLhs(2,2);

	iRet = getVarAddressFromPosition(1, &piAddr);
	if(iRet)
	{
		return 1;
	}

	if(getVarType(piAddr) != sci_matrix)
	{
		OverLoad(1);
		return 0;
	}

	if(isVarComplex(piAddr))
	{
		Error(9999);
		sciprint(_("%s: real data only.\n"), fname);
	}

	iRet = getMatrixOfDouble(piAddr, &iRows, &iCols, &pdblReal);
	if(iRet)
	{
		return 1;
	}

	iRet = allocMatrixOfDouble(Rhs + 1, iRows, iCols, &pdblCoef);
	if(iRet)
	{
		return 1;
	}

	iRet = allocMatrixOfDouble(Rhs + 2, iRows, iCols, &pdblExp);
	if(iRet)
	{
		return 1;
	}

	for(i = 0 ; i < iRows * iCols; i++)
	{
		pdblCoef[i] = dfrexps(pdblReal[i], &pdblExp[i]);
	}

	LhsVar(1) = Rhs + 1;
	LhsVar(2) = Rhs + 2;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
