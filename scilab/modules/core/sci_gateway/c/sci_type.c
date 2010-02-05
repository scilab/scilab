/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "gw_core.h"
#include "stack-c.h"
#include "api_scilab.h"
#include "api_oldstack.h"
/*--------------------------------------------------------------------------*/
int sci_type(char *fname, int* _piKey)
{
	SciErr sciErr;
	int* piAddr	= NULL;
	int iType		= 0;
	int iRet = 0;

	CheckRhs(1,1);
	CheckLhs(1,1);

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

	iRet = createMatrixOfDoubleFromInteger(_piKey, Rhs + 1, 1, 1, &iType);
	if(iRet)
	{
		return 0;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
