/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/ 
#include "gw_elementary_functions.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intfrexp)(int *id);
/*--------------------------------------------------------------------------*/
int sci_frexp(char *fname,unsigned long fname_len)
{
	static int id[6];
    SciErr sciErr;
    int *piAddressVarOne = NULL;

    CheckRhs(1, 1);
    CheckLhs(2, 2);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if ( isVarComplex(pvApiCtx, piAddressVarOne) )
    {
        Scierror(999,_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), fname, 1);
        return 0;
    }

	C2F(intfrexp)(id);
	return 0;
}
/*--------------------------------------------------------------------------*/
