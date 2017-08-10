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

int sci_bug_15121(char* fname, void *pvApiCtx)
{
    static rhs_opts opts[] =
    {
        { -1, "B", -1, 0, 0, NULL},
        { -1, "A", -1, 0, 0, NULL},
        { -1, NULL, -1, 0, 0, NULL}
    };

    if (getOptionals(pvApiCtx, fname, opts) == 0)
    {
        Scierror(999, "foo: error occured in getOptionals().");
        return 1;
    }

    return 0;
}