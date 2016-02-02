/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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
/*--------------------------------------------------------------------------*/
#include "core_gw.hxx"
#include "double.hxx"
#include "string.hxx"
#include "function.hxx"
#include <list>
#include "configvariable.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "os_string.h"
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_intppty(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    bool bAdd = true;
    if (in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected."), "intppty", 0, 2);
        return types::Function::Error;
    }

    if (in.size() == 2)
    {
        if (in[1]->isString() == false || in[1]->getAs<types::String>()->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: a string expected.\n"), "intppty", 2);
            return types::Function::Error;
        }

        types::String* pMode = in[1]->getAs<types::String>();
        if (os_wcsicmp(pMode->get(0), L"add") == 0)
        {
            bAdd = true;
        }
        else if (os_wcsicmp(pMode->get(0), L"remove") == 0)
        {
            bAdd = false;
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), "intppty", 2, "add", "remove");
            return types::Function::Error;
        }

    }

    if (in.size() > 0)
    {
        if (in[0]->isString() == false || in[0]->getAs<types::String>()->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: a string expected.\n"), "intppty", 1);
            return types::Function::Error;
        }


        types::String* pS = in[0]->getAs<types::String>();

        if (bAdd)
        {
            ConfigVariable::addReferenceModule(pS->get(0));
        }
        else
        {
            ConfigVariable::removeReferenceModule(pS->get(0));
        }
    }
    else
    {
        //get list of reference modules
        std::list<std::wstring> l = ConfigVariable::getReferenceModules();
        if (l.size() == 0)
        {
            out.push_back(types::Double::Empty());
        }
        else
        {
            types::String* pOut = new types::String(1, (int)l.size());
            std::list<std::wstring>::iterator it = l.begin();

            for (int i = 0; it != l.end() ; ++it, ++i)
            {
                pOut->set(i, (*it).c_str());
            }

            out.push_back(pOut);
        }
    }
    return types::Function::OK;
}
