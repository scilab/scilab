/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO- Antoine ELIAS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
types::Function::ReturnValue sci_lasterror(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    bool bClearError = true;
    if (in.size() > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected."), "lasterror", 0, 1);
        return types::Function::Error;
    }

    if (_iRetCount > 4)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "lasterror", 1, 4);
        return types::Function::Error;
    }

    if (in.size() == 1)
    {
        if (in[0]->isBool() == false || in[0]->getAs<types::Bool>()->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Boolean vector expected.\n"), "lasterror", 1);
            return types::Function::Error;
        }
        bClearError = in[0]->getAs<types::Bool>()->get()[0] == 1; //convert int to bool
    }

    // check on error number because error message can be empty.
    if (ConfigVariable::getLastErrorNumber() == 0)
    {
        out.push_back(types::Double::Empty());
    }
    else
    {
        std::wstring wstLastErrorMessage = ConfigVariable::getLastErrorMessage();
        std::vector<std::wstring> vectLines;
        std::wistringstream iss(wstLastErrorMessage);
        std::wstring line;
        // get all lines
        while (std::getline( iss, line, L'\n' ))
        {
            vectLines.push_back(line);
        }

        // do not create an empty line if the end of the error message is '\n'
        if (vectLines.back() == L"")
        {
            vectLines.pop_back();
        }

        types::String* StrLastError = new types::String((int)vectLines.size(), 1);
        // put lines in output
        for (int i = 0; i < (int)vectLines.size(); i++)
        {
            StrLastError->set(i, vectLines[i].c_str());
        }

        vectLines.clear();
        out.push_back(StrLastError);
    }

    if (_iRetCount > 1)
    {
        types::Double* pErrorNumber = new types::Double(ConfigVariable::getLastErrorNumber());
        out.push_back(pErrorNumber);

        if (_iRetCount > 2)
        {
            types::Double* pErrorLine = new types::Double(ConfigVariable::getLastErrorLine());
            out.push_back(pErrorLine);

            if (_iRetCount > 3)
            {
                std::wstring wstLastErrorFunction = ConfigVariable::getLastErrorFunction();
                if (wstLastErrorFunction.size() == 0)
                {
                    out.push_back(new types::String(L""));
                }
                else
                {
                    out.push_back(new types::String(wstLastErrorFunction.c_str()));
                }
            }
        }
    }

    if (bClearError)
    {
        ConfigVariable::clearLastError();
    }

    ConfigVariable::setLastErrorCall();
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
