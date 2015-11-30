/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "core_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "scilabWrite.hxx"

extern "C"
{
#include "configvariable_interface.h"
#include "charEncoding.h"
#include "sciprint.h"
#include "getos.h"
#include "localization.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include "os_string.h"
#include "Sciwarning.h"
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_warning(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String *psInput = NULL;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "warning", 1);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "warning", 1);
        return types::Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "warning", 1);
        return types::Function::Error;
    }

    psInput = in[0]->getAs<types::String>();

    if (psInput->getSize() == 1)
    {
        /* "on" "off" "query" "stop" */
        if (wcscmp(psInput->get(0), L"on") == 0)
        {
            setWarningMode(TRUE);
            setWarningStop(FALSE);
            return types::Function::OK;
        }

        if (wcscmp(psInput->get(0), L"off") == 0)
        {
            setWarningMode(FALSE);
            setWarningStop(FALSE);
            return types::Function::OK;
        }

        if (wcscmp(psInput->get(0), L"stop") == 0)
        {
            setWarningMode(TRUE);
            setWarningStop(TRUE);
            return types::Function::OK;
        }

        if (wcscmp(psInput->get(0), L"query") == 0)
        {
            if (getWarningMode())
            {
                if (getWarningStop())
                {
                    // WarningMode and WarningStop => warning stop mode active
                    out.push_back(new types::String(L"stop"));
                }
                else
                {
                    // WarningMode and !WarningStop => warning on  mode active
                    out.push_back(new types::String(L"on"));
                }
            }
            else
            {
                out.push_back(new types::String(L"off"));
            }
            return types::Function::OK;
        }
    }

    if (getWarningMode())
    {
        for (int i = 0; i < psInput->getSize() ; ++i)
        {
            wchar_t* pwstTemp = psInput->get(i);
            size_t iSize = (wcslen(pwstTemp) + 10 + 1);
            wchar_t* pwstToPrint = (wchar_t*)MALLOC(sizeof(wchar_t) * iSize);
            os_swprintf(pwstToPrint, iSize, _W("WARNING: %ls\n").c_str(), pwstTemp);
            scilabForcedWriteW(pwstToPrint);
            FREE(pwstToPrint);
        }

        if (getWarningStop())
        {
            Sciwarning("");
        }
    }


    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
