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
#include "gw_core.h"
#include "stack-c.h"
#include "api_common.h"
#include "api_string.h"
#include "api_double.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "searchmacroinlibraries.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sci_whereis(char *fname,unsigned long fname_len)
{
	int *piAddressVarOne = NULL;

	getVarAddressFromPosition(1, &piAddressVarOne);
	if (getVarType(piAddressVarOne) == sci_strings)
	{
		char *pStVarOne = NULL;
		int lenStVarOne = 0;
		int m = 0, n = 0;

		char **librariesResult = NULL;
		int librariesResultSize = 0;

		getMatrixOfString(piAddressVarOne, &m, &n, &lenStVarOne, &pStVarOne);

		if ( (m != n) && (n != 1) ) 
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

		getMatrixOfString(piAddressVarOne,&m,&n,&lenStVarOne,&pStVarOne);

		librariesResult = searchmacroinlibraries(pStVarOne, &librariesResultSize);

		if ( (librariesResultSize == 0) || (librariesResult == NULL) )
		{
			// return []
			createMatrixOfDouble(Rhs + 1, 0, 0, NULL);
		}
		else
		{
			int m_out = librariesResultSize;
			int n_out = 1;

			createMatrixOfString(Rhs + 1, m_out, n_out, librariesResult);
		}

		LhsVar(1) = Rhs + 1;

		freeArrayOfString(librariesResult, librariesResultSize);
		if (pStVarOne){FREE(pStVarOne); pStVarOne = NULL;}

		C2F(putlhsvar)();
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d.\n"),fname, 1);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
