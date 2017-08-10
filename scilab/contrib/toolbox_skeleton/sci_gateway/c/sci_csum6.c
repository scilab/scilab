/* ==================================================================== */
/* Template toolbox_skeleton */
/* This file is released under the 3-clause BSD license. See COPYING-BSD. */
/* ==================================================================== */
#include "api_scilab.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include "csum.h"
#include <localization.h>

static const char fname[] = "csum6";
/* ==================================================================== */
int sci_csum6(scilabEnv env, int nin, scilabVar* in, int nopt, scilabOpt* opt, int nout, scilabVar* out)
{
    double in1 = 0;
    double in2 = 0;
    double out1 = 0;
    /* --> result = csum(3,8)
    /* check that we have only 2 input arguments */
    /* check that we have only 1 output argument */
    if (nin > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), fname, 2);
        return 1;
    }

    if (scilab_isDouble(env, in[0]) == 0 || scilab_isScalar(env, in[0]) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), fname, 1);
        return 1;
    }

    if (scilab_isDouble(env, in[1]) == 0 || scilab_isScalar(env, in[1]) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), fname, 2);
        return 1;
    }

    //get value of input 1
    scilab_getDouble(env, in[0], &in1);
    //get value of input 2
    scilab_getDouble(env, in[1], &in2);

    /* call c function csub */
    csum(&in1, &in2, &out1);

    //create output var and assign it to first output
    out[0] = scilab_createDouble(env, out1);
    //return without error
    return 0;
}
/* ==================================================================== */

