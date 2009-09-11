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
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "BOOL.h"
#include "api_common.h"
#include "api_double.h"
#include "api_string.h"
#include "strsplitfunction.h"
/*----------------------------------------------------------------------------*/
int sci_strsplit(char *fname,unsigned long fname_len)
{
	int lw = 0;

	int *piAddressVarOne = NULL;
	wchar_t *pStVarOne = NULL;
	int lenStVarOne = 0;
	int m = 0, n = 0;

	/* Check Input & Output parameters */
	CheckRhs(1,3);
	CheckLhs(1,2);

	if (Rhs == 3)
	{
		int *piAddressVarThree = NULL;
		double *pdVarThree = NULL;

		int *piAddressVarTwo = NULL;

		getVarAddressFromPosition(3, &piAddressVarThree);

		if (getVarType(piAddressVarThree) != sci_matrix)
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A scalar expected.\n"), fname, 3);
			return 0;
		}

		getVarDimension(piAddressVarThree, &m, &n);
		if ( (m != n) && (n != 1) ) 
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 3);
			return 0;
		}

		// get value of third argument
		getMatrixOfDouble(piAddressVarThree, &m, &n, &pdVarThree);

		if ( ((int)pdVarThree[0] < 1) && ((int)pdVarThree[0] != -1) )
		{
			Scierror(999,_("%s: Wrong value for input argument #%d: A value > 0 expected.\n"), fname, 3);
			return 0;
		}

		getVarAddressFromPosition(2, &piAddressVarTwo);

		if (getVarType(piAddressVarTwo) != sci_strings) 
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,2);
			return 0;
		}
	}

	getVarAddressFromPosition(1, &piAddressVarOne);

	if (getVarType(piAddressVarOne) == sci_matrix)
	{
		getVarDimension(piAddressVarOne, &m, &n);
		if ( (m != n) && (n != 0) ) 
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
			return 0;
		}

		// strsplit([], ...) returns []

		createMatrixOfDouble(Rhs + 1, 0, 0, NULL);
		LhsVar(1) = Rhs + 1;
		C2F(putlhsvar)();
		return 0;
	}

	if (getVarType(piAddressVarOne) != sci_strings)
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
		return 0;
	}

	if ( (m != n) && (n != 1) ) 
	{
		Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,1);
		return 0;
	}

	if (Rhs == 2)
	{
		int *piAddressVarTwo = NULL;

		getVarAddressFromPosition(2, &piAddressVarTwo);

		if (getVarType(piAddressVarTwo) == sci_matrix)
		{
			int m2 = 0, n2 = 0;
			double *pdVarTwo = NULL;

			if (Lhs == 2)
			{
				Scierror(78,_("%s: Wrong number of output arguments: %d expected.\n"), fname, 1);
				return 0;
			}

			// get value of first argument only now
			pStVarOne = (wchar_t*)MALLOC(sizeof(wchar_t)*(lenStVarOne + 1));
			if (pStVarOne == NULL)
			{
				Scierror(999,_("%s : Memory allocation error.\n"),fname);
				return 0;
			}
			getMatrixOfWideString(piAddressVarOne,&m,&n,&lenStVarOne,&pStVarOne);

			// get value of second argument
			getMatrixOfDouble(piAddressVarTwo, &m2, &n2, &pdVarTwo);

			if ( (m2 == 1) || (n2 == 1) ) 
			{
				int i = 0;
				int m_out = 0, n_out = 0;
				strsplit_error ierr = STRSPLIT_NO_ERROR;

				wchar_t **results = strsplitfunction(pStVarOne, pdVarTwo, m2 * n2, &ierr);

				switch (ierr)
				{
				case STRSPLIT_NO_ERROR:
					{
						m_out = (m2 * n2) + 1;
						n_out = 1;

						createMatrixOfWideString(Rhs + 1, m_out, n_out, results);

						LhsVar(1) = Rhs + 1;
						C2F(putlhsvar)();

						freeArrayOfWideString(results, m_out);
					}
					break;
				case STRSPLIT_INCORRECT_VALUE_ERROR:
					{
						Scierror(116,_("%s: Wrong size for input argument #%d.\n"),fname, 2);
					}
					break;
				case STRSPLIT_INCORRECT_ORDER_ERROR:
					{
						Scierror(99,_("%s: Elements of %dth argument must be in increasing order.\n"), fname, 2);
					}
					break;
				case STRSPLIT_MEMORY_ALLOCATION_ERROR:
					{
						Scierror(999,_("%s : Memory allocation error.\n"), fname);
					}
					break;
				default:
					{
						Scierror(999,_("%s: error.\n"),fname);
					}
					break;
				}
			}
			else
			{
				Scierror(999,_("%s: Wrong size for input argument #%d.\n"),fname, 2);
			}

			if (pStVarOne) {FREE(pStVarOne); pStVarOne = NULL;}

			return 0;
		}
		else if (getVarType(piAddressVarTwo) != sci_strings)
		{
			Scierror(999,_("%s: Wrong size for input argument #%d.\n"),fname, 2);
			return 0;
		}
		else /* sci_strings */
		{
			int m = 0, n = 0;
			int *piAddressVarTwo = NULL;

			getVarAddressFromPosition(2, &piAddressVarTwo);

			if (getVarType(piAddressVarTwo) != sci_strings)
			{
				Scierror(999,_("%s: Wrong type for input argument #%d: strings expected.\n"), fname, 2);
				return 0;
			}

			getVarDimension(piAddressVarTwo, &m, &n);
			if ( (m != n) && ((m * n) != 1) )
			{
				int k = 0;
				wchar_t **pStVarTwo = NULL;
				int *lenStVarTwo = NULL;

				lenStVarTwo = (int *)MALLOC(sizeof(int) * (m * n));
				if (lenStVarTwo == NULL)
				{
					Scierror(999,_("%s : Memory allocation error.\n"), fname);
					return 0;
				}
				getMatrixOfWideString(piAddressVarTwo, &m, &n, lenStVarTwo, pStVarTwo);

				pStVarTwo = (wchar_t **)MALLOC(sizeof(wchar_t *) * (m * n));
				if (pStVarTwo== NULL)
				{
					if (lenStVarTwo) {FREE(lenStVarTwo); lenStVarTwo = NULL;}
					Scierror(999,_("%s : Memory allocation error.\n"), fname);
					return 0;
				}

				getMatrixOfWideString(piAddressVarTwo, &m, &n, lenStVarTwo, pStVarTwo);

				/* checks that 2nd parameter is not a array of regexp pattern */
				if (pStVarTwo)
				{
					for (k = 0; k < m * n; k++)
					{
						if (pStVarTwo[k])
						{
							int lenStr = (int)wcslen(pStVarTwo[k]);
							if (lenStr > 2)
							{
								if ( (pStVarTwo[k][0] == '/') && (pStVarTwo[k][lenStr - 1] == '/') )
								{
									Scierror(999,_("%s: Wrong value for input argument #%d: a string expected, not a regexp pattern.\n"), fname, 2);
									if (lenStVarTwo) {FREE(lenStVarTwo); lenStVarTwo = NULL;}
									freeArrayOfWideString(pStVarTwo, m * n);
									return 0;
								}
							}
						}
					}
				}
				if (lenStVarTwo) {FREE(lenStVarTwo); lenStVarTwo = NULL;}
				freeArrayOfWideString(pStVarTwo, m * n);
			}
		}
	}

	// others cases , we call %_strsplit macro
	C2F(overload)(&lw, fname, fname_len);

	return 0;
}
/*----------------------------------------------------------------------------*/
