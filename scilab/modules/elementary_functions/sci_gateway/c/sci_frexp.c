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
int sci_frexp(char *fname, int* _piKey)
{
	SciErr sciErr;
	int i;
	int iRows					= 0;
	int iCols					= 0;
	int iType					= 0;


	int* piAddr				= NULL;


	double *pdblReal	= NULL;
	double *pdblCoef	= NULL;
	double *pdblExp		= NULL;

	CheckRhs(1,1);
	CheckLhs(2,2);

	sciErr = getVarAddressFromPosition(_piKey, 1, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = getVarType(_piKey, piAddr, &iType);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if(iType != sci_matrix)
	{
		OverLoad(1);
		return 0;
	}

	if(isVarComplex(_piKey, piAddr))
	{
		Error(9999);
		sciprint(_("%s: real data only.\n"), fname);
	}

	sciErr = getMatrixOfDouble(_piKey, piAddr, &iRows, &iCols, &pdblReal);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = allocMatrixOfDouble(_piKey, Rhs + 1, iRows, iCols, &pdblCoef);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = allocMatrixOfDouble(_piKey, Rhs + 2, iRows, iCols, &pdblExp);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
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
