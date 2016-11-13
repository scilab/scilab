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
#include <ctype.h>
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "sci_malloc.h"
#include "os_string.h"

const char fname[] = "string_test";

int sci_string_test(scilabEnv env, int nin, scilabVar* in, int nopt, scilabOpt opt, int nout, scilabVar* out)
{
    int i = 0;
    int inr1 = 0;
    int inc1 = 0;
    int size1 = 0;
    wchar_t** in1 = NULL;

    wchar_t* in2 = 0;

    int dim1 = 3;
    int dims1[] = {0, 0, 2};
    wchar_t** out1 = NULL;

    wchar_t* out2;
    int len2 = 0;

    if (nin != 2)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 2);
        return STATUS_ERROR;
    }

    if (nout != 2)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), fname, 3);
        return STATUS_ERROR;
    }

    //in[0] : matrix 2d of string
    if (scilab_isString(env, in[0]) == 0 || scilab_isMatrix2d(env, in[0]) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string matrix expected.\n"), fname, 1);
        return STATUS_ERROR;
    }

    size1 = scilab_getDim2d(env, in[0], &inr1, &inc1);
    scilab_getStringArray(env, in[0], &in1);

    //in[1] : string
    if (scilab_isString(env, in[1]) == 0 || scilab_isScalar(env, in[1]) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A double expected.\n"), fname, 2);
        return STATUS_ERROR;
    }

    scilab_getString(env, in[1], &in2);

    //out1 : matrix 2d of string with same size of in[0]
    dims1[0] = inr1;
    dims1[1] = inc1;
    out[0] = scilab_createStringMatrix(env, dim1, dims1);
    scilab_getStringArray(env, out[0], &out1);

    for (i = 0; i < size1; ++i)
    {
        wchar_t temp[128];
        wcscpy(temp, in1[i]);
        wcscat(temp, L".one");
        out1[i] = os_wcsdup(temp);

        wcscpy(temp, in1[i]);
        wcscat(temp, L".two");
        out1[i + size1] = os_wcsdup(temp);
    }

    //out2 : string
    out2 = os_wcsdup(in2);
    len2 = wcslen(out2);
    for (i = 0; i < len2; ++i)
    {
        if (out2[i] >= L'a' && out2[i] <= L'z')
        {
            out2[i] = ((out2[i] - 'a' + 26 - 1) % 26) + 'a';
        }
        else if (out2[i] >= L'A' && out2[i] <= L'Z')
        {
            out2[i] = ((out2[i] - 'A' + 26 - 1) % 26) + 'A';
        }
        else
        {
            //no change
        }
    }

    out[1] = scilab_createString(env, out2);
    FREE(out2);
    return STATUS_OK;
}
