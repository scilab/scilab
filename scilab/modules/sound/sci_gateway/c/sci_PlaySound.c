/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2007 - INRIA - Scilab 
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
#include <wchar.h>
#include <stdio.h>
#ifdef _MSC_VER
	#include <Windows.h>
	#pragma comment(lib, "winmm.lib")
#endif
#include "gw_sound.h"
#include "stack-c.h"
#include "expandPathVariable.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "localization.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
static int playsound(wchar_t *wcFilename);
/*--------------------------------------------------------------------------*/
/* private function called by playsnd */
/*--------------------------------------------------------------------------*/
int sci_Playsound (char *fname,unsigned long fname_len)
{
	StrErr strErr;
	int *piAddressVarOne = NULL;
	wchar_t *pStVarOne = NULL;
	int iType1 = 0;
	int lenStVarOne = 0;
	int m1 = 0, n1 = 0;
	wchar_t *expandedPath = NULL;

	CheckRhs(1,1);
	CheckLhs(0,1);

	strErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	strErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	if (iType1 != sci_strings )
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
		return 0;
	}

	strErr = getMatrixOfWideString(pvApiCtx, piAddressVarOne,&m1,&n1,&lenStVarOne,&pStVarOne);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	if ( (m1 != n1) && (n1 != 1) ) 
	{
		Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,1);
		return 0;
	}

	pStVarOne = (wchar_t*)MALLOC(sizeof(wchar_t)*(lenStVarOne + 1));
	if (pStVarOne == NULL)
	{
		Scierror(999,_("%s : Memory allocation error.\n"),fname);
		return 0;
	}

	strErr = getMatrixOfWideString(pvApiCtx, piAddressVarOne, &m1, &n1, &lenStVarOne, &pStVarOne);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	expandedPath = expandPathVariableW(pStVarOne);
	if (pStVarOne) {FREE(pStVarOne); pStVarOne = NULL;}

#ifdef _MSC_VER
	{
		if (expandedPath)
		{
			playsound(expandedPath);
			FREE(expandedPath);
			expandedPath = NULL;
		}
		
		LhsVar(1) = 0;
		C2F(putlhsvar)();
	}
#else
	{
		if (expandedPath) {FREE(expandedPath); expandedPath = NULL;}
		Scierror(999,_("%s: An error occurred: %s\n"),fname,_("Cannot play file.") );
	}
#endif
	return 0;
}
/*--------------------------------------------------------------------------*/
static int playsound(wchar_t *wcFilename)
{
#ifdef _MSC_VER
	if (wcFilename)
	{
		/* Stop Playing*/
		PlaySoundW(NULL, NULL, SND_PURGE);
		PlaySoundW(wcFilename, NULL, SND_ASYNC|SND_FILENAME);
	}
#endif
	return 0;
}
/*--------------------------------------------------------------------------*/

