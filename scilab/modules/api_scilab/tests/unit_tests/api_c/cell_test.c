/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution. The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
#include <ctype.h>
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "sci_malloc.h"
#include "os_string.h"

const char fname[] = "cell_test";

int sci_cell_test(scilabEnv env, int nin, scilabVar* in, int nopt, scilabOpt opt, int nout, scilabVar* out)
{
    int i = 0;
    //input
    int size1 = 0;
    scilabVar in1 = NULL;
    double* dims = NULL;
    scilabVar in2 = NULL;
    int size2 = 0;
    //output
    int outSize = 1;
    scilabVar out1 = NULL;
    scilabVar* vars = NULL;

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
    if (scilab_isDouble(env, in1) == 0 || scilab_getSize(env, in1) != 2)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: double vector of size 2 expected.\n"), fname, 1);
        return STATUS_ERROR;
    }

    size1 = scilab_getSize(env, in1);
    scilab_getDoubleArray(env, in1, &dims);

    outSize = (int)dims[0] * (int)dims[1];

    //in2
    in2 = in[1];
    if (scilab_isList(env, in2) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A list expected.\n"), fname, 2);
        return STATUS_ERROR;
    }

    size2 = scilab_getSize(env, in2);


    if (outSize != size2)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d.\n"), fname, 2);
        return STATUS_ERROR;
    }

    out1 = scilab_createCellMatrix2d(env, (int)dims[0], (int)dims[1]);
    for (i = 0; i < outSize; ++i)
    {
        scilab_setCell2dValue(env, out1, i, 0, scilab_getListItem(env, in2, i));
    }

    out[0] = out1;
    return STATUS_OK;
}
