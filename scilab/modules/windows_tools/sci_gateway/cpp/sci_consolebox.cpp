/*
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "windows_tools_gw.hxx"
#include "function.hxx"
#include "string.hxx"

extern "C"
{
#include "Scierror.h"
#include "sciprint.h"
#include "localization.h"
#include "WinConsole.h"
#include "sci_mode.h"
}

using namespace types;

Function::ReturnValue sci_consolebox(typed_list &in, int _iRetCount, typed_list &out)
{
    if (in.size() < 0 || in.size() > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d or %d expected.\n"), "consolebox", 0, 1);
        return Function::Error;
    }

    if (getScilabMode() != SCILAB_STD)
    {
        sciprint(_("Only on Windows Mode, not in Console Mode.\n"));
        out.push_back(new String(L"off"));
        return Function::OK;
    }


    //request mode
    if (in.size() == 0)
    {
        if (GetConsoleState())
        {
            out.push_back(new String(L"on"));
        }
        else
        {
            out.push_back(new String(L"off"));
        }
        return Function::OK;
    }

    InternalType* pIT1 = in[0];

    if (pIT1->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "consolebox", 1);
        return Function::Error;
    }

    String* pS1 = pIT1->getAs<String>();
    if (pS1->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), "consolebox", 1);
        return Function::Error;
    }

    std::wstring wcsAction(pS1->get(0));

    if (wcsAction == L"on")
    {
        SetConsoleState(1);
        ShowScilex();
    }
    else if (wcsAction == L"off")
    {
        SetConsoleState(0);
        HideScilex();
    }
    else if (wcsAction == L"toggle")
    {
        SwitchConsole();
    }
    else
    {
        Scierror(999, ("%s: Wrong input argument: '%s', '%s' or '%s' expected.\n"), "consolebox", "on", "off", "toggle");
        return Function::Error;
    }

    if (GetConsoleState())
    {
        out.push_back(new String(L"on"));
    }
    else
    {
        out.push_back(new String(L"off"));
    }

    return Function::OK;
}
/*--------------------------------------------------------------------------*/
