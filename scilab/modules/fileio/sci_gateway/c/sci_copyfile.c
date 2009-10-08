/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
#ifndef _MSC_VER
#include <errno.h>
#endif 
#include "gw_fileio.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "localization.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "copyfile.h"
#include "FileExist.h"
#include "isdir.h"
#include "splitpath.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
static wchar_t* getFilenameWithExtension(wchar_t* wcFullFilename);
static void returnCopyFileResultOnStack(int ierr, char *fname);
/*--------------------------------------------------------------------------*/
int sci_copyfile(char *fname,unsigned long fname_len)
{
	StrErr strErr;
	int *piAddressVarOne = NULL;
	wchar_t *pStVarOne = NULL;
	int iType1 = 0;
	int lenStVarOne = 0;
	int m1 = 0, n1 = 0;

	int *piAddressVarTwo = NULL;
	wchar_t *pStVarTwo = NULL;
	int iType2 = 0;
	int lenStVarTwo = 0;
	int m2 = 0, n2 = 0;

	/* Check Input & Output parameters */
	CheckRhs(2,2);
	CheckLhs(1,2);

	strErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	strErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
	if (iType1 != sci_strings)
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
		return 0;
	}

	strErr = getVarDimension(pvApiCtx, piAddressVarOne, &m1, &n1);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	if ( (m1 != n1) && (n1 != 1) ) 
	{
		Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
		return 0;
	}

	// get value of first argument only now (+ 2 if we add file separator @ the end)
	pStVarOne = (wchar_t*)MALLOC(sizeof(wchar_t)*(lenStVarOne + 2));
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

	if (iType2 != sci_strings)
	{
		if (pStVarOne) {FREE(pStVarOne); pStVarOne = NULL;}
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
		return 0;
	}

	strErr = getVarDimension(pvApiCtx, piAddressVarTwo, &m2, &n2);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	if ( (m2 != n2) && (n2 != 1) ) 
	{
		if (pStVarOne) {FREE(pStVarOne); pStVarOne = NULL;}
		Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 2);
		return 0;
	}

	// get value of first argument only now (+ 2 if we add file separator @ the end)
	pStVarTwo = (wchar_t*)MALLOC(sizeof(wchar_t)*(lenStVarTwo + 2));
	if (pStVarTwo == NULL)
	{
		if (pStVarOne) {FREE(pStVarOne); pStVarOne = NULL;}
		Scierror(999,_("%s : Memory allocation error.\n"),fname);
		return 0;
	}
	
	strErr = getMatrixOfWideString(pvApiCtx, piAddressVarTwo, &m2, &n2, &lenStVarTwo, &pStVarTwo);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	if ( isdirW(pStVarOne) || FileExistW(pStVarOne) )
	{
		int ierrCopy = 0;

		if ( isdirW(pStVarOne) || FileExistW(pStVarOne) )
		{
			
			if ( isdirW(pStVarOne)  )
			{
				/* copy a directory into a directory */
				ierrCopy = CopyDirectoryFunction(pStVarTwo, pStVarOne);
			}
			else if (FileExistW(pStVarOne))
			{
				if (isdirW(pStVarTwo)) 
				{
					/* copy file into a existing directory */
					wchar_t* filename = getFilenameWithExtension(pStVarOne);
					if (filename)
					{
						#define FORMAT_FULLFILENAME "%s/%s"
						wchar_t* destFullFilename = NULL;

						/* remove last file separator if it exists */
						if ( (pStVarTwo[wcslen(pStVarTwo) - 1] == L'\\') || 
							(pStVarTwo[wcslen(pStVarTwo) - 1] == L'/') )
						{
							pStVarTwo[wcslen(pStVarTwo) - 1] = L'\0';
						}

						destFullFilename = (wchar_t*)MALLOC(sizeof(wchar_t)* ((int)wcslen(pStVarTwo) +
							(int)wcslen(filename) + (int)wcslen(L"/") + 1));
						wcscpy(destFullFilename, pStVarTwo);
						wcscat(destFullFilename, L"/");
						wcscat(destFullFilename, filename);

						ierrCopy = CopyFileFunction(destFullFilename, pStVarOne);

						FREE(filename); filename = NULL;
						FREE(destFullFilename); destFullFilename = NULL;
					}
					else
					{
						if (pStVarOne) {FREE(pStVarOne); pStVarOne = NULL;}
						if (pStVarTwo) {FREE(pStVarTwo); pStVarTwo = NULL;}

						Scierror(999,_("%s : Memory allocation error.\n"),fname);
						return 0;
					}
				}
				else 
				{
					/* copy a file into a file */
					ierrCopy = CopyFileFunction(pStVarTwo, pStVarOne);
				}
			}
			else
			{
				if (pStVarOne) {FREE(pStVarOne); pStVarOne = NULL;}
				if (pStVarTwo) {FREE(pStVarTwo); pStVarTwo = NULL;}
				Scierror(999,_("%s: Wrong value for input argument #%d: A valid filename or directory expected.\n"), fname, 1);
			}
		}
		else
		{
			if (pStVarOne) {FREE(pStVarOne); pStVarOne = NULL;}
			if (pStVarTwo) {FREE(pStVarTwo); pStVarTwo = NULL;}

			Scierror(999,_("%s: Wrong value(s) for input argument(s).\n"), fname);
			return 0;
		}

		returnCopyFileResultOnStack(ierrCopy, fname);
	}
	else
	{
		Scierror(999,_("%s: Wrong value for input argument #%d: A valid filename or directory expected.\n"), fname, 1);
	}

	if (pStVarOne) {FREE(pStVarOne); pStVarOne = NULL;}
	if (pStVarTwo) {FREE(pStVarTwo); pStVarTwo = NULL;}

	return 0;
}
/*--------------------------------------------------------------------------*/
static wchar_t* getFilenameWithExtension(wchar_t* wcFullFilename)
{
	wchar_t* wcfilename = NULL;

	if (wcFullFilename)
	{
		wchar_t *wcdrv = MALLOC(sizeof(wchar_t*) * ((int)wcslen(wcFullFilename) + 1));
		wchar_t* wcdir = MALLOC(sizeof(wchar_t*) * ((int)wcslen(wcFullFilename) + 1));
		wchar_t* wcname = MALLOC(sizeof(wchar_t*) * ((int)wcslen(wcFullFilename) + 1));
		wchar_t* wcext = MALLOC(sizeof(wchar_t*) * ((int)wcslen(wcFullFilename) + 1));
		wcfilename = MALLOC(sizeof(wchar_t*) * ((int)wcslen(wcFullFilename) + 1));

		splitpathW(wcFullFilename, FALSE, wcdrv, wcdir, wcname, wcext);

		wcscpy(wcfilename, wcname);
		wcscat(wcfilename, wcext);

		if (wcdrv) {FREE(wcdrv); wcdrv = NULL;}
		if (wcdir) {FREE(wcdir); wcdir = NULL;}
		if (wcname) {FREE(wcname); wcname = NULL;}
		if (wcext) {FREE(wcext); wcext = NULL;}
	}
	return wcfilename;
}
/*--------------------------------------------------------------------------*/
static void returnCopyFileResultOnStack(int ierr, char *fname)
{
	double dError = 0.;
	wchar_t **strError = NULL;
	int m_out = 1, n_out = 1;

	strError = (wchar_t**)MALLOC(sizeof(wchar_t*)* 1);
	if (strError == NULL)
	{
		Scierror(999,_("%s : Memory allocation error.\n"),fname);
		return;
	}

#ifdef _MSC_VER
	if (ierr)
	{
		#define BUFFER_SIZE 1024
		DWORD dw = GetLastError(); 
		wchar_t buffer[BUFFER_SIZE];

		if (FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, 
			dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),buffer, BUFFER_SIZE, NULL) == 0) 
		{
			wcscpy(buffer, L"Unknown Error");
		}

		// Compatibility with previous version , we return 0
		//dError = (double) dw;
		dError = (double) 0.;

		strError[0] = (wchar_t*)MALLOC(sizeof(wchar_t)* ((int)wcslen(buffer) + 1));
		if (strError[0] == NULL)
		{
			Scierror(999,_("%s : Memory allocation error.\n"),fname);
			return;
		}

		wcscpy(strError[0], buffer);
	}
	else
	{
		dError = 1.;
		strError[0] = (wchar_t*)MALLOC(sizeof(wchar_t)* 1);
		if (strError[0] == NULL)
		{
			Scierror(999,_("%s : Memory allocation error.\n"),fname);
			return;
		}
		wcscpy(strError[0], L"");
	}
#else
	if (ierr)
	{
		// Compatibility with previous version , we return 0
		//dError = (double) ierr;
		//dError = (double) 0.;
		strError[0] = to_wide_string(strerror(errno));
		if (strError[0] == NULL)
		{
			Scierror(999,_("%s : Memory allocation error.\n"),fname);
			return;
		}
	}
	else
	{
		dError = 1.;
		strError[0] = (wchar_t*)MALLOC(sizeof(wchar_t)* 1);
		if (strError[0] == NULL)
		{
			Scierror(999,_("%s : Memory allocation error.\n"),fname);
			return;
		}
		wcscpy(strError[0], L"");
	}
#endif

	createMatrixOfDouble(pvApiCtx, Rhs + 1, m_out, n_out, &dError);
	
	LhsVar(1) = Rhs + 1;

	if (Lhs == 2)
	{
		createMatrixOfWideString(pvApiCtx, Rhs + 2, m_out, n_out, strError);
		LhsVar(2) = Rhs + 2;
	}

	freeArrayOfWideString(strError, 1);

	C2F(putlhsvar)();
}
/*--------------------------------------------------------------------------*/

