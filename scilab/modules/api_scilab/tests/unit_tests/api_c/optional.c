/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*
 * Copyright (C) 2012 - 2017 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/

#include "api_scilab.h"
#include "Scierror.h"

#include <stdio.h>

int sci_optional_args(char* fname, void *pvApiCtx)
{
    char* pstName = NULL;
    int iAge = 0;
    int iDrivingLicense = 0;
    char stOutput[100];

    static rhs_opts opts[] =
    {
        { -1, "name", -1, 0, 0, NULL},
        { -1, "age", -1, 0, 0, NULL},
        { -1, "driving_license", -1, 0, 0, NULL},
        { -1, NULL, -1, 0, 0, NULL}
    };

    CheckInputArgument(pvApiCtx, 0, 3);
    CheckOutputArgument(pvApiCtx, 0, 1);

    if (getOptionals(pvApiCtx, fname, opts) == 0)
    {
        Scierror(999, "foo: error occured in getOptionals().");
        return 1;
    }

    // name
    if (opts[0].iPos != -1)
    {
        getAllocatedSingleString(pvApiCtx, opts[0].piAddr, &pstName);
    }
    else
    {
        pstName = strdup("John Doe");
    }

    // age
    if (opts[1].iPos != -1)
    {
        double dblAge = 0;
        getScalarDouble(pvApiCtx, opts[1].piAddr, &dblAge);
        iAge = (int)dblAge;
    }
    else
    {
        iAge = 77;
    }

    // driving license
    if (opts[2].iPos != -1)
    {
        getScalarBoolean(pvApiCtx, opts[2].piAddr, &iDrivingLicense);
    }
    else
    {
        iDrivingLicense = 0;
    }

    sprintf(stOutput, "%s, %d years old, %s a driving license.", pstName, iAge, iDrivingLicense ? "has" : "does not have");
    if (createSingleString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, stOutput))
    {
        return 1;
    }
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

    return 0;
}
