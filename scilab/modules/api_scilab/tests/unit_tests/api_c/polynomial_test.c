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
#include "sciprint.h"
#include "sci_malloc.h"

const char fname[] = "polynomial_test";

int sci_polynomial_test(scilabEnv env, int nin, scilabVar* in, int nopt, scilabOpt opt, int nout, scilabVar* out)
{
    int i = 0;
    int inr1 = 0;
    int inc1 = 0;
    int size1 = 0;
    double* in1 = NULL;
    int rank1 = 0;
    wchar_t* name1 = NULL;

    double* in2 = 0;
    int rank2 = 0;
    wchar_t* name2 = NULL;

    double* out1 = NULL;

    double* out2 = 0;

    if (nin != 2)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 2);
        return STATUS_ERROR;
    }

    if (nout != 2)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), fname, 2);
        return STATUS_ERROR;
    }

    //in[0] : matrix 2d of polynomial
    if (scilab_isPoly(env, in[0]) == 0 || scilab_isMatrix2d(env, in[0]) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A polynomial matrix expected.\n"), fname, 1);
        return STATUS_ERROR;
    }

    //scilab_getPolyVarname(env, in[0], &name1);
    size1 = scilab_getDim2d(env, in[0], &inr1, &inc1);

    //in[1] : polynomial
    if (scilab_isPoly(env, in[1]) == 0 || scilab_isScalar(env, in[1]) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A polynomial expected.\n"), fname, 2);
        return STATUS_ERROR;
    }

    //scilab_getPolyVarname(env, in[1], &name2);
    rank2 = scilab_getPolyArray(env, in[1], 0, &in2);

    //out1 : matrix 2d of polynomial with same size of in[0]
    out[0] = scilab_createPolyMatrix2d(env, L"s", inr1, inc1, 0);

    for (i = 0; i < size1; ++i)
    {
        int j = 0;
        rank1 = scilab_getPolyArray(env, in[0], i, &in1);
        out1 = (double*)MALLOC((rank1 + 2) * sizeof(double));//+1 to grow and +1 because rank is (size - 1)
        out1[0] = 0;

        for (j = 0; j < rank1 + 1; ++j)
        {
            out1[j + 1] = in1[j];
        }

        scilab_setPolyArray(env, out[0], i, rank1 + 1, out1);
        FREE(out1);
    }

    //out2 : polynomial -in2
    out[1] = scilab_createPoly(env, L"s", 0);
    out2 = (double*)MALLOC((rank2 + 1) * sizeof(double));
    for (i = 0; i < rank2 + 1; ++i)
    {
        out2[i] = -in2[i];
    }

    scilab_setPolyArray(env, out[1], 0, rank2, out2);
    FREE(out2);
    return STATUS_OK;
}
