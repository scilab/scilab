
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU , Allan CORNET
 * Copyright (C) DIGITEO - 2009 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*------------------------------------------------------------------------*/
#include <string.h>
#include "gw_string.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "api_scilab.h"
#include "freeArrayOfString.h"
#include "strsubst.h"
#include "localization.h"
#include "Scierror.h"
#include "pcre_error.h"
#include "pcre_private.h"
#include "BOOL.h"
/*-------------------------------------------------------------------------------------*/
#define CHAR_R "r"
#define CHAR_S "s"
/*-------------------------------------------------------------------------------------*/
int sci_strsubst(char *fname,unsigned long fname_len)
{
	StrErr strErr;
	int *piAddressVarOne	= NULL;
	int iType							= 0;
	int mOne	= 0, nOne		= 0;

	CheckRhs(3,4);
	CheckLhs(1,1);

	strErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}
	strErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	if (iType == sci_strings)
	{
		char **pStVarOne			= NULL;
		int *lenStVarOne			= NULL;

		int *piAddressVarTwo	= NULL;
		int iType2						= 0;
		int mTwo = 0, nTwo		= 0;
		char *pStVarTwo				= NULL;
		int lenStVarTwo				= 0;

		int *piAddressVarThree = NULL;
		int mThree = 0, nThree = 0;
		char *pStVarThree				= NULL;
		int lenStVarThree				= 0;
		int iType3							= 0;

		BOOL isRegExp = FALSE;

		if (Rhs == 4)
		{
			int *piAddressVarFour = NULL;
			int iType4						= 0;
			int mFour = 0, nFour	= 0;
			char *pStVarFour			= NULL;
			int lenStVarFour			= 0;

			strErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddressVarFour);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			strErr = getVarType(pvApiCtx, piAddressVarFour, &iType4);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			if (iType4  != sci_strings )
			{
				Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,4);
				return 0;
			}

			strErr = getMatrixOfString(pvApiCtx, piAddressVarFour,&mFour,&nFour,&lenStVarFour,&pStVarFour);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			if ( (mFour != nFour) && (nFour != 1) ) 
			{
				Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,4);
				return 0;
			}

			pStVarFour = (char*)MALLOC(sizeof(char)*(lenStVarFour + 1));
			if (pStVarFour)
			{
				strErr = getMatrixOfString(pvApiCtx, piAddressVarFour, &mFour, &nFour, &lenStVarFour, &pStVarFour);
				if(strErr.iErr)
				{
					printError(&strErr, 0);
					return 0;
				}

				if ( (strcmp(pStVarFour,CHAR_R) == 0) || (strcmp(pStVarFour,CHAR_S) == 0) )
				{
					if (strcmp(pStVarFour,CHAR_R) == 0)
					{
						isRegExp = TRUE;
					}
					else
					{
						isRegExp = FALSE;
					}
				}
				else
				{
					FREE(pStVarFour); pStVarFour = NULL;
					Scierror(999,_("%s: Wrong value for input argument #%d: 's' or 'r' expected.\n"),fname,4);
					return 0;
				}
			}
			else
			{
				Scierror(999,_("%s: No more memory.\n"),fname);
				return 0;
			}
		}

		strErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		// checks type 3th input argument
		strErr = getVarType(pvApiCtx, piAddressVarThree, &iType3);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		if (iType3  != sci_strings )
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,3);
			return 0;
		}

		// checks dimension 3th input argument
		strErr = getMatrixOfString(pvApiCtx, piAddressVarThree,&mThree,&nThree,&lenStVarThree,&pStVarThree);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		if ( (mThree != nThree) && (nThree != 1) ) 
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,3);
			return 0;
		}

		strErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}


		// checks type 2nd input argument
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

		// checks dimension 2nd input argument
		strErr = getMatrixOfString(pvApiCtx, piAddressVarTwo,&mTwo,&nTwo,&lenStVarTwo,&pStVarTwo);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		if ( (mTwo != nTwo) && (nTwo != 1) ) 
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,2);
			return 0;
		}

		strErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		// checks type 1st input argument
		strErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
		if (iType  != sci_strings )
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
			return 0;
		}

		// get length 3th input argument
		strErr = getMatrixOfString(pvApiCtx, piAddressVarThree, &mThree, &nThree, &lenStVarThree, &pStVarThree);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		pStVarThree = (char*)MALLOC(sizeof(char)*(lenStVarThree + 1));
		if (pStVarThree)
		{
			// get value 3th input argument
			strErr = getMatrixOfString(pvApiCtx, piAddressVarThree, &mThree, &nThree, &lenStVarThree, &pStVarThree);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			// get length 2nd input argument
			strErr = getMatrixOfString(pvApiCtx, piAddressVarTwo, &mTwo, &nTwo, &lenStVarTwo, &pStVarTwo);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			pStVarTwo = (char*)MALLOC(sizeof(char)*(lenStVarTwo + 1));
			if (pStVarTwo)
			{
				// get value 2nd input argument
				strErr = getMatrixOfString(pvApiCtx, piAddressVarTwo, &mTwo, &nTwo, &lenStVarTwo, &pStVarTwo);
				if(strErr.iErr)
				{
					printError(&strErr, 0);
					return 0;
				}

				strErr = getMatrixOfString(pvApiCtx, piAddressVarOne,&mOne,&nOne,lenStVarOne,pStVarOne);
				if(strErr.iErr)
				{
					printError(&strErr, 0);
					return 0;
				}

				lenStVarOne = (int *)MALLOC(sizeof(int) * (mOne * nOne));
				if (lenStVarOne)
				{
					strErr = getMatrixOfString(pvApiCtx, piAddressVarOne,&mOne,&nOne,lenStVarOne,pStVarOne);
					if(strErr.iErr)
					{
						printError(&strErr, 0);
						return 0;
					}

					pStVarOne = (char **)MALLOC(sizeof(char *) * (mOne * nOne));
					if (pStVarOne)
					{
						char **Output_StringMatrix = NULL;

						int i = 0;
						for (i = 0; i < mOne * nOne; i++)
						{
							pStVarOne[i] = (char*)MALLOC(sizeof(char) * (lenStVarOne[i] + 1));
							if (pStVarOne[i] == NULL)
							{
								FREE(pStVarThree); pStVarThree = NULL;
								FREE(pStVarTwo); pStVarTwo = NULL;
								FREE(lenStVarOne); lenStVarOne = NULL;
								freeArrayOfString(pStVarOne, i);
								Scierror(999,_("%s: No more memory.\n"), fname);
								return 0;
							}
						}

						strErr = getMatrixOfString(pvApiCtx, piAddressVarOne,&mOne,&nOne,lenStVarOne,pStVarOne);
						if(strErr.iErr)
						{
							printError(&strErr, 0);
							return 0;
						}


						FREE(lenStVarOne); lenStVarOne = NULL;

						if (isRegExp)
						{
							int ierr = (int)PCRE_FINISHED_OK;
							Output_StringMatrix = strsubst_reg(pStVarOne, mOne * nOne, pStVarTwo, pStVarThree, &ierr);
							if ( (ierr != PCRE_FINISHED_OK) && (ierr != NO_MATCH) && (ierr != PCRE_EXIT) )
							{
								FREE(pStVarThree); pStVarThree = NULL;
								FREE(pStVarTwo); pStVarTwo = NULL;
								pcre_error(fname, ierr);
								return 0;
							}
						}
						else
						{
							Output_StringMatrix = strsubst(pStVarOne, mOne * nOne, pStVarTwo, pStVarThree);
						}

						FREE(pStVarThree); pStVarThree = NULL;
						FREE(pStVarTwo); pStVarTwo = NULL;

						strErr = createMatrixOfString(pvApiCtx, Rhs + 1, mOne, nOne, Output_StringMatrix);
						if(strErr.iErr)
						{
							printError(&strErr, 0);
							return 0;
						}

						LhsVar(1) = Rhs + 1 ;
						C2F(putlhsvar)();
						return 0;
					}
					else
					{
						FREE(pStVarThree); pStVarThree = NULL;
						FREE(pStVarTwo); pStVarTwo = NULL;
						FREE(lenStVarOne); lenStVarOne = NULL;
						Scierror(999,_("%s: No more memory.\n"),fname);
						return 0;
					}
				}
				else
				{
					FREE(pStVarThree); pStVarThree = NULL;
					FREE(pStVarTwo); pStVarTwo = NULL;
					Scierror(999,_("%s: No more memory.\n"),fname);
					return 0;
				}
			}
			else
			{
				FREE(pStVarThree); pStVarThree = NULL;
				Scierror(999,_("%s: No more memory.\n"),fname);
				return 0;
			}
		}
		else
		{
			Scierror(999,_("%s: No more memory.\n"),fname);
			return 0;
		}
	}
	else if (iType == sci_matrix)
	{
		strErr = getVarDimension(pvApiCtx, piAddressVarOne, &mOne, &nOne);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		if ( (mOne == 0) && (nOne == 0) ) /* strsubst([],...) returns [] */
		{
			strErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, mOne, nOne, NULL);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			LhsVar(1) = Rhs + 1 ;
			C2F(putlhsvar)();
			return 0;
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: Matrix of strings or empty matrix expected.\n"), fname);
			return 0;
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
		return 0;
	}
	return 0;
}
/*-------------------------------------------------------------------------------------*/
