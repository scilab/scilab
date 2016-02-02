/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Digiteo 2011 - Cedric DELAMARRE
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


#include "function.hxx"
#include "string.hxx"
#include "list.hxx"
#include "double.hxx"
#include "funcmanager.hxx"
#include "string_gw.hxx"

extern "C"
{
#include "core_math.h"
#include "localization.h"
#include "Scierror.h"
#include "os_string.h"
#include "locale.h"
}


types::Function::ReturnValue sci_strtod(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pOutDouble = NULL;
    types::String* pOutString = NULL;
    types::String* pString = NULL;

    wchar_t pwstKey[] = L"1234567890";
    wchar_t pwstSymbol[] = L"-+.";
    wchar_t wstDecimalSep = L'.';

    unsigned long long ullNan = 0x7ff8000000000000;
    double dblNan = *( double* )&ullNan;

    if (in.size() < 1 || in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "strtod", 1, 2);
        return types::Function::Error;
    }
    if (_iRetCount > 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "strtod", 1, 2);
        return types::Function::Error;
    }

    if (in[0]->isDouble() && in[0]->getAs<types::Double>()->isEmpty())
    {
        out.push_back(types::Double::Empty());
        if (_iRetCount == 2)
        {
            out.push_back(new types::String(L""));
        }

        return types::Function::OK;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings or empty matrix expected.\n"), "strtod", 1);
        return types::Function::Error;
    }

    pString = in[0]->getAs<types::String>();

    pOutDouble = new types::Double(pString->getDims(), pString->getDimsArray());
    if (_iRetCount == 2)
    {
        pOutString = new types::String(pString->getDims(), pString->getDimsArray());
    }

    if (in.size() == 2)
    {
        if (in[1]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A single string expected.\n"), "strtod", 2);
            return types::Function::Error;
        }

        types::String* pString2 = in[1]->getAs<types::String>();
        std::wstring pwstr(pString2->get(0));

        if (pwstr != L"." && pwstr != L",")
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: '.' or ',' expected.\n"), "strtod", 2);
            return types::Function::Error;
        }

        wstDecimalSep = *(pwstr.c_str());
        pwstSymbol[2] = wstDecimalSep;

        if (wstDecimalSep == L',')
        {
#ifdef _MSC_VER
            setlocale(LC_NUMERIC, "French_France.1252");
#else
            setlocale(LC_NUMERIC, "fr_FR.UTF-8");
#endif
        }
    }

    for (int i = 0 ; i < pString->getSize() ; i++)
    {
        //Double part
        bool bStop = false;
        wchar_t *pwstStop = NULL;
        wchar_t* pstStr = pString->get(i);
        int iSign = (int)wcscspn(pstStr, pwstSymbol);
        int iKey = (int)wcscspn(pstStr, pwstKey);


        //symbol can be use only if it is before key
        if (iSign == iKey - 1)
        {
            //let strtod do with symbol
            iKey -= 1;
        }

        //special case for "-.3"
        if (iSign == iKey - 2 && pstStr[iSign + 1] == wstDecimalSep)
        {
            //let strtod do with symbol
            iKey -= 2;
        }

        //Check if there is a number in the string
        if (iKey)
        {
            for (int j = 0 ; j < iKey ; j++)
            {
                if ((pstStr[j] != ' ') && (pstStr[j] != '\t') && (pstStr[j] != '\r'))// spaces are accepted
                {
                    pOutDouble->set(i, dblNan);
                    bStop = true;
                    pwstStop = pstStr;
                    break;
                }
            }

            //it is still a number
            if (bStop == false)
            {
                //only spaces ?
                if (wcslen(pstStr) == iKey) // strtod("  ")
                {
                    pOutDouble->set(i, dblNan);
                    pwstStop = pstStr;
                }
                else // strtod("  000xxx")
                {
                    pOutDouble->set(i, wcstod(pstStr + iKey, &pwstStop));
                }
            }
        }
        else if (wcslen(pstStr) == 0) //strtod("")
        {
            pOutDouble->set(i, dblNan);
        }
        else //all characters are digits
        {
            pOutDouble->set(i, wcstod(pstStr, &pwstStop));
        }

        if (_iRetCount == 2)
        {
            if (pwstStop)
            {
                pOutString->set(i, pwstStop);
            }
            else
            {
                pOutString->set(i, L"");
            }
        }
    }

    if (wstDecimalSep == L',')
    {
        setlocale(LC_NUMERIC, "C");
    }

    out.push_back(pOutDouble);

    if (_iRetCount == 2)
    {
        out.push_back(pOutString);
    }

    return types::Function::OK;
}

