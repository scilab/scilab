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
extern "C"
{
#include "gw_xcos.h"
#include "stack-c.h"
#include "callxcos.h"
#include "api_common.h"
#include "api_string.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "getFullFilename.h"
}
/*--------------------------------------------------------------------------*/
int sci_Xcos(char *fname,unsigned long fname_len)
{
	CheckRhs(0,1);
	CheckLhs(0,1);

	if (Rhs == 0)
	{
		callXcos(NULL, 0);
	}
	else
	{
		int m1 = 0, n1 = 0;
		int *piAddressVarOne = NULL;
		int i = 0;
		int lw = 1;
		int iType = 0;
		SciErr sciErr;

		sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		if ( iType == sci_strings )
		{
			char **pStVarOne = NULL;
			int *lenStVarOne = NULL;
			char **pStFullFilenames = NULL;

			/* get dimensions */
			sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}

			lenStVarOne = (int*)MALLOC(sizeof(int)*(m1 * n1));
			if (lenStVarOne == NULL)
			{
				Scierror(999,_("%s: No more memory.\n"), fname);
				return 0;
			}

			/* get lengths */
			sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
			if(sciErr.iErr)
			{
				if (lenStVarOne) { FREE(lenStVarOne); lenStVarOne = NULL;}
				printError(&sciErr, 0);
				return 0;
			}

			pStVarOne = (char **)MALLOC(sizeof(char*)*(m1*n1));
			if (pStVarOne == NULL)
			{
				if (lenStVarOne) { FREE(lenStVarOne); lenStVarOne = NULL;}
				Scierror(999,_("%s: No more memory.\n"), fname);
				return 0;
			}

			pStFullFilenames = (char **)MALLOC(sizeof(char*)*(m1*n1));
			if (pStFullFilenames == NULL)
			{
				if (lenStVarOne) { FREE(lenStVarOne); lenStVarOne = NULL;}
				freeArrayOfString(pStVarOne, m1 * n1);
				Scierror(999,_("%s: No more memory.\n"), fname);
				return 0;
			}

			for(i = 0; i < m1 * n1; i++)
			{
				pStVarOne[i] = (char*)MALLOC(sizeof(char*) * (lenStVarOne[i] + 1));
			}

			/* get strings */
			sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
			if(sciErr.iErr)
			{
				freeArrayOfString(pStFullFilenames, m1 * n1);
				freeArrayOfString(pStVarOne, m1 * n1);
				if (lenStVarOne) { FREE(lenStVarOne); lenStVarOne = NULL;}
				printError(&sciErr, 0);
				return 0;
			}

			/* Expand paths */
			for(i = 0; i < m1 * n1; i++)
			{
				pStFullFilenames[i] = getFullFilename(pStVarOne[i]);
			}

			if (lenStVarOne) { FREE(lenStVarOne); lenStVarOne = NULL;}
			freeArrayOfString(pStVarOne, m1 * n1);

			callXcos(pStFullFilenames, m1 * n1);

			freeArrayOfString(pStFullFilenames, m1 * n1);
		}
		else if (iType == sci_mlist)
		{
			C2F(overload)(&lw, fname, fname_len);
			return 0;
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
			return 0;
		}
	}

	LhsVar(1) = 0;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
