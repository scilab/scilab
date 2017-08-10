/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
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

#include "graphics_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "graphichandle.hxx"
#include "overload.hxx"
#include "string.hxx"

extern "C"
{
#include "sci_malloc.h"
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
#include "FigureList.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_get(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    long long llH = 0;
    types::String* pS = NULL;
    int iObjUID = 0;
    types::InternalType* pOut = NULL;

    if (in.size() < 1 || in.size() > 2)
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
        Overload::generateNameAndCall(L"get", in, _iRetCount, out);
        return types::Function::OK;
    }

    if (p1->isDouble())
    {
        types::Double* pDbll1 = p1->getAs<types::Double>();
        if (pDbll1->isScalar() == false)
        {
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), "get", 1);
            return types::Function::Error;
        }

        double pdbll1 = pDbll1->get(0);
        if (pdbll1 != 0)
        {
            Overload::generateNameAndCall(L"get", in, _iRetCount, out);
            return types::Function::OK;
        }

        /* Console property */
        if (in.size() == 1)
        {
            types::InternalType* pIT = static_cast<types::InternalType*>(sciReturnHandle(getHandle(getConsoleIdentifier())));
            if (pIT == NULL)
            {
                /* An error has occurred */
                return types::Function::OK;
            }

            out.push_back(pIT);
            return types::Function::OK;
        }

        if (in[1]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "get", 2);
            return types::Function::Error;
        }

        types::String* pStr = in[1]->getAs<types::String>();
        if (pStr->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "get", 2);
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
        return types::Function::OK;
    }
    else if (p1->isHandle())
    {
        types::GraphicHandle* pH = p1->getAs<types::GraphicHandle>();
        if (pH->isScalar() == false)
        {
            return Overload::call(L"%h_get", in, _iRetCount, out);
        }

        if (in.size() == 1)
        {
            //get path from handle
            int uic = getObjectFromHandle((long)pH->get(0));
            char* path = get_path(uic);
            if (path[0] == '\0')
            {
                Scierror(999, _("%s: Unable to get useful path from this handle.\n"), "get");
                FREE(path);
                return types::Function::Error;
            }

            pOut = new types::String(path);
            FREE(path);
            out.push_back(pOut);
            return types::Function::OK;
        }

        if (in[1]->isString() == false)
        {
            Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), "get", 2);
            return types::Function::Error;
        }

        pS = in[1]->getAs<types::String>();

        if (pS->isScalar() == false)
        {
            Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), "get", 2);
            return types::Function::Error;
        }

        llH = pH->get(0);
    }
    else if (p1->isString())
    {
        if (in[0]->isString() == false)
        {
            Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), "get", 1);
            return types::Function::Error;
        }

        pS = in[0]->getAs<types::String>();
        if (pS->isScalar() == false)
        {
            Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), "get", 1);
            return types::Function::Error;
        }

        wchar_t* pstProperty = pS->get(0);

        if (wcscmp(pstProperty, L"default_figure")  &&
                wcscmp(pstProperty, L"default_axes")    &&
                wcscmp(pstProperty, L"current_figure")  &&
                wcscmp(pstProperty, L"current_axes")    &&
                wcscmp(pstProperty, L"current_entity")  &&
                wcscmp(pstProperty, L"hdl")             &&
                wcscmp(pstProperty, L"figures_id"))
        {
            char* pstProperty = wide_string_to_UTF8(pS->get(0));
            int uid = search_path(pstProperty);
            FREE(pstProperty);

            if (uid != 0)
            {
                llH = getHandle(uid);
                if (in.size() == 1)
                {
                    out.push_back(new types::GraphicHandle(llH));
                    return types::Function::OK;
                }

                if (in[1]->isString() == false)
                {
                    Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), "get", 2);
                    return types::Function::Error;
                }

                pS = in[1]->getAs<types::String>();
                if (pS->isScalar() == false)
                {
                    Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), "get", 2);
                    return types::Function::Error;
                }
            }
            else
            {
                out.push_back(types::Double::Empty());
                return types::Function::OK;
            }
        }
    }
    else
    {
        // Overload
        return Overload::call(L"%" + p1->getShortTypeStr() + L"_get", in, _iRetCount, out);
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
        pOut = (types::InternalType*)callGetProperty(NULL, iObjUID, pstProperty);
    }
    else
    {
        pOut = (types::InternalType*)callGetProperty(NULL, 0, pstProperty);
    }

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
