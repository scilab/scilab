/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include <string.h>
#include "api_scilab.h"
#include "gw_core.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "scilabmode.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
int C2F(sci_getscilabmode)(char *fname, unsigned long fname_len)
{
    char *output = NULL ;
    int iRet = 0;

    CheckInputArgument(pvApiCtx, 0, 0) ;
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    switch (getScilabMode())
    {
        case SCILAB_API:
        default :
            output = strdup("API");
            break;
        case SCILAB_STD:
            output = strdup("STD");
            break;
        case SCILAB_NW:
            output = strdup("NW");
            break;
        case SCILAB_NWNI:
            output = strdup("NWNI");
            break;
    }

    /* Create the string matrix as return of the function */
    iRet = createSingleString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, output);
    FREE(output); // Data have been copied into Scilab memory
    if (iRet)
    {
        return 1;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
