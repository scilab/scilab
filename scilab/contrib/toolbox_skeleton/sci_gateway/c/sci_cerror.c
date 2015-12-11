/* ==================================================================== */
/* Template toolbox_skeleton */
/* This file is released under the 3-clause BSD license. See COPYING-BSD. */
/* ==================================================================== */
#include "api_scilab.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include <localization.h>

/* ==================================================================== */
int sci_cerror(char *fname, void* pvApiCtx)
{
    int iRhs = nbInputArgument(pvApiCtx);

    if (iRhs != 1)
    {
        Scierror(999, _d("toolbox_skeleton", "%s: I'm waiting for only one argument.\n"), fname);
        return 0;
    }
    else
    {
        Scierror(999, _d("toolbox_skeleton", "%s: Yeah! %d is a good number of arguments but I prefer fail, sorry.\n"), fname, 1);
        return 0;
    }
}
/* ==================================================================== */
