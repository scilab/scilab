/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "core_gw.hxx"
#include "types.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "charEncoding.h"
}

#define DEFAULT_ERROR_CODE 10000

using namespace types;

Function::ReturnValue sci_error(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (_iRetCount != 1)
    {
        ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d expected.\n"), L"error", 1);
        return Function::Error;
    }

    if (in.size() != 1 && in.size() != 2)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"error", 1, 2);
        return Function::Error;
    }

    if (in.size() == 1)
    {
        // RHS == 1
        if (in[0]->isString() == false && in[0]->isDouble() == false)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d.\n"), L"error", 1);
            return Function::Error;
        }

        if (in[0]->isString() == true)
        {
            if (in[0]->getAs<types::String>()->getSize() == 1)
            {
                ScierrorW(DEFAULT_ERROR_CODE, L"%ls", in[0]->getAs<types::String>()->get(0));
                return Function::Error;
            }
            else
            {
                ScierrorW(999, _W("%ls: Wrong size for input argument #%d: A string expected.\n"), L"error", 1);
                return Function::Error;
            }
        }
        else
        {
            if (in[0]->getAs<Double>()->getSize() != 1)
            {
                ScierrorW(999, _W("%ls: Wrong size for input argument #%d: A scalar expected.\n"), L"error", 1);
                return Function::Error;
            }

            if (in[0]->getAs<Double>()->getReal(0, 0) <= 0 || in[0]->getAs<Double>()->isComplex())
            {
                ScierrorW(999, _W("%ls: Wrong value for input argument #%d: Value greater than 0 expected.\n"), L"error", 1);
                return Function::Error;
            }


            // FIXME : Find a way to retrieve error message from given ID.
            ScierrorW((int)in[0]->getAs<Double>()->getReal(0, 0),
                      L"[Error %d]: message given by ID... Should avoid this !!",
                      (int) in[0]->getAs<Double>()->getReal(0, 0));
            return Function::Error;
        }

    }
    else
    {
        // RHS = 2 according to previous check.
        if (in[0]->isDouble() == false)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d.\n"), L"error", 1);
            return Function::Error;
        }

        if (in[1]->isString() == false)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d.\n"), L"error", 2);
            return Function::Error;
        }

        if (in[0]->getAs<Double>()->getSize() != 1)
        {
            ScierrorW(999, _W("%ls: Wrong size for input argument #%d: A scalar expected.\n"), L"error", 1);
            return Function::Error;
        }

        if (in[1]->getAs<types::String>()->getSize() != 1)
        {
            ScierrorW(999, _W("%ls: Wrong size for input argument #%d: A scalar expected.\n"), L"error", 2);
            return Function::Error;
        }

        if (in[0]->getAs<Double>()->getReal(0, 0) <= 0 || in[0]->getAs<Double>()->isComplex())
        {
            ScierrorW(999, _W("%ls: Wrong value for input argument #%d: Value greater than 0 expected.\n"), L"error", 1);
            return Function::Error;
        }

        ScierrorW((int)in[0]->getAs<Double>()->getReal(0, 0),
                  in[1]->getAs<types::String>()->get(0));
        return Function::Error;

    }


    return Function::Error;
}
