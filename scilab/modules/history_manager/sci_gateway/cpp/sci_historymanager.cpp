/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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

#include "history_manager_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "HistoryManager.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "getCommentDateSession.h"
#include "HistoryManager.h"
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_historymanager(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pStr = NULL;

    if (in.size() == 0)
    {
        if (HistoryManager::historyIsEnabled())
        {
            out.push_back(new types::String(L"on"));
        }
        else
        {
            out.push_back(new types::String(L"off"));
        }
    }
    else if (in.size() == 1)
    {
        if ((in[0]->isString() == false) || (in[0]->getAs<types::String>()->isScalar() == false))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "historymanager", 1);
            return types::Function::Error;
        }

        pStr = in[0]->getAs<types::String>();
        if (wcscmp(pStr->get(0), L"on") == 0)
        {
            char* pstCommentBeginSession = getCommentDateSession(FALSE);
            HistoryManager::getInstance()->appendLine(pstCommentBeginSession);
            out.push_back(new types::String(L"on"));
            FREE(pstCommentBeginSession);
        }
        else if (wcscmp(pStr->get(0), L"off") == 0)
        {
            HistoryManager::killInstance();
            out.push_back(new types::String(L"off"));
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Must be '%s' or '%s'.\n"), "historymanager", 1, "on", "off");
            return types::Function::Error;
        }
    }
    else
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "historymanager", 0, 1);
        return types::Function::Error;
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

