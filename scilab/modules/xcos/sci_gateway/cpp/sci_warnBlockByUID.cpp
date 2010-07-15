/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Vincent COUVERt
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "Xcos.hxx"
extern "C"
{
#include "gw_xcos.h"
#include "stack-c.h"
#include "callxcos.h"
#include "api_common.h"
#include "api_string.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "getFullFilename.h"
#include "getScilabJavaVM.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_xcos;
/*--------------------------------------------------------------------------*/
int sci_warnBlockByUID(char *fname, int* _piKey)
{
	SciErr sciErr;


	CheckRhs(2,2);
	CheckLhs(0,1);

	int m1 = 0, n1 = 0;
	int *piAddressVarOne = NULL;
	int m2 = 0, n2 = 0;
	int *piAddressVarTwo = NULL;

	char *pStVarOne = NULL;
	int lenStVarOne = 0;

	char *pStVarTwo = NULL;
	int lenStVarTwo = 0;

	/** READ UID **/
	sciErr = getVarAddressFromPosition(_piKey, 1, &piAddressVarOne);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	/* get dimensions */
	sciErr = getMatrixOfString(_piKey, piAddressVarOne, &m1, &n1, NULL, NULL);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if (m1 * n1 != 1) {
		Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,1);
		return 0;
	} 

	/* get lengths */
	sciErr = getMatrixOfString(_piKey, piAddressVarOne, &m1, &n1, &lenStVarOne, NULL);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	pStVarOne = (char*)MALLOC(sizeof(char*) * (lenStVarOne + 1));

	/* get strings */
	sciErr = getMatrixOfString(_piKey, piAddressVarOne, &m1, &n1, &lenStVarOne, &pStVarOne);
	if(sciErr.iErr)
	{
		FREE(pStVarOne);
		printError(&sciErr, 0);
		return 0;
	}

	/** READ MESSAGE **/
	sciErr = getVarAddressFromPosition(_piKey, 2, &piAddressVarTwo);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	/* get dimensions */
	sciErr = getMatrixOfString(_piKey, piAddressVarTwo, &m1, &n1, &lenStVarTwo, NULL);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if (m1 * n1 != 1) {
		Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,2);
		return 0;
	} 

	/* get lengths */
	sciErr = getMatrixOfString(_piKey, piAddressVarTwo, &m1, &n1, &lenStVarTwo, NULL);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	pStVarTwo = (char*)MALLOC(sizeof(char*) * (lenStVarTwo + 1));

	/* get strings */
	sciErr = getMatrixOfString(_piKey, piAddressVarTwo, &m1, &n1, &lenStVarTwo, &pStVarTwo);
	if(sciErr.iErr)
	{
		FREE(pStVarTwo);
		printError(&sciErr, 0);
		return 0;
	}
	Xcos::warnCellByUID(getScilabJavaVM(), pStVarOne, pStVarTwo);

	LhsVar(1) = 0;
	PutLhsVar();

	FREE(pStVarOne);
	FREE(pStVarTwo);
	return 0;
}
/*--------------------------------------------------------------------------*/
