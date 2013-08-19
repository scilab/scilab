/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Pedro SOUZA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "gw_graphics.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "HandleManagement.h"
#include "Light.h"


int sci_light_delete(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int* piAddr	= NULL;
    int nbRow, nbCol;
    char * light;
    long long* pLightHandle = NULL;
    BOOL result;


    CheckInputArgument(pvApiCtx, 1, 1);

    if (checkInputArgumentType(pvApiCtx, 1, sci_handles))
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
        if (sciErr.iErr)
        {
            return 0;
        }
        getMatrixOfHandle(pvApiCtx, piAddr, &nbRow, &nbCol, &pLightHandle);
        if (pLightHandle == NULL) return FALSE;

        if (nbRow * nbCol != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
            return FALSE;
        }

        result = deleteLight(*pLightHandle);
        if (result == FALSE)
        {
            return FALSE;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for argument %d: A graphic handle expected.\n"), fname, 1);
        return FALSE;
    }

    return TRUE;
}
