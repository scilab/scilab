/* ==================================================================== */
/* Template toolbox_skeleton */
/* This file is released under the 3-clause BSD license. See COPYING-BSD. */
/* ==================================================================== */
#include "api_scilab.hxx"
#include "Scierror.h"
#include "localization.h"

/* ==================================================================== */
api_scilab::Status sci_cpperror(api_scilab::input &in, int _iRetCount, api_scilab::output &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _d("toolbox_skeleton", "%s: I'm waiting for only one argument.\n"), "cerror");
        return api_scilab::Error;
    }
    else
    {
        Scierror(999, _d("toolbox_skeleton", "%s: Yeah! %d is a good number of arguments but I prefer fail, sorry.\n"), "cerror", 1);
        return api_scilab::Error;
    }
}
/* ==================================================================== */
