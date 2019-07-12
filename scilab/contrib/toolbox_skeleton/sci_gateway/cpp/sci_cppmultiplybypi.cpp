/* ==================================================================== */
/* Template toolbox_skeleton */
/* This file is released under the 3-clause BSD license. See COPYING-BSD. */
/* ==================================================================== */
#include "double.hxx"
#include "function.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "multiplybypi.h"
}

/* ==================================================================== */
types::Function::ReturnValue sci_cppmultiplybypi(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    /* --> result = multiplybypi(8) */
    /* --> result = multiplybypi([12, 42; 42, 12]) */

    /* check that we have only 1 input argument */
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "cppmultiplybypi", 1);
        return types::Function::Error;
    }

    /* check that we have only 1 output argument */
    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected."), "cppmultiplybypi", 1);
        return types::Function::Error;
    }

    /* check input type */
    if (in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "cppmultiplybypi", 1);
        return types::Function::Error;
    }

    /* get matrix */
    types::Double* pdblIn = in[0]->getAs<types::Double>();

    //create output variable
    types::Double* pdblOut = new types::Double(pdblIn->getDims(), pdblIn->getDimsArray());

    /* The difference with the csum & csub is that we give the argument as copy
     * and not as reference */
    for (int i = 0 ; i < pdblIn->getSize() ; ++i)
    {
        /* For each element of the matrix, multiply by pi */
        pdblOut->set(i, multiplybypi(pdblIn->get(i)));
    }

    //set output paramter
    out.push_back(pdblOut);

    //return gateway status
    return types::Function::OK;
}
/* ==================================================================== */

