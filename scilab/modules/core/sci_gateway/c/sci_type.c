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
	int iRet		= 0;
	int* piAddr	= NULL;
	int iType		= 0;

	CheckRhs(1,1);
	CheckLhs(1,1);

	iRet = getVarAddressFromPosition(1, &piAddr, _piKey);
	if(iRet)
	{
		return 1;
	}

	iType = getVarType(piAddr);
	iRet = createMatrixOfDoubleFromInteger(Rhs + 1, 1, 1, &iType, _piKey);
	if(iRet)
	{
		return 1;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
