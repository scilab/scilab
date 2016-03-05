/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
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
#include "localization.h"

const char fname[] = "overload_test";

int sci_overload_test(scilabEnv env, int nin, scilabVar* in, int nopt, scilabOpt opt, int nout, scilabVar* out)
{
    if (nin < 1 || nin > 2)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), fname, 1, 2);
        return STATUS_ERROR;
    }

    //call overload function
    if (nin == 1)
    {
        return scilab_overload(env, in[0], nin, in, nout, out);
    }
    else // call function in[0] with in[1] as argument
    {
        wchar_t* name = NULL;
        if (scilab_isString(env, in[0]) == 0 || scilab_isScalar(env, in[0]) == 0)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
            return STATUS_ERROR;
        }

        scilab_getString(env, in[0], &name);

        return scilab_call(env, name, nin - 1, in + 1, nout, out);
    }
    return STATUS_OK;
}
