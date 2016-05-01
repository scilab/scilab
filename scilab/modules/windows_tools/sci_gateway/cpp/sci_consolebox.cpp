/*
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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

#include "windows_tools_gw.hxx"
#include "function.hxx"
#include "string.hxx"

extern "C"
{
#include "Scierror.h"
#include "sciprint.h"
#include "localization.h"
#include "WinConsole.h"
#include "configvariable_interface.h"
}

types::Function::ReturnValue sci_consolebox(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() < 0 || in.size() > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d or %d expected.\n"), "consolebox", 0, 1);
        return types::Function::Error;
    }

    if (getScilabMode() != SCILAB_STD)
    {
        sciprint(_("Only on Windows Mode, not in Console Mode.\n"));
        out.push_back(new types::String(L"off"));
        return types::Function::OK;
    }


    //request mode
    if (in.size() == 0)
    {
        if (GetConsoleState())
        {
            out.push_back(new types::String(L"on"));
        }
        else
        {
            out.push_back(new types::String(L"off"));
        }
        return types::Function::OK;
    }

    types::InternalType* pIT1 = in[0];

    if (pIT1->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "consolebox", 1);
        return types::Function::Error;
    }

    types::String* pS1 = pIT1->getAs<types::String>();
    if (pS1->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), "consolebox", 1);
        return types::Function::Error;
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
        return types::Function::Error;
    }

    if (GetConsoleState())
    {
        out.push_back(new types::String(L"on"));
    }
    else
    {
        out.push_back(new types::String(L"off"));
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
