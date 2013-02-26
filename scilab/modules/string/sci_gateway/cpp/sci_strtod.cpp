/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Digiteo 2011 - Cedric DELAMARRE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
}


types::Function::ReturnValue sci_strtod(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pOutDouble = NULL;
    types::String* pOutString = NULL;
    types::String* pString = NULL;

    wchar_t pwstKey[] = L"1234567890";
    unsigned long long ullNan = 0x7ff8000000000000;
    double dblNan = *( double* )&ullNan;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "strtod", 1);
        return types::Function::Error;
    }
    if (_iRetCount > 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "strtod", 1, 2);
        return types::Function::Error;
    }

    if (in[0]->isDouble() && in[0]->getAs<types::Double>()->isEmpty())
    {
        out.push_back(new types::Double(dblNan));
        if (_iRetCount == 2)
        {
            out.push_back(new types::String(L""));
        }

        return types::Function::OK;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings or empty matrix expected.\n"), "strtod", 1);
    }

    pString = in[0]->getAs<types::String>();

    pOutDouble = new types::Double(pString->getDims(), pString->getDimsArray());
    if (_iRetCount == 2)
    {
        pOutString = new types::String(pString->getDims(), pString->getDimsArray());
    }

    for (int i = 0 ; i < pString->getSize() ; i++)
    {
        //Double part
        bool bStop = false;
        wchar_t *pwstStop = NULL;
        wchar_t* pstStr = pString->get(i);
        int iPos = (int)wcscspn(pstStr, pwstKey);

        //Check if there is a number in the string
        if (iPos)
        {
            for (int j = 0 ; j < iPos ; j++)
            {
                if (pstStr[j] != ' ') // spaces are accepted
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
                if (wcslen(pstStr) == iPos) // strtod("  ")
                {
                    pOutDouble->set(i, dblNan);
                    pwstStop = pstStr;
                }
                else // strtod("  000xxx")
                {
                    pOutDouble->set(i, wcstod(pstStr + iPos, &pwstStop));
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

    out.push_back(pOutDouble);

    if (_iRetCount == 2)
    {
        out.push_back(pOutString);
    }

    return types::Function::OK;
}

