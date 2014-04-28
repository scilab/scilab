/* ==================================================================== */
/* Template toolbox_skeleton */
/* Example detail in "API_scilab getting started" help page */
/* This file is released under the 3-clause BSD license. See COPYING-BSD. */
/* ==================================================================== */
#include "api_scilab.hxx"

/* ==================================================================== */
api_scilab::Status sci_cppfoo(api_scilab::input &in, int _iRetCount, api_scilab::output &out)
{
    ////////// Check the number of input and output arguments //////////
    /* --> [c, d] = foo(a, b) */
    /* check that we have only 2 input arguments */
    if (in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "cppfoo", 2);
        return api_scilab::Error;
    }

    /* check that we have only 2 output argument */
    if (_iRetCount != 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected."), "cppfoo", 2);
        return api_scilab::Error;
    }

    ////////// Manage the first input argument (double) //////////

    /* Check that the first input argument is a real matrix (and not complex) */
    if (api_scilab::isDouble(in[0]) == false || api_scilab::getAsDouble(in[0])->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "cppfoo", 1);
        return api_scilab::Error;
    }

    api_scilab::Double* pdbl = api_scilab::getAsDouble(in[0]);

    ////////// Manage the second input argument (boolean) //////////
    if (api_scilab::isBool(in[1]) == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A boolean matrix expected.\n"), "cppfoo", 2);
        return api_scilab::Error;
    }

    api_scilab::Bool* pb = api_scilab::getAsBool(in[1]);


    ////////// Application code //////////
    // Could be replaced by a call to a library

    api_scilab::Double* pOut1 = new api_scilab::Double(pdbl->getDims(), pdbl->getDimsArray());
    for (int i = 0 ; i < pdbl->getSize() ; ++i)
    {
        /* For each element of the matrix, multiply by 2 */
        pOut1->set(i, pdbl->get(i) * 2);
    }

    api_scilab::Bool* pOut2 = new api_scilab::Bool(pb->getDims(), pb->getDimsArray());
    for (int i = 0 ; i < pb->getSize() ; ++i)
    {
        /* For each element of the matrix, invert the value */
        pOut2->set(i, pb->get(i) == FALSE ? TRUE : FALSE);
    }

    ////////// return output parameters //////////
    out.push_back(api_scilab::getReturnVariable(pOut1));
    out.push_back(api_scilab::getReturnVariable(pOut2));

    //clear api variables
    delete pdbl;
    delete pb;
    delete pOut1;
    delete pOut2;
    //return gateway status
    return api_scilab::OK;
}
/* ==================================================================== */

