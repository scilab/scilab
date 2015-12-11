/* ==================================================================== */
/* Template toolbox_skeleton */
/* This file is released under the 3-clause BSD license. See COPYING-BSD. */
/* ==================================================================== */
#include "api_scilab.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include "multiplybypi.h"
#include <localization.h>

static const char fname[] = "multiplybypi6";
/* ==================================================================== */
int sci_multiplybypi6(scilabEnv env, int nin, scilabVar* in, int nopt, scilabOpt* opt, int nout, scilabVar* out)
{
    int i = 0;
    double* in1 = 0;
    double* out1 = 0;
    int row = 0;
    int col = 0;
    int size = 0;
    /* --> result = csum(3,8)
    /* check that we have only 2 input arguments */
    /* check that we have only 1 output argument */
    if (nin > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), fname, 2);
        return 1;
    }

    if (scilab_isDouble(env, in[0]) == 0 || scilab_isMatrix2d(env, in[0]) == 0)
    {
        Scierror(999, "%s: Wrong type for input argument #%d: A double matrix expected.\n", fname, 1);
        return 1;
    }

    size = scilab_getDim2d(env, in[0], &row, &col);
    out[0] = scilab_createDoubleMatrix2d(env, row, col, 0);

    scilab_getDoubleArray(env, in[0], &in1);
    scilab_getDoubleArray(env, out[0], &out1);
    /* The difference with the csum & csub is that we give the argument as copy
     * and not as reference */
    for (i = 0; i < size; ++i)
    {
        /* For each element of the matrix, multiply by pi */
        out1[i] = multiplybypi(in1[i]);
    }

    return 0;
}
/* ==================================================================== */

