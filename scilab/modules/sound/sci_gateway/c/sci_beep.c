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
#include <stdio.h>
#include <string.h>
#include "gw_sound.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "api_scilab.h"
#include "BOOL.h"
#if _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
#define BEEP_ON "on"
#define BEEP_OFF "off"
/*--------------------------------------------------------------------------*/
static BOOL beepON = TRUE;
/*--------------------------------------------------------------------------*/
void doBeep(void);
/*--------------------------------------------------------------------------*/
int sci_beep(char *fname,unsigned long fname_len)
{
	StrErr strErr;
	char *output = NULL;
	int m_out = 1, n_out = 1;

	Rhs = Max(0,Rhs);
	CheckRhs(0,1);
	CheckLhs(0,1);

	if (Rhs == 0)
	{
		if (beepON) doBeep();
	}
	else
	{
		int *piAddressVarOne = NULL;
		char *pStVarOne = NULL;
		int iType1 = 0;
		int lenStVarOne = 0;
		int m1 = 0, n1 = 0;

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

		if (iType1  != sci_strings )
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
			return 0;
		}

		strErr = getMatrixOfString(pvApiCtx, piAddressVarOne,&m1,&n1,&lenStVarOne,&pStVarOne);
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

		pStVarOne = (char*)MALLOC(sizeof(char)*(lenStVarOne + 1));
		if (pStVarOne == NULL)
		{
			Scierror(999,_("%s : Memory allocation error.\n"),fname);
			return 0;
		}

		strErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, &lenStVarOne, &pStVarOne);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		if ( (strcmp(pStVarOne, BEEP_ON) == 0) || (strcmp(pStVarOne, BEEP_OFF) == 0) )
		{
			if ( strcmp(pStVarOne, BEEP_ON) == 0 )
			{
				beepON = TRUE;
			}
			else
			{
				beepON = FALSE;
			}

			FREE(pStVarOne); pStVarOne = NULL;
		}
		else
		{
			FREE(pStVarOne); pStVarOne = NULL;
			Scierror(999,_("%s: Wrong input argument #%d: '%s' or '%s' expected.\n"),fname,1, BEEP_ON, BEEP_OFF);
			return 0;
		}
	}

	if (beepON)
	{
		output = strdup(BEEP_ON);
	}
	else
	{
		output = strdup(BEEP_OFF);
	}

	strErr = createMatrixOfString(pvApiCtx, Rhs + 1, m_out, n_out, &output);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();

	if (output) {FREE(output); output = NULL;}

	return 0;
}
/*--------------------------------------------------------------------------*/
void doBeep(void)
{
#ifdef _MSC_VER
	MessageBeep((UINT)-1);
#else
	puts("\a");
#endif
}
/*--------------------------------------------------------------------------*/
