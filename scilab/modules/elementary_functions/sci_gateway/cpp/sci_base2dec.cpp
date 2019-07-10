/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2019 - Stéphane MOTTELET
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
/*--------------------------------------------------------------------------*/

#include <string>
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "overload.hxx"
#include "elem_func_gw.hxx"

extern "C"
{
#include "Scierror.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_base2dec(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "base2dec", 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(77, _("%s: Wrong number of output argument(s): %d expected.\n"), "base2dec", 1);
        return types::Function::Error;
    }

    if (in[1]->isDouble() == false || in[1]->getAs<types::Double>()->isComplex() == true)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar integer value expected.\n"), "base2dec", 2);
        return types::Function::Error;
    }

    if (in[1]->getAs<types::Double>()->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar integer value expected.\n"), "base2dec", 2);
        return types::Function::Error;
    }

    double dBaseUsed = in[1]->getAs<types::Double>()->get(0);
    if (dBaseUsed != trunc(dBaseUsed))
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), "base2dec", 2);
        return types::Function::Error;
    }

    if (dBaseUsed < 2 || dBaseUsed > 36)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Must be between %d and %d."), "base2dec", 2, 2, 36);
        return types::Function::Error;
    }

    if (in[0]->isString())
    {
        types::String* pS = in[0]->getAs<types::String>();
        types::Double* pOut = new types::Double(pS->getDims(), pS->getDimsArray());

        int iBaseUsed = (unsigned int)dBaseUsed;
        double dblValue;

        for (int i = 0; i < pS->getSize(); i++)
        {
            size_t iPos;
            try
            {
                dblValue = (double)std::stoll(pS->get(i), &iPos, iBaseUsed);
                // manually throw exceptions when not thrown by std::stoll but error should be triggered
                if (dblValue > pow(2, 53))
                {
                    throw std::out_of_range("");
                }

                if (iPos < wcslen(pS->get(i)))
                {
                    throw std::invalid_argument("");
                }
            }
            catch (std::out_of_range)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: converted value is out of range.\n"), "base2dec", 1);
                pOut->killMe();
                return types::Function::Error;
            }
            catch (std::invalid_argument)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: Valid base %d representations expected.\n"), "base2dec", 1, iBaseUsed);
                pOut->killMe();
                return types::Function::Error;
            }

            pOut->set(i, dblValue);
        }

        out.push_back(pOut);
        return types::Function::OK;
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: a string expected.\n"), "base2dec", 2);
        return types::Function::Error;
    }

}
/*--------------------------------------------------------------------------*/
