/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010 - DIGITEO - Allan CORNET
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 * Copyright (C) 2017 Siddhartha Gairola
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/
/*--------------------------------------------------------------------------*/
#include "string_gw.hxx"
#include "function.hxx"
#include "context.hxx"
#include "types.hxx"
#include "bool.hxx"
#include "double.hxx"
#include "stripblanks.hxx"
/*--------------------------------------------------------------------------*/
extern "C"
{
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_stripblanks(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
#define FUNCNAME "stripblanks"
    bool bRemoveTab = false;
    double  flag = 0;

    // check input parameters
    if (in.size() < 1 || in.size() > 3)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), FUNCNAME, 1, 3);
        return types::Function::Error;
    }

    // check output parameters
    if (_iRetCount != 1 && _iRetCount != -1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), FUNCNAME, 1);
        return types::Function::Error;
    }

    if (in.size() > 1)
    {
        if (in[1]->isBool() == false || in[1]->getAs<types::Bool>()->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), FUNCNAME, 2);
            return types::Function::Error;
        }

        if (in[1]->getAs<types::Bool>()->get()[0] == 1)
        {
            bRemoveTab = true;
        }
    }

    if (in.size() == 3)
    {
        if (in[2]->isDouble() == false || in[2]->getAs<types::Double>()->isScalar() == false)
        {

            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), FUNCNAME, 2);
            return types::Function::Error;

        }

        flag = in[2]->getAs<types::Double>()->get()[0];

        if (floor(flag) != flag || (flag != 0.0 && flag != 1.0 && flag != -1.0))
        {

            Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), FUNCNAME, 3, "-1, 0, 1");
            return types::Function::Error;
        }

    }

    switch (in[0]->getType())
    {
        case types::InternalType::ScilabString:
        {
            types::String *pS = stripblanks(in[0]->getAs<types::String>(), bRemoveTab, static_cast<int>(flag));
            if (pS == NULL)
            {
                Scierror(999, _("%s : No more memory.\n"), FUNCNAME);
                return types::Function::Error;
            }

            out.push_back(pS);
            break;
        }
        case types::InternalType::ScilabDouble://manage []
        {
            if (in[0]->getAs<types::Double>()->getSize() != 0)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings or empty matrix expected.\n"), FUNCNAME, 1);
                return types::Function::Error;
            }

            out.push_back(types::Double::Empty());
            break;
        }
        default:
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings or empty matrix expected.\n"), FUNCNAME, 1);
            return types::Function::Error;
        }
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
