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

#include "gw_functions.h"
#include "api_scilab.h"
#include "stack-c.h"
#include "recursionFunction.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intexec)(); /* fortran */
/*--------------------------------------------------------------------------*/
#define ERRCATCH_KEYWORD "errcatch"
/*--------------------------------------------------------------------------*/
int C2F(sci_exec)(char *fname,unsigned long fname_len)
{
	if ( isRecursionCallToFunction() )
	{
		C2F(intexec)(fname, fname_len);
	}
	else
	{
		StrErr strErr;

		int iType1						= 0;
		int iType2						= 0;
		int iType3						= 0;

		int *piAddressVarOne = NULL;
		int *piAddressVarTwo = NULL;
		int *piAddressVarThree = NULL;

		CheckRhs(1,3);
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

		if (Rhs > 1)
		{
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
		}

		if (Rhs > 2)
		{
			strErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			strErr = getVarType(pvApiCtx, piAddressVarThree, &iType3);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}
		}

		if ((iType1 == sci_matrix) || (iType1 == sci_strings) || (iType1 == sci_c_function))
		{
			int m1 = 0, n1 = 0;

			/*  sci_c_function not managed by getVarDimension */
			if (iType1 != sci_c_function)
			{
				strErr = getVarDimension(pvApiCtx, piAddressVarOne, &m1, &n1);
				if(strErr.iErr)
				{
					printError(&strErr, 0);
					return 0;
				}

				if (m1 * n1 != 1)
				{
					Scierror(999,_("%s: Wrong size for input argument #%d.\n"),fname,1);
					return 0;
				}
			}

			if (Rhs > 1)
			{
				if ((iType2 != sci_matrix) && (iType2 != sci_strings))
				{
					Scierror(999,_("%s: Wrong type for input argument #%d: A scalar or a string expected.\n"),fname,2);
					return 0;
				}
				else
				{
					if (iType2 == sci_matrix)
					{	
						int m2 = 0, n2 = 0;
						double *pdVarTwo = NULL;
						int iVarTwo = 0;

						strErr = getMatrixOfDouble(pvApiCtx, piAddressVarTwo,&m2,&n2,&pdVarTwo);
						if(strErr.iErr)
						{
							printError(&strErr, 0);
							return 0;
						}

						if (m2 * n2 != 1)
						{
							Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname,2);
							return 0;
						}

						iVarTwo = (int)pdVarTwo[0];
						if (*pdVarTwo != (double)iVarTwo)
						{
							Scierror(999,_("%s: Wrong value for input argument #%d: A integer expected.\n"),fname,2);
							return 0;
						}
					}
					else /* sci_strings */
					{
						int m2 = 0, n2 = 0;
						char *pStVarTwo = NULL;
						int lenStVarTwo = 0;

						strErr = getMatrixOfString(pvApiCtx, piAddressVarTwo,&m2,&n2,&lenStVarTwo,&pStVarTwo);
						if(strErr.iErr)
						{
							printError(&strErr, 0);
							return 0;
						}

						pStVarTwo = (char*)MALLOC(sizeof(char)*(lenStVarTwo + 1));
						if (pStVarTwo)
						{
							strErr = getMatrixOfString(pvApiCtx, piAddressVarTwo,&m2,&n2,&lenStVarTwo,&pStVarTwo);
							if(strErr.iErr)
							{
								printError(&strErr, 0);
								return 0;
							}

							if (strcmp(pStVarTwo,ERRCATCH_KEYWORD))
							{
								Scierror(999,_("%s: Wrong value for input argument #%d: 'errcatch' expected.\n"),fname,2);
								FREE(pStVarTwo);
								pStVarTwo = NULL;
								return 0;
							}

							FREE(pStVarTwo);
							pStVarTwo = NULL;
						}
						else
						{
							Scierror(999,_("%s : Memory allocation error.\n"),fname);
							return 0;
						}
					}
				}
			}

			if (Rhs > 2)
			{
				BOOL bCombo = FALSE;
				if ((iType3 != sci_matrix) && (iType3 != sci_strings))
				{
					Scierror(999,_("%s: Wrong type for input argument #%d: A scalar or a string expected.\n"),fname,3);
					return 0;
				}
				else
				{
					if (iType3 == sci_matrix)
					{
						int m3 = 0, n3 = 0;
						double *pdVarThree = NULL;
						int iVarThree = 0;

						strErr = getMatrixOfDouble(pvApiCtx, piAddressVarThree,&m3,&n3,&pdVarThree);
						if(strErr.iErr)
						{
							printError(&strErr, 0);
							return 0;
						}

						if (m3 * n3 != 1)
						{
							Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname,3);
							return 0;
						}

						iVarThree = (int)pdVarThree[0];
						if (*pdVarThree != (double)iVarThree)
						{
							Scierror(999,_("%s: Wrong value for input argument #%d: A integer expected.\n"),fname,3);
							return 0;
						}
					}
					else  /* sci_strings */
					{
						int m3 = 0, n3 = 0;
						char *pStVarThree = NULL;
						int lenStVarThree = 0;

						strErr = getMatrixOfString(pvApiCtx, piAddressVarOne,&m3,&n3,&lenStVarThree,&pStVarThree);
						if(strErr.iErr)
						{
							printError(&strErr, 0);
							return 0;
						}

						pStVarThree = (char*)MALLOC(sizeof(char)*(lenStVarThree + 1));
						if (pStVarThree)
						{
							strErr = getMatrixOfString(pvApiCtx, piAddressVarThree,&m3,&n3,&lenStVarThree,&pStVarThree);
							if(strErr.iErr)
							{
								printError(&strErr, 0);
								return 0;
							}

							if (strcmp(pStVarThree,ERRCATCH_KEYWORD))
							{
								Scierror(999,_("%s: Wrong value for input argument #%d: 'errcatch' expected.\n"),fname,3);
								FREE(pStVarThree);
								pStVarThree = NULL;
								return 0;
							}

							FREE(pStVarThree);
							pStVarThree = NULL;
						}
						else
						{
							Scierror(999,_("%s : Memory allocation error.\n"),fname);
							return 0;
						}
					}
				}

				if ((iType2 == sci_matrix) && (iType3 == sci_strings)) bCombo = TRUE;
				if ((iType2 == sci_strings) && (iType3 == sci_matrix)) bCombo = TRUE;

				if (!bCombo)
				{
					Scierror(999,_("%s: Wrong value(s) for input argument(s).\n"),fname,3);
					return 0;
				}
			}

			/* input parameters seem good */
			C2F(intexec)(fname, fname_len);
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A scalar or a string expected.\n"),fname,1);
		}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
