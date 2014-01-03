/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "graphics_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "graphichandle.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"

extern "C"
{
#include "MALLOC.h"
#include "charEncoding.h"
#include "localization.h"
#include "Scierror.h"
#include "BuildObjects.h"
#include "CurrentObject.h"
#include "CurrentSubwin.h"
#include "HandleManagement.h"
#include "GetHashTable.h"
#include "returnProperty.h"
#include "getConsoleIdentifier.h"
#include "GetScreenProperty.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_get(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    long long llH = 0;
    types::String* pS = NULL;
    int iObjUID = 0;
    types::InternalType* pOut = NULL;
    double pdbll1 = 0;

    if (in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "get", 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "get", 1);
        return types::Function::Error;
    }

    types::InternalType* p1 = in[0];

    if (p1->isMList() || p1->isTList())
    {
        ExecVisitor exec;
        Overload::generateNameAndCall(L"get", in, _iRetCount, out, &exec);
        return types::Function::OK;
    }

    if (p1->isDouble())
    {
        double pdbll1 = p1->getAs<types::Double>()->get(0);
        if (pdbll1 == 0) /* Root property */
        {
            if (in.size() == 1)
            {
                types::InternalType* pIT = static_cast<types::InternalType*>(sciReturnHandle(getHandle(getConsoleIdentifier())));
                if (pIT == NULL)
                {
                    /* An error has occurred */
                    return types::Function::OK;
                }

                out.push_back(pIT);
            }
            else // in.size == 2
            {
                if (in[1]->isString() == false)
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: Single string expected.\n"), "get", 2);
                    return types::Function::Error;
                }

                types::String* pStr = in[1]->getAs<types::String>();
                if (pStr->isScalar() == false)
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: Single string expected.\n"), "get", 2);
                    return types::Function::Error;
                }

                char* pstr = wide_string_to_UTF8(pStr->get(0));
                void* pvPropScreen = GetScreenProperty(NULL, pstr);

                if (pvPropScreen == NULL) /* Return property */
                {
                    Scierror(999, _("%s: Could not read property '%s' for root object.\n"), "get", pstr);
                    FREE(pstr);
                    return types::Function::Error;
                }

                FREE(pstr);
                out.push_back(static_cast<types::InternalType*>(pvPropScreen));
            }
        }
        else
        {
            ExecVisitor exec;
            Overload::generateNameAndCall(L"get", in, _iRetCount, out, &exec);
        }
        return types::Function::OK;
    }
    else if (p1->isHandle())
    {
        if (in.size() != 2)
        {
            //Error
            return types::Function::Error;
        }

        types::GraphicHandle* pH = p1->getAs<types::GraphicHandle>();
        if (pH->isScalar() == false)
        {
            ExecVisitor exec;
            return Overload::call(L"%h_get", in, _iRetCount, out, &exec);
        }

        pS = in[1]->getAs<types::String>();
        if (in[1]->isString() == false || pS->isScalar() == false)
        {
            Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), "get", 2);
            return types::Function::Error;
        }

        llH = pH->get(0);
    }
    else if (p1->isString())
    {
        if (in.size() != 1)
        {
            //Error
            return types::Function::Error;
        }

        pS = in[0]->getAs<types::String>();
        if (in[0]->isString() == false || pS->isScalar() == false)
        {
            Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), "get", 1);
            return types::Function::Error;
        }

        wchar_t* pstProperty = pS->get(0);

        if (wcscmp(pstProperty, L"default_figure") && wcscmp(pstProperty, L"default_axes"))
        {
            if ( wcscmp(pstProperty, L"current_figure") &&
                    wcscmp(pstProperty, L"current_axes") &&
                    wcscmp(pstProperty, L"current_entity") &&
                    wcscmp(pstProperty, L"hdl") &&
                    wcscmp(pstProperty, L"figures_id"))
            {
                if ( wcscmp(pstProperty, L"children") &&
                        wcscmp(pstProperty, L"zoom_") &&
                        wcscmp(pstProperty, L"clip_box") &&
                        wcscmp(pstProperty, L"auto_"))
                {
                    getOrCreateDefaultSubwin();
                    llH = getHandle(getCurrentObject());
                }
                else
                {
                    llH = getHandle(getCurrentSubWin());    /* on recupere le pointeur d'objet par le handle */
                }
            }
            else
            {
                llH = 0;
            }
        }
    }

    char* pstProperty = wide_string_to_UTF8(pS->get(0));

    if (llH)
    {
        iObjUID = getObjectFromHandle((long)llH);
        if (iObjUID == 0)
        {
            FREE(pstProperty);
            Scierror(999, _("%s: The handle is not or no more valid.\n"), "get");
            return types::Function::Error;
        }
    }


    pOut = (types::InternalType*)callGetProperty(NULL, iObjUID, pstProperty);
    if (pOut == NULL)
    {
        /* An error has occurred */
        FREE(pstProperty);
        return types::Function::Error;
    }

    out.push_back(pOut);
    FREE(pstProperty);
    return types::Function::OK;
}
