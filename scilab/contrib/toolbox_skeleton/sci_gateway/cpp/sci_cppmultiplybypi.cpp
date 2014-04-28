/* ==================================================================== */
/* Template toolbox_skeleton */
/* This file is released under the 3-clause BSD license. See COPYING-BSD. */
/* ==================================================================== */
#include "api_scilab.hxx"

extern "C"
{
#include "multiplybypi.h"
}

/* ==================================================================== */
api_scilab::Status sci_cppmultiplybypi(api_scilab::input &in, int _iRetCount, api_scilab::output &out)
{
    /* --> result = multiplybypi(8) */
    /* --> result = multiplybypi([12, 42; 42, 12]) */

    /* check that we have only 1 input argument */
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "cppmultiplybypi", 1);
        return api_scilab::Error;
    }

    /* check that we have only 1 output argument */
    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected."), "cppmultiplybypi", 1);
        return api_scilab::Error;
    }

    /* check input type */
    if (api_scilab::isDouble(in[0]) == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "cppmultiplybypi", 1);
        return api_scilab::Error;
    }

    /* get matrix */
    api_scilab::Double* pdblIn = api_scilab::getAsDouble(in[0]);

    //create output variable
    api_scilab::Double* pdblOut = new api_scilab::Double(pdblIn->getDims(), pdblIn->getDimsArray());

    /* The difference with the csum & csub is that we give the argument as copy
     * and not as reference */
    for (int i = 0 ; i < pdblIn->getSize() ; ++i)
    {
        /* For each element of the matrix, multiply by pi */
        pdblOut->set(i, multiplybypi(pdblIn->get(i)));
    }

    //set output paramter
    out.push_back(api_scilab::getReturnVariable(pdblOut));

    //clear api variables
    delete pdblIn;
    delete pdblOut;
    //return gateway status
    return api_scilab::OK;
}
/* ==================================================================== */

