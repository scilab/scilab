/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <stdlib.h>

#include "localization.h"
#include "Scierror.h"
#include "HandleManagement.h"
#include "api_scilab.h"
#include "gw_gui.h"
/*--------------------------------------------------------------------------*/
int sci_getcallbackobject(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    int* piAddrpObjUID = NULL;
    int nbRow = 0;
    int nbCol = 0;
    char** pObjUID = NULL;
    unsigned long graphicHandle = 0;

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 0, 1);

    if ((checkInputArgumentType(pvApiCtx, 1, sci_strings)))
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrpObjUID);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of string at position 1.
        if (getAllocatedMatrixOfString(pvApiCtx, piAddrpObjUID, &nbRow, &nbCol, &pObjUID))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: String matrix expected.\n"), fname, 1);
            return 1;
        }

        if (nbCol != 1 || nbRow == 0)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
            return FALSE;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
        return FALSE;
    }

    graphicHandle = getHandle(pObjUID[0]);
    freeAllocatedMatrixOfString(nbRow, nbCol, pObjUID);

    /* Create return variable */
    if (graphicHandle == 0)     /* Non-existing object --> return [] */
    {
        double* stkAdr = NULL;
        nbRow = 0;
        nbCol = 0;

        sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, nbRow, nbCol, &stkAdr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }
    }
    else                        /* Return the handle */
    {
        long long* stkAdr = NULL;
        nbRow = 1;
        nbCol = 1;

        sciErr = allocMatrixOfHandle(pvApiCtx, nbInputArgument(pvApiCtx) + 1, nbRow, nbCol, &stkAdr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        *stkAdr = (long long)graphicHandle;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return TRUE;
}

/*--------------------------------------------------------------------------*/
