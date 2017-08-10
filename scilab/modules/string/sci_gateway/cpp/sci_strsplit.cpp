/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
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

#include "string_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "double.hxx"
#include "bool.hxx"
#include "overload.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "Scierror.h"
#include "localization.h"
#include "strsplit.h"
#include "freeArrayOfString.h"
}

types::Function::ReturnValue sci_strsplit(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pStrIn = NULL;
    int iValueThree = 0;

    if (in.size() < 1 || in.size() > 3)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), "strsplit", 1, 3);
        return types::Function::Error;
    }

    if (_iRetCount != 1 && _iRetCount != 2)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d to %d expected.\n"), "strsplit", 1, 2);
        return types::Function::Error;
    }

    // [[], ""] = strsplit([],...)
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
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "strsplit", 1);
        return types::Function::Error;
    }

    pStrIn = in[0]->getAs<types::String>();

    if (pStrIn->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), "strsplit", 1);
        return types::Function::Error;
    }

    if (in.size() > 2)
    {
        if (in[2]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A double expected.\n"), "strsplit", 3);
            return types::Function::Error;
        }

        types::Double* pDblIn = in[2]->getAs<types::Double>();

        if (pDblIn->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar double expected.\n"), "strsplit", 3);
            return types::Function::Error;
        }

        iValueThree = (int)pDblIn->get(0);

        if ( (double)iValueThree != pDblIn->get(0))
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), "strsplit", 3);
            return types::Function::Error;
        }

        if ((iValueThree < 1) && (iValueThree != -1))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A positive value expected.\n"), "strsplit", 3);
            return types::Function::Error;
        }
    }

    if (in.size() > 1)
    {
        if (in[1]->isDouble())
        {
            types::Double* pDbl = in[1]->getAs<types::Double>();

            if (_iRetCount == 2)
            {
                Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), "strsplit", 1);
                return types::Function::Error;
            }

            if (pDbl->getRows() != 1 && pDbl->getCols() != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A Scalar or vector expected.\n"), "strsplit", 2);
                return types::Function::Error;
            }

            strsplit_error ierr = STRSPLIT_NO_ERROR;
            wchar_t **results = strsplit(pStrIn->get(0), pDbl->get(), pDbl->getSize(), &ierr);

            switch (ierr)
            {
                case STRSPLIT_NO_ERROR:
                {
                    types::String* pStrOut = new types::String(pDbl->getSize() + 1, 1);
                    pStrOut->set(results);

                    freeArrayOfWideString(results, pDbl->getSize() + 1);
                    out.push_back(pStrOut);
                    return types::Function::OK;
                }
                break;
                case STRSPLIT_INCORRECT_VALUE_ERROR:
                {
                    freeArrayOfWideString(results, pDbl->getSize() + 1);
                    Scierror(999, _("%s: Wrong value for input argument #%d.\n"), "strsplit", 2);
                    return types::Function::Error;
                }
                break;
                case STRSPLIT_INCORRECT_ORDER_ERROR:
                {
                    freeArrayOfWideString(results, pDbl->getSize() + 1);
                    Scierror(999, _("%s: Elements of %dth argument must be in increasing order.\n"), "strsplit", 2);
                    return types::Function::Error;
                }
                break;
                case STRSPLIT_MEMORY_ALLOCATION_ERROR:
                {
                    freeArrayOfWideString(results, pDbl->getSize() + 1);
                    Scierror(999, _("%s: Memory allocation error.\n"), "strsplit");
                    return types::Function::Error;
                }
                break;
                default:
                {
                    freeArrayOfWideString(results, pDbl->getSize() + 1);
                    Scierror(999, _("%s: error.\n"), "strsplit");
                    return types::Function::Error;
                }
                break;
            }
        }
        else if (in[1]->isString())
        {
            types::String* pStr = in[1]->getAs<types::String>();
            if (pStr->isScalar() == false)
            {
                // checks that 2nd parameter is not an array of regexp pattern
                wchar_t** pwcsStr = pStr->get();
                for (int i = 0; i < pStr->getSize(); i++)
                {
                    if (pwcsStr[i])
                    {
                        int iLen = (int)wcslen(pwcsStr[i]);
                        if (iLen > 2 && pwcsStr[i][0] == L'/' && pwcsStr[i][iLen - 1] == L'/')
                        {
                            Scierror(999, _("%s: Wrong value for input argument #%d: a string expected, not a regexp pattern.\n"), "strsplit", 2);
                            return types::Function::Error;
                        }
                    }
                }
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A double or string expected.\n"), "strsplit", 2);
            return types::Function::Error;
        }
    }

    return Overload::call(L"%_strsplit", in, _iRetCount, out);
}
/*-------------------------------------------------------------------------------------*/
