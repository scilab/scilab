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
#if _MSC_VER
#include <windows.h>
#include "strdup_windows.h"
#endif
#include "gw_sound.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "api_scilab.h"
#include "BOOL.h"
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
	SciErr sciErr;
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

		sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
			return 0;
		}

		sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
			return 0;
		}

		if (iType1  != sci_strings )
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
			return 0;
		}

		sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne,&m1,&n1,&lenStVarOne, NULL);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
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
			Scierror(999,_("%s: Memory allocation error.\n"),fname);
			return 0;
		}

		sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, &lenStVarOne, &pStVarOne);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
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

	sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, m_out, n_out, &output);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
        Scierror(999,_("%s: Memory allocation error.\n"), fname);
		return 0;
	}

    if (output) {FREE(output); output = NULL;}

	LhsVar(1) = Rhs+1;
	PutLhsVar();

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
