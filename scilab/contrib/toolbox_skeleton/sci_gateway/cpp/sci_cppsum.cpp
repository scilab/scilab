/* ==================================================================== */
/* Template toolbox_skeleton */
/* This file is released under the 3-clause BSD license. See COPYING-BSD. */
/* ==================================================================== */
#include "api_scilab.hxx"

extern "C"
{
#include "csum.h"
}

/* ==================================================================== */
api_scilab::Status sci_cppsum(api_scilab::input &in, int _iRetCount, api_scilab::output &out)
{
    /* --> result = csum(3,8)

    /* check that we have only 2 input arguments */
    if (in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "cpp_sum", 2);
        return api_scilab::Error;
    }

    /* check that we have only 1 output argument */
    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected."), "cpp_sum", 1);
        return api_scilab::Error;
    }

    if (api_scilab::isDouble(in[0]) == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "cpp_sum", 1);
        return api_scilab::Error;
    }

    api_scilab::Double* pIn1 = api_scilab::getAsDouble(in[0]);
    if (in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "cpp_sum", 2);
        return api_scilab::Error;
    }

    api_scilab::Double* pIn2 = api_scilab::getAsDouble(in[1]);

    /* check size */
    if (pIn1->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "cpp_sum", 1);
        return api_scilab::Error;
    }

    if (pIn2->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "cpp_sum", 2);
        return api_scilab::Error;
    }

    api_scilab::Double* pOut = new api_scilab::Double(1, 1);
    /* call c function csub */
    csum(pIn1->get(), pIn2->get(), pOut->get());

    /* return result */
    out.push_back(api_scilab::getReturnVariable(pOut));

    //clear api variables
    delete pIn2;
    delete pIn1;
    delete pOut;
    //return gateway status
    return api_scilab::OK;
}
/* ==================================================================== */

