/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include "gw_core.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "fromjava.h"
#include "localization.h"
#include "Scierror.h"
#include "gw_call_scilab.h"

/*--------------------------------------------------------------------------*/
int sci_fromjava(char *fname, void* pvApiCtx)
{
    CheckInputArgument(pvApiCtx, 0, 0);
    CheckOutputArgument(pvApiCtx, 1, 1);

    if ( IsFromJava() )
    {
        if (createScalarBoolean(pvApiCtx, 1, TRUE))
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }
    }
    else
    {
        if (createScalarBoolean(pvApiCtx, 1, FALSE))
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }
    }

    AssignOutputVariable(pvApiCtx, 1) = 1;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
