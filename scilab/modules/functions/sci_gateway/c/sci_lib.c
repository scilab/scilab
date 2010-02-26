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
/*--------------------------------------------------------------------------*/
#include "stack-c.h"
#include "gw_functions.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "machine.h"
#include "FileExist.h"
#include "getFullFilename.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intlib)();
/*--------------------------------------------------------------------------*/
int sci_libfunc(char *fname, int*_piKey)
{
	SciErr sciErr;
	int m1 = 0, n1 = 0;
	int *piAddressVarOne = NULL;
	int iType1 = 0;
	char *pStVarOne = NULL;
	char lib_filename[bsiz];
	char *fullfilename = NULL;
	int lenStVarOne = 0;

	int len = 0;

	/* Check the number of input argument */
	CheckRhs(1,1); 

	/* Check the number of output argument */
	CheckLhs(1,1);

	/* get Address of inputs */
	sciErr = getVarAddressFromPosition(_piKey, 1, &piAddressVarOne);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = getVarType(_piKey, piAddressVarOne, &iType1);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if (iType1  != sci_strings )
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
		return 0;
	}

	sciErr = getMatrixOfString(_piKey, piAddressVarOne,&m1,&n1,&lenStVarOne,&pStVarOne);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	/* check size */
	if ( (m1 != n1) && (n1 != 1) ) 
	{
		Scierror(999,"%s: Wrong size for input argument #%d: A string expected.\n",fname,1);
		return 0;
	}

	pStVarOne = (char*)MALLOC(sizeof(char)*(lenStVarOne + 1));

	if (pStVarOne == NULL)
	{
		Scierror(999,"%s : Memory allocation error.\n", fname);
		return 0;
	}

	/* get string One */
	sciErr = getMatrixOfString(_piKey, piAddressVarOne,&m1,&n1,&lenStVarOne,&pStVarOne);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if ( (pStVarOne[strlen(pStVarOne)-1] != '/') && 
		(pStVarOne[strlen(pStVarOne)-1] != '\\') )
	{
		pStVarOne = (char*)REALLOC(pStVarOne, (strlen(pStVarOne) + strlen(DIR_SEPARATOR) + 1) * sizeof(char));
		if (pStVarOne)
		{
			strcat(pStVarOne, DIR_SEPARATOR);
		}
		else
		{
			Scierror(999,"%s : Memory allocation error.\n",fname);
			return 0;
		}
	}

	fullfilename = getFullFilename(pStVarOne);
	if (fullfilename)
	{
		if ((int)strlen(fullfilename) >= bsiz)
		{
			strncpy(lib_filename, fullfilename, bsiz - 1);
			lib_filename[bsiz - 1] = '\0';
		}
		else
		{
			strcpy(lib_filename, fullfilename);
		}

		FREE(fullfilename);
		fullfilename = NULL;
	}
	else
	{
		if ((int)strlen(pStVarOne) >= bsiz)
		{
			strncpy(lib_filename, pStVarOne, bsiz - 1);
			lib_filename[bsiz - 1] = '\0';
		}
		else
		{
			strcpy(lib_filename, pStVarOne);
		}
	}

	if (pStVarOne)
	{
		FREE(pStVarOne);
		pStVarOne = NULL;
	}

	len = (int)strlen(lib_filename);
	C2F(intlib)(&len, lib_filename);

	return 0;
}
/*--------------------------------------------------------------------------*/
