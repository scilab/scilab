/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO- Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "core_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "double.hxx"
#include "bool.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/

using namespace types;

types::Function::ReturnValue sci_lasterror(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    bool bClearError = true;
    if(in.size() > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected."), "lasterror", 0, 1);
        return Function::Error;
    }

    if(_iRetCount > 4)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "lasterror", 1, 4);
        return Function::Error;
    }

    if(in.size() == 1)
    {
        if(in[0]->isBool() == false || in[0]->getAs<types::Bool>()->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Boolean vector expected.\n"), "lasterror", 1);
            return Function::Error;
        }
        bClearError = in[0]->getAs<types::Bool>()->get()[0] == 1; //convert int to bool
    }

    //String* pErrorMessage = new String(
    std::wstring wstLastErrorMessage = ConfigVariable::getLastErrorMessage();
    if(wstLastErrorMessage.size() == 0)
    {
        out.push_back(Double::Empty());
    }
    else
    {
        out.push_back(new String(wstLastErrorMessage.c_str()));
    }
    
    if(_iRetCount > 1)
    {
        Double* pErrorNumber = new Double(ConfigVariable::getLastErrorNumber());
        out.push_back(pErrorNumber);

        if(_iRetCount > 2)
        {
            Double* pErrorLine = new Double(ConfigVariable::getLastErrorLine());
            out.push_back(pErrorLine);

            if(_iRetCount > 3)
            {
                std::wstring wstLastErrorFunction = ConfigVariable::getLastErrorFunction();
                if(wstLastErrorFunction.size() == 0)
                {
                    out.push_back(Double::Empty());
                }
                else
                {
                    out.push_back(new String(wstLastErrorFunction.c_str()));
                }
            }
        }
    }

    if(bClearError)
    {
        ConfigVariable::clearLastError();
    }

    ConfigVariable::setLastErrorCall();
    return Function::OK;
}
/*--------------------------------------------------------------------------*/
