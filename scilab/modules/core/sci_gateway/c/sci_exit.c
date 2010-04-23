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
#include "MALLOC.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "exitCodeValue.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_exit)(char *fname,unsigned long fname_len)
{
	SciErr sciErr;

	CheckLhs(1,1);
	CheckRhs(0,1);

	if (Rhs == 0)
	{
		setExitCodeValue(0);
	}
	else
	{
		int iExit = 0;
		int m1 = 0, n1 = 0;
		int iType1 = 0;
		int *piAddressVarOne = NULL;
		double *pdVarOne = NULL;

		/* get Address of inputs */
		sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		/* check input type */
		if ( iType1 != sci_matrix )
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A scalar expected.\n"),fname,1);
			return 0;
		}

		sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarOne,&m1,&n1,&pdVarOne);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		if( n1 != 1 || m1 != 1)
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname,1);
			return 0;
		}

		iExit = (int) *pdVarOne;

		if (*pdVarOne != (double)iExit)
		{
			Scierror(999,_("%s: Wrong value for input argument #%d: A integer expected.\n"),fname,1);
			return 0;
		}

		setExitCodeValue(iExit);
	}

	// this value do quit in scirun
	C2F(com).fun = -999;

	LhsVar(1) = 0; 
	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
