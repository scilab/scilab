/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "api_scilab.hxx"
#include "newtype.hxx"

api_scilab::Status sci_newtype(api_scilab::input &in, int _iRetCount, api_scilab::output &out)
{
    if (in.size() > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "newtype", 1);
        return api_scilab::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected."), "newtype", 1);
        return api_scilab::Error;
    }

    if (in.size() == 1)
    {
        if (api_scilab::isBool(in[0]) == false || api_scilab::getAsBool(in[0])->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar boolean expected.\n"), "cppfoo", 1);
            return api_scilab::Error;
        }

        api_scilab::Bool* pIn = api_scilab::getAsBool(in[0]);
        out.push_back(new NewType(pIn->get(0)));
    }
    else
    {
        out.push_back(new NewType());
    }

    return api_scilab::OK;
}
