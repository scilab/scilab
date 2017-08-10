/* ==================================================================== */
/* Template toolbox_skeleton */
/* Example detail in "API_scilab getting started" help page */
/* This file is released under the 3-clause BSD license. See COPYING-BSD. */
/* ==================================================================== */
#include "double.hxx"
#include "bool.hxx"
#include "function.hxx"

extern "C" 
{
#include "Scierror.h"
#include "localization.h"
}
/* ==================================================================== */
types::Function::ReturnValue sci_cppfoo(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    ////////// Check the number of input and output arguments //////////
    /* --> [c, d] = foo(a, b) */
    /* check that we have only 2 input arguments */
    if (in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "cppfoo", 2);
        return types::Function::Error;
    }

    /* check that we have only 2 output argument */
    if (_iRetCount != 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected."), "cppfoo", 2);
        return types::Function::Error;
    }

    ////////// Manage the first input argument (double) //////////

    /* Check that the first input argument is a real matrix (and not complex) */
    if (in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "cppfoo", 1);
        return types::Function::Error;
    }

    types::Double* pdbl = in[0]->getAs<types::Double>();

    ////////// Manage the second input argument (boolean) //////////
    if (in[1]->isBool() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A boolean matrix expected.\n"), "cppfoo", 2);
        return types::Function::Error;
    }

    types::Bool* pb = in[1]->getAs<types::Bool>();


    ////////// Application code //////////
    // Could be replaced by a call to a library

    types::Double* pOut1 = new types::Double(pdbl->getDims(), pdbl->getDimsArray());
    for (int i = 0 ; i < pdbl->getSize() ; ++i)
    {
        /* For each element of the matrix, multiply by 2 */
        pOut1->set(i, pdbl->get(i) * 2);
    }

    types::Bool* pOut2 = new types::Bool(pb->getDims(), pb->getDimsArray());
    for (int i = 0 ; i < pb->getSize() ; ++i)
    {
        /* For each element of the matrix, invert the value */
        pOut2->set(i, pb->get(i) == FALSE ? TRUE : FALSE);
    }

    ////////// return output parameters //////////
    out.push_back(pOut1);
    out.push_back(pOut2);

    //return gateway status
    return types::Function::OK;
}
/* ==================================================================== */

