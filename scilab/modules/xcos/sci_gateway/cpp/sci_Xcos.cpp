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
		char **pStVarOne = NULL;
		int *lenStVarOne = NULL;
		int i = 0;
		int lw = 1;
                int iType = 0;
                StrErr strErr;

		getVarAddressFromPosition(1, &piAddressVarOne);

                strErr = getVarType(piAddressVarOne, &iType);
                if(strErr.iErr)
                  {
                    printError(&strErr, 0);
                    return false;
                  }
                
		if ( iType == sci_strings )
		{
			/* get dimensions */
			getMatrixOfString(piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);

			lenStVarOne = (int*)MALLOC(sizeof(int)*(m1 * n1));
			if (lenStVarOne == NULL)
			{
				Scierror(999,_("%s: No more memory.\n"), fname);
				return 0;
			}

			/* get lengths */
			getMatrixOfString(piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);

			pStVarOne = (char **)MALLOC(sizeof(char*)*(m1*n1));
			if (pStVarOne == NULL)
			{
				Scierror(999,_("%s: No more memory.\n"), fname);
				return 0;
			}

			for(i = 0; i < m1 * n1; i++)
			{
				pStVarOne[i] = (char*)MALLOC(sizeof(char*) * (lenStVarOne[i] + 1));
			}

			/* get strings */
			getMatrixOfString(piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);

			callXcos(pStVarOne,m1 * n1);
			freeArrayOfString(pStVarOne,m1 * n1);
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
