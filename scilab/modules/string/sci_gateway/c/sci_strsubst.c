
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
#include "api_common.h"
#include "api_string.h"
#include "api_double.h"
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
	int *piAddressVarOne = NULL;
	int mOne = 0, nOne = 0;

	CheckRhs(3,4);
	CheckLhs(1,1);

	getVarAddressFromPosition(1, &piAddressVarOne);

	if (getVarType(piAddressVarOne) == sci_strings)
	{
		char **pStVarOne = NULL;
		int *lenStVarOne = NULL;

		int *piAddressVarTwo = NULL;
		int mTwo = 0, nTwo = 0;
		char *pStVarTwo = NULL;
		int lenStVarTwo = 0;

		int *piAddressVarThree = NULL;
		int mThree = 0, nThree = 0;
		char *pStVarThree = NULL;
		int lenStVarThree = 0;

		BOOL isRegExp = FALSE;

		if (Rhs == 4)
		{
			int *piAddressVarFour = NULL;
			int mFour = 0, nFour = 0;
			char *pStVarFour = NULL;
			int lenStVarFour = 0;

			getVarAddressFromPosition(4, &piAddressVarFour);

			if ( getVarType(piAddressVarFour) != sci_strings )
			{
				Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,4);
				return 0;
			}

			getMatrixOfString(piAddressVarFour,&mFour,&nFour,&lenStVarFour,&pStVarFour);
			if ( (mFour != nFour) && (nFour != 1) ) 
			{
				Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,4);
				return 0;
			}

			pStVarFour = (char*)MALLOC(sizeof(char)*(lenStVarFour + 1));
			if (pStVarFour)
			{
				getMatrixOfString(piAddressVarFour, &mFour, &nFour, &lenStVarFour, &pStVarFour);
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

		getVarAddressFromPosition(3, &piAddressVarThree);

		// checks type 3th input argument
		if ( getVarType(piAddressVarThree) != sci_strings )
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,3);
			return 0;
		}

		// checks dimension 3th input argument
		getMatrixOfString(piAddressVarThree,&mThree,&nThree,&lenStVarThree,&pStVarThree);
		if ( (mThree != nThree) && (nThree != 1) ) 
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,3);
			return 0;
		}

		getVarAddressFromPosition(2, &piAddressVarTwo);

		// checks type 2nd input argument
		if ( getVarType(piAddressVarTwo) != sci_strings )
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,2);
			return 0;
		}

		// checks dimension 2nd input argument
		getMatrixOfString(piAddressVarTwo,&mTwo,&nTwo,&lenStVarTwo,&pStVarTwo);
		if ( (mTwo != nTwo) && (nTwo != 1) ) 
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,2);
			return 0;
		}

		getVarAddressFromPosition(1, &piAddressVarOne);

		// checks type 1st input argument
		if ( getVarType(piAddressVarOne) != sci_strings )
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
			return 0;
		}

		// get length 3th input argument
		getMatrixOfString(piAddressVarThree, &mThree, &nThree, &lenStVarThree, &pStVarThree);
		pStVarThree = (char*)MALLOC(sizeof(char)*(lenStVarThree + 1));
		if (pStVarThree)
		{
			// get value 3th input argument
			getMatrixOfString(piAddressVarThree, &mThree, &nThree, &lenStVarThree, &pStVarThree);

			// get length 2nd input argument
			getMatrixOfString(piAddressVarTwo, &mTwo, &nTwo, &lenStVarTwo, &pStVarTwo);
			pStVarTwo = (char*)MALLOC(sizeof(char)*(lenStVarTwo + 1));
			if (pStVarTwo)
			{
				// get value 2nd input argument
				getMatrixOfString(piAddressVarTwo, &mTwo, &nTwo, &lenStVarTwo, &pStVarTwo);

				getMatrixOfString(piAddressVarOne,&mOne,&nOne,lenStVarOne,pStVarOne);

				lenStVarOne = (int *)MALLOC(sizeof(int) * (mOne * nOne));
				if (lenStVarOne)
				{
					getMatrixOfString(piAddressVarOne,&mOne,&nOne,lenStVarOne,pStVarOne);

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

						getMatrixOfString(piAddressVarOne,&mOne,&nOne,lenStVarOne,pStVarOne);

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

						createMatrixOfString(Rhs + 1, mOne, nOne, Output_StringMatrix);
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
	else if (getVarType(piAddressVarOne) == sci_matrix)
	{
		getVarDimension(piAddressVarOne, &mOne, &nOne);
		if ( (mOne == 0) && (nOne == 0) ) /* strsubst([],...) returns [] */
		{
			createMatrixOfDouble(Rhs + 1, mOne, nOne, NULL);
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
