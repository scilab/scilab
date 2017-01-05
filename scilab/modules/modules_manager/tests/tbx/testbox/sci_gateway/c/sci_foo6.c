/* ==================================================================== */
/* Template toolbox_skeleton */
/* Example detail in "API_scilab getting started" help page */
/* This file is released under the 3-clause BSD license. See COPYING-BSD. */
/* ==================================================================== */
#include "api_scilab.h"
#include "Scierror.h"
#include "BOOL.h"
#include <localization.h>

static const char fname[] = "foo6";
/* ==================================================================== */
int sci_foo6(scilabEnv env, int nin, scilabVar* in, int nopt, scilabOpt* opt, int nout, scilabVar* out)
{
    int i = 0;
    int row1 = 0;
    int col1 = 0;
    int size1 = 0;
    double* in1 = NULL;
    double* out1 = NULL;

    int row2 = 0;
    int col2 = 0;
    int size2 = 0;
    int* in2 = NULL;
    int* out2 = NULL;

    /* --> [a,b] = foo(d[i,j], b[i,j])
    /* check that we have only 2 input arguments */
    /* check that we have only 1 output argument */
    if (nin > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), fname, 2);
        return 1;
    }

    if (scilab_isDouble(env, in[0]) == 0 || scilab_isMatrix2d(env, in[0]) == 0 || scilab_isComplex(env, in[0]) == 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), fname, 1);
        return 1;
    }

    if (scilab_isBoolean(env, in[1]) == 0 || scilab_isMatrix2d(env, in[1]) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A boolean matrix expected.\n"), fname, 2);
        return 1;
    }

    size1 = scilab_getDim2d(env, in[0], &row1, &col1);
    size2 = scilab_getDim2d(env, in[1], &row2, &col2);

    if (row1 != row2 || col1 != col2)
    {
        Scierror(999, _("%s: Wrong size for input arguments: Same size expected.\n"), fname);
        return 1;
    }

    scilab_getDoubleArray(env, in[0], &in1);
    scilab_getBooleanArray(env, in[1], &in2);

    out[0] = scilab_createDoubleMatrix2d(env, row1, col1, 0);
    out[1] = scilab_createBooleanMatrix2d(env, row2, col2);

    scilab_getDoubleArray(env, out[0], &out1);
    scilab_getBooleanArray(env, out[1], &out2);

    for (i = 0; i < size1; ++i)
    {
        //For each element of the matrix, multiply by 2
        out1[i] = in1[i] * 2;
    }

    for (i = 0; i < size2; ++i)
    {
        //For each element of the matrix, invert the value
        out2[i] = in2[i] == TRUE ? FALSE : TRUE;
    }

    return 0;
}
/* ==================================================================== */

