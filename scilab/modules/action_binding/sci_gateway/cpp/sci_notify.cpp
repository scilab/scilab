/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Vincent COUVERT
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "Signal.hxx"

extern "C"
{
#include "gw_action_binding.h"
#include "stack-c.h"
#include "api_common.h"
#include "api_string.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "getScilabJavaVM.h"
#include "api_oldstack.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_action_binding_utils;
/*--------------------------------------------------------------------------*/
int sci_notify(char *fname, int*_piKey)
{
	CheckRhs(1,1);
	CheckLhs(0,1);

	int m1 = 0, n1 = 0;
	int *piAddressVarOne = NULL;
	char **pStVarOne = NULL;
	int *lenStVarOne = NULL;
	int iType = 0;
	SciErr sciErr;

	sciErr = getVarAddressFromPosition(_piKey, 1, &piAddressVarOne);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = getVarType(_piKey, piAddressVarOne, &iType);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if ( iType != sci_strings )
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
		return 0;
	}

	/* get dimensions */
	sciErr = getMatrixOfString(_piKey, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if ( m1 * n1 != 1 )
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
		return 0;
	}

	lenStVarOne = (int*)MALLOC(sizeof(int));
	if (lenStVarOne == NULL)
	{
		Scierror(999,_("%s: No more memory.\n"), fname);
		return 0;
	}

	/* get lengths */
	sciErr = getMatrixOfString(_piKey, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	pStVarOne = (char **)MALLOC(sizeof(char*));
	if (pStVarOne == NULL)
	{
		Scierror(999,_("%s: No more memory.\n"), fname);
		return 0;
	}

	pStVarOne[0] = (char*)MALLOC(sizeof(char*) * (lenStVarOne[0] + 1));

	/* get strings */
	sciErr = getMatrixOfString(_piKey, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	Signal::notify(getScilabJavaVM(), pStVarOne[0]);
	freeArrayOfString(pStVarOne, 1);

	LhsVar(1) = 0;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
