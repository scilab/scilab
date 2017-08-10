/* ==================================================================== */
/* Template toolbox_skeleton */
/* This file is released under the 3-clause BSD license. See COPYING-BSD. */
/* ==================================================================== */
#include "function.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

/* ==================================================================== */
types::Function::ReturnValue sci_cpperror(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _d("toolbox_skeleton", "%s: I'm waiting for only one argument.\n"), "cerror");
        return types::Function::Error;
    }
    else
    {
        Scierror(999, _d("toolbox_skeleton", "%s: Yeah! %d is a good number of arguments but I prefer fail, sorry.\n"), "cerror", 1);
        return types::Function::Error;
    }
}
/* ==================================================================== */
