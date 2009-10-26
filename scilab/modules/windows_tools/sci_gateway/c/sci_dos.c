/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
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
#include "gw_windows_tools.h"
#include "PATH_MAX.h"
#include "stack-c.h"
#include "MALLOC.h" /* MALLOC */
#include "spawncommand.h"
#include "sciprint.h"
#include "tmpdir.h"
#include "FileExist.h"
#include "localization.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
#include "getshortpathname.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
#define BUFSIZE 4096
/*--------------------------------------------------------------------------*/
pipeinfo pipeOut = {INVALID_HANDLE_VALUE, NULL,0};
pipeinfo pipeErr = {INVALID_HANDLE_VALUE, NULL,0};
/*--------------------------------------------------------------------------*/
static int PrintOuput(char **ouput,int nbrlines);
/*--------------------------------------------------------------------------*/
int sci_dos(char *fname,unsigned long l)
{
	StrErr strErr;
	int *piAddressVarOne = NULL;
	int iType1	= 0;
	int m1 = 0, n1 = 0;
	char *pStVarOne = NULL;
	int lenStVarOne = 0;

	char **Output = NULL;
	int numberoflines = 0;
	BOOL ECHOMODE = FALSE;
	
	CheckRhs(1,2);
	CheckLhs(1,3);

	if (Rhs == 2)
	{
		int *piAddressVarTwo = NULL;
		int m2 = 0, n2 = 0;
		int iType2 = 0;
		char *pStVarTwo = NULL;
		int lenStVarTwo = 0;

		strErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		strErr = getVarType(pvApiCtx, piAddressVarTwo, &iType2);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		if (iType2  != sci_strings )
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,2);
			return 0;
		}

		strErr = getMatrixOfString(pvApiCtx, piAddressVarTwo, &m2, &n2, &lenStVarTwo, &pStVarTwo);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		if ( (m2 != n2) && (n2 != 1) ) 
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,2);
			return 0;
		}

		pStVarTwo = (char*)MALLOC(sizeof(char)*(lenStVarTwo + 1));
		if (pStVarTwo)
		{
			strErr = getMatrixOfString(pvApiCtx, piAddressVarTwo, &m2, &n2, &lenStVarTwo, &pStVarTwo);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			if ( strcmp(pStVarTwo, "-echo") )
			{
				FREE(pStVarTwo); pStVarTwo = NULL;
				Scierror(999,_("%s: Wrong value for input argument #%d: '%s' expected.\n"),fname,2,"-echo");
				return 0;
			}
			else
			{
				ECHOMODE = TRUE;
			}
		}
		else
		{
			Scierror(999,_("%s: No more memory.\n"),fname);
			return 0;
		}
	}

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

	strErr = getMatrixOfString(pvApiCtx, piAddressVarOne,&m1, &n1, &lenStVarOne, &pStVarOne);
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
	if (pStVarOne)
	{
		double exitCode = 0.;
		BOOL DetachProcessOption = FALSE;
		BOOL *StatusExit = NULL;

		strErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, &lenStVarOne, &pStVarOne);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		DetachProcessOption = DetectDetachProcessInCommandLine(pStVarOne);
		exitCode = (double)spawncommand(pStVarOne, DetachProcessOption);
		FREE(pStVarOne); pStVarOne = NULL;

		StatusExit = (BOOL*)MALLOC(sizeof(BOOL));

		if (DetachProcessOption)
		{
			if ( strlen(pipeErr.OutputBuffer) )
			{
				/* StdErr will be "Output" */
				*StatusExit = FALSE;
				Output = CreateOuput(&pipeErr,DetachProcessOption);
				numberoflines = pipeErr.NumberOfLines;
			}
			else
			{
				/* StdOut will be "Output" */
				*StatusExit = TRUE;
				Output = CreateOuput(&pipeOut,DetachProcessOption);
				numberoflines = pipeOut.NumberOfLines;
			}
		}
		else
		{
			char FileTMPDir[PATH_MAX + 16];
			BOOL bConvert = FALSE;

			char *TMPDirLong = getTMPDIR();
			char *TMPDirShort = getshortpathname(TMPDirLong, &bConvert);

			sprintf(FileTMPDir,"%s\\DOS.OK",TMPDirLong);
			FREE(TMPDirLong); TMPDirLong = NULL;
			FREE(TMPDirShort); TMPDirShort = NULL;

			if (FileExist(FileTMPDir))
			{
				DeleteFile(FileTMPDir);
				/* StdOut will be "Output" */
				*StatusExit = TRUE;
				Output = CreateOuput(&pipeOut,DetachProcessOption);
				numberoflines = pipeOut.NumberOfLines;
			}
			else
			{
				/* StdErr will be "Output" */
				*StatusExit = FALSE;
				Output = CreateOuput(&pipeErr,DetachProcessOption);
				numberoflines = pipeErr.NumberOfLines;
			}
		}

		if (ECHOMODE) PrintOuput(Output, numberoflines);

		if (Lhs == 1)
		{
			int m_out = 1, n_out = 1;
			strErr = createMatrixOfBoolean(pvApiCtx, Rhs + 1, m_out, n_out, StatusExit);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			LhsVar(1) = Rhs + 1;
		}
		else
		{
			int m_out2 = 1;
			int n_out2 = 1;

			if (Output[0])
			{
				int m_out1 = numberoflines;
				int n_out1 = 1;
				strErr = createMatrixOfString(pvApiCtx, Rhs + 1, m_out1, n_out1, Output);
			}
			else
			{
				/* returns [] */
				int m_out1 = 0;
				int n_out1 = 0;
				strErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, m_out1, n_out1, NULL);
			}

			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			LhsVar(1) = Rhs + 1;

			strErr = createMatrixOfBoolean(pvApiCtx, Rhs + 2, m_out2, n_out2, StatusExit);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			LhsVar(2) = Rhs + 2;
		}

		if (Lhs > 2)
		{
			int m_out3 = 1, n_out3 = 1;
			strErr = createMatrixOfDouble(pvApiCtx, Rhs + 3, m_out3, n_out3, &exitCode); 
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			LhsVar(3) = Rhs + 3;
		}

		C2F(putlhsvar)();

		if (StatusExit) {FREE(StatusExit); StatusExit = NULL;}
		freeArrayOfString(Output, numberoflines);

		ClosePipeInfo (pipeOut);
		ClosePipeInfo (pipeErr);

	}
	else
	{
		Scierror(999,_("%s: No more memory.\n"),fname);
	}
	
	return 0;
}
/*--------------------------------------------------------------------------*/
static int PrintOuput(char **ouput,int nbrlines)
{
	if (ouput)
	{
		int i = 0;
		for(i = 0;i<nbrlines;i++) 
		{
			if (ouput[i])
			{
				sciprint("%s\n",ouput[i]);
			}
		}
		return TRUE;
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/

