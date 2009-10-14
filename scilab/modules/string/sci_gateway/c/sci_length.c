
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA 2007 - Cong WU
 * Copyright (C) INRIA 2008 - Allan CORNET
 * Copyright (C) DIGITEO 2009 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* desc : For usual or polynomial matrix  n  is the int equal to
   number of rows times number of columns of  M . (Also valid for  M
   a boolean matrix)

   For matrices made of character strings (and in particular for a
   character string)  length  returns in  n  the length of entries of
   the matrix of character strings  M .

   The length of a list is the number of elements in the list
   (also given by  size ).

   length('123')  is  3 .  length([1,2;3,4])  is  4 .                     */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "gw_string.h"
#include "stack-c.h"
#include "api_scilab.h"
#include "core_math.h"
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
#include "charEncoding.h"
/*----------------------------------------------------------------------------*/
/* get length */
static int lengthStrings(int *piAddressVar);
static int lengthOthers(char *fname);
static int lengthDefault(int *piAddressVar);
static int lengthList(int *piAddressVar);
/* !!! WARNING !!! : Read comments about length on sparse matrix */
static int lengthSparse(int *piAddressVar);
/*----------------------------------------------------------------------------*/
int sci_length(char *fname,unsigned long fname_len)
{
	int *piAddressVarOne = NULL;
	int iScilabType = 0;

	/* get Address of inputs */
	StrErr strErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	strErr = getVarType(pvApiCtx, piAddressVarOne, &iScilabType);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	switch ( iScilabType )
	{
	case sci_strings :
		{
			return lengthStrings(piAddressVarOne);
		}
	case sci_sparse :
		{
			return lengthSparse(piAddressVarOne);
		}
	case sci_list :
	case sci_tlist :
	case sci_mlist :
		{
			return lengthList(piAddressVarOne);
		}
		break;

	case sci_matrix : case sci_poly : case sci_boolean : case sci_boolean_sparse : 
	case sci_matlab_sparse : case sci_ints : case sci_handles : 
		{
			return lengthDefault(piAddressVarOne);
		}
	default :
		return lengthOthers(fname);
		break;
	}
}
/*--------------------------------------------------------------------------*/
static int lengthStrings(int *piAddressVar)
{
	StrErr strErr;
	int m1 = 0, n1 = 0;
	int iType				= 0;
	char **pStVarOne = NULL;
	int *lenStVarOne = NULL;

	int m_out = 0, n_out = 0;
	int *piAddressOut = NULL;
	double *pdOut = NULL;
	int i = 0;

	int ierr = 0;

	strErr = getVarType(pvApiCtx, piAddressVar, &iType);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	if ( iType != sci_strings )
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),"length",1);
		return 0;
	}

	strErr = getMatrixOfString(pvApiCtx, piAddressVar, &m1, &n1, lenStVarOne, pStVarOne);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		Scierror(999,_("%s: impossible to get dimensions of this matrix.\n"),"length");
		return 0;
	}

	lenStVarOne = (int*)MALLOC(sizeof(int) * (m1*n1));
	if (lenStVarOne == NULL)
	{
		Scierror(999,_("%s: No more memory.\n"),"length");
		return 0;
	}

	strErr = getMatrixOfString(pvApiCtx, piAddressVar, &m1, &n1, lenStVarOne, pStVarOne);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		FREE(lenStVarOne); lenStVarOne = NULL;
		Scierror(999,_("%s: impossible to get dimensions of this matrix.\n"),"length");
		return 0;
	}

	pStVarOne = (char**)MALLOC(sizeof(char*) * (m1*n1));
	if (pStVarOne == NULL)
	{
		Scierror(999,_("%s: No more memory.\n"),"length");
		return 0;
	}

	for (i = 0; i < m1 * n1; i++)
	{
		pStVarOne[i] = (char*)MALLOC(sizeof(char) * (lenStVarOne[i] + 1));
		if (pStVarOne[i] == NULL)
		{
			FREE(lenStVarOne); lenStVarOne = NULL;
			freeArrayOfString(pStVarOne, i);
			Scierror(999,_("%s: No more memory.\n"),"length");
			return 0;
		}
	}

	strErr = getMatrixOfString(pvApiCtx, piAddressVar, &m1, &n1, lenStVarOne, pStVarOne);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		FREE(lenStVarOne); lenStVarOne = NULL;
		freeArrayOfString(pStVarOne, m1 * n1);
		Scierror(999,_("%s: impossible to get dimensions of this matrix.\n"),"length");
		return 0;
	}

	m_out = m1;  n_out = n1;
	pdOut = (double*)MALLOC(sizeof(double) * (m_out * n_out));

	if (pdOut == NULL)
	{
		FREE(lenStVarOne); lenStVarOne = NULL;
		freeArrayOfString(pStVarOne, m1 * n1);
		Scierror(999,_("%s: No more memory.\n"),"length");
		return 0;
	}

	for (i = 0; i < m_out * n_out; i++)
	{
		int clen = (int)strlen(pStVarOne[i]);
		int scilen = lenStVarOne[i];

		int trueLength = 0;

		wchar_t *wcStr = NULL;

		if (scilen > clen)  
		{
			int j = 0;
			/* bug 4727 */
			/* A scilab string is a array of characters */
			/* we can put '\0' in a scilab string */
			for (j = 0; j < lenStVarOne[i]; j++)
			{
				if (pStVarOne[i][j] == 0) 
				{
					pStVarOne[i][j] = ' ';
				}
			}

			wcStr = to_wide_string(pStVarOne[i]);
			if (wcStr) 
			{
				trueLength = (int) wcslen(wcStr);
				FREE(wcStr); wcStr = NULL;
			}
		}
		else 
		{
			wcStr = to_wide_string(pStVarOne[i]);
			if (wcStr) 
			{
				trueLength = (int) wcslen(wcStr);
				FREE(wcStr); wcStr = NULL;
			}
		}

		pdOut[i] = (double)trueLength;
	}

	freeArrayOfString(pStVarOne,  m_out * n_out);

	FREE(lenStVarOne); lenStVarOne = NULL;

	strErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, m_out, n_out, pdOut);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	LhsVar(1) = Rhs + 1; 
	C2F(putlhsvar)();

	FREE(pdOut); pdOut = NULL;
	return 0;	
}
/*--------------------------------------------------------------------------*/
static int lengthOthers(char *fname)
{
	/* unknown type */
	Scierror(999, _("%s: Wrong type for input argument(s).\n"),fname);
	return 0;
}
/*--------------------------------------------------------------------------*/
/* !!! WARNING !!! */
/* Compatibility with Scilab 4.x */
/* length returned is the max of dimensions of the sparse matrix max(m,n) */
/* and not m * n */
static int lengthSparse(int *piAddressVar)
{
	int m_out = 0, n_out = 0;
	int *piAddressOut = NULL;
	double *pdOut = NULL;

	int m = 0, n = 0;
	StrErr strErr = getVarDimension(pvApiCtx, piAddressVar, &m, &n);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	m_out = 1;  n_out = 1;
	pdOut = (double*)MALLOC(sizeof(double) * (m_out * n_out));
	if (pdOut == NULL)
	{
		Scierror(999,_("%s: No more memory.\n"),"length");
		return 0;
	}

	pdOut[0] = Max(m,n);

	strErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, m_out, n_out, pdOut);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	LhsVar(1) = Rhs + 1; 
	C2F(putlhsvar)();

	FREE(pdOut); pdOut = NULL;
	return 0;
}
/*--------------------------------------------------------------------------*/
static int lengthList(int *piAddressVar)
{
	int m_out = 0, n_out = 0;
	int *piAddressOut = NULL;
	double *pdOut = NULL;

	int nbItem = 0;
	StrErr strErr = getListItemNumber(pvApiCtx, piAddressVar, &nbItem);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	m_out = 1;  n_out = 1;
	pdOut = (double*)MALLOC(sizeof(double) * (m_out * n_out));
	if (pdOut == NULL)
	{
		Scierror(999,_("%s: No more memory.\n"),"length");
		return 0;
	}

	pdOut[0] = (double) nbItem;

	strErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, m_out, n_out, pdOut);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	LhsVar(1) = Rhs + 1; 
	C2F(putlhsvar)();

	FREE(pdOut); pdOut = NULL;
	return 0;
}
/*--------------------------------------------------------------------------*/
static int lengthDefault(int *piAddressVar)
{
	int m_out = 0, n_out = 0;
	int *piAddressOut = NULL;
	double *pdOut = NULL;

	int m = 0, n = 0;
	StrErr strErr = getVarDimension(pvApiCtx, piAddressVar, &m, &n);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	m_out = 1;  n_out = 1;
	pdOut = (double*)MALLOC(sizeof(double) * (m_out * n_out));
	if (pdOut == NULL)
	{
		Scierror(999,_("%s: No more memory.\n"),"length");
		return 0;
	}

	pdOut[0] = m * n;

	strErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, m_out, n_out, pdOut);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	LhsVar(1) = Rhs + 1; 
	C2F(putlhsvar)();

	FREE(pdOut); pdOut = NULL;
	return 0;
}
/*--------------------------------------------------------------------------*/
