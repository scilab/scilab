/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright(C) 2015 - Scilab Enterprises - Antoine ELIAS
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
#include "windows_tools_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "registry.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "PATH_MAX.h"
#include "freeArrayOfString.h"
}
/*--------------------------------------------------------------------------*/
const std::string fname = "winqueryreg";
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_winqueryreg(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    std::wstring param1;
    std::wstring param2;
    std::wstring param3;
    types::String* pS = nullptr;

    int rhs = static_cast<int>(in.size());
    if (rhs != 2 && rhs != 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), fname.data(), 2, 3);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    pS = in[0]->getAs<types::String>();
    if (pS->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: String expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    param1 = pS->get()[0];

    if (in[1]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname.data(), 2);
        return types::Function::Error;
    }

    pS = in[1]->getAs<types::String>();
    if (pS->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: String expected.\n"), fname.data(), 2);
        return types::Function::Error;
    }

    param2 = pS->get()[0];

    if (rhs == 3)
    {
        if (in[2]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname.data(), 3);
            return types::Function::Error;
        }

        pS = in[2]->getAs<types::String>();
        if (pS->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: String expected.\n"), fname.data(), 3);
            return types::Function::Error;
        }

        param3 = pS->get()[0];

        bool bKey = param1 == L"key";
        bool bValue = param1 == L"name";

        if (bKey || bValue)
        {
            int count = 0;
            if (bValue)
            {
                WindowsQueryRegistryNumberOfValuesInList(param2, param3, count);
            }
            else
            {
                WindowsQueryRegistryNumberOfKeysInList(param2, param3, count);
            }

            if (count)
            {
                bool res = false;
#define MAX_ELMT_REGLIST 255
                std::vector<std::wstring> listKeysName;

                if (count > MAX_ELMT_REGLIST)
                {
                    count = MAX_ELMT_REGLIST;
                }

                if (bValue)
                {
                    res = WindowsQueryRegistryValuesList(param2, param3, count, listKeysName);
                }
                else
                {
                    res = WindowsQueryRegistryKeysList(param2, param3, count, listKeysName);
                }

                if (res)
                {
                    int i = 0;
                    pS = new types::String(count, 1);
                    wchar_t** p = pS->get();
                    for (auto l : listKeysName)
                    {
                        p[i++] = os_wcsdup(l.data());
                    }

                    out.push_back(pS);
                    return types::Function::OK;
                }
                else
                {
                    Scierror(999, _("%s: Cannot open Windows registry.\n"), fname);
                    return types::Function::Error;
                }
            }
            else
            {
                out.push_back(types::Double::Empty());
                return types::Function::OK;
            }
        }
        else
        {
            //continue, third param is a valname
        }
    }

    bool isStringKey = false;
    std::wstring outputSt;
    int outputInt = 0;
    bool bTest = WindowsQueryRegistry(param1, param2, param3, outputSt, outputInt, isStringKey);
    if (bTest)
    {
        if (isStringKey)
        {
            out.push_back(new types::String(outputSt.c_str()));
        }
        else
        {
            out.push_back(new types::Double(outputInt));
        }
    }
    else
    {
        Scierror(999, _("%s: Cannot query value of this type.\n"), fname.data());
        return types::Function::Error;
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
