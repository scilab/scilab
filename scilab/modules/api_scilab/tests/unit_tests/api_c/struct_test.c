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

const char fname[] = "struct_test";

int sci_struct_test(scilabEnv env, int nin, scilabVar* in, int nopt, scilabOpt opt, int nout, scilabVar* out)
{
    int i = 0;
    //input
    scilabVar in1 = NULL;
    int size1 = 0;
    wchar_t** fields = NULL;
    scilabVar in2 = NULL;
    int size2 = 0;
    //output
    scilabVar out1 = NULL;

    //goal is to take string vector and list from intput to
    //create a struct with fields names from string and
    //fields data from list.

    if (nin != 2)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 2);
        return STATUS_ERROR;
    }

    if (nout != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), fname, 1);
        return STATUS_ERROR;
    }

    //in1
    in1 = in[0];
    if (scilab_isString(env, in1) == 0 || scilab_isVector(env, in1) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string vector expected.\n"), fname, 1);
        return STATUS_ERROR;
    }

    size1 = scilab_getSize(env, in1);
    scilab_getStringArray(env, in1, &fields);

    //in2
    in2 = in[1];
    if (scilab_isList(env, in2) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A list expected.\n"), fname, 2);
        return STATUS_ERROR;
    }

    size2 = scilab_getSize(env, in2);

    if (size1 != size2)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: arg1 and arg2 must have same size.\n"), fname);
        return STATUS_ERROR;
    }

    out1 = scilab_createStruct(env);

    for (i = 0; i < size1; ++i)
    {
        scilab_addField(env, out1, fields[i]);
        scilab_setStructMatrix2dData(env, out1, fields[i], 0, 0, scilab_getListItem(env, in2, i));
    }

    out[0] = out1;
    return STATUS_OK;
}
