/* ==================================================================== */
/* Template toolbox_skeleton */
/* This file is released under the 3-clause BSD license. See COPYING-BSD. */
/* ==================================================================== */
#include "api_scilab.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include <localization.h>

static const char fname[] = "cerror6";
/* ==================================================================== */
int sci_cerror6(scilabEnv env, int nin, scilabVar* in, int nopt, scilabOpt* opt, int nout, scilabVar* out)
{
    if (nin != 1)
    {
        Scierror(999, _d("toolbox_skeleton", "%s: I'm waiting for only one argument.\n"), fname);
        return STATUS_ERROR;
    }
    else
    {
        Scierror(999, _d("toolbox_skeleton", "%s: Yeah! %d is a good number of arguments but I prefer fail, sorry.\n"), fname, 1);
        return STATUS_ERROR;
    }
}
/* ==================================================================== */
