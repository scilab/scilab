/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/

#include "history_manager_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "HistoryManager.hxx"

extern "C"
{
#include "MALLOC.h"
#include "getCommentDateSession.h"
#include "HistoryManager.h"
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_historymanager(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pStr = NULL;

    if(in.size() == 0)
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
    else if(in.size() == 1)
    {
        if((in[0]->isString() == false) || (in[0]->getAs<types::String>()->isScalar() == false))
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A string expected.\n"), L"historymanager", 1);
            return types::Function::Error;
        }

        pStr = in[0]->getAs<types::String>();
        if(wcscmp(pStr->get(0),L"on") == 0)
        {
            char* pstCommentBeginSession = getCommentDateSession(FALSE);
            HistoryManager::getInstance()->appendLine(pstCommentBeginSession);
            out.push_back(new types::String(L"on"));
            FREE(pstCommentBeginSession);
        }
        else if(wcscmp(pStr->get(0),L"off") == 0)
        {
            HistoryManager::killInstance();
            out.push_back(new types::String(L"off"));
        }
        else
        {
            ScierrorW(999,_W("%ls: Wrong value for input argument #%d: Must be '%ls' or '%ls'.\n"), L"historymanager", 1, L"on", L"off");
            return types::Function::Error;
        }
    }
    else
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"historymanager", 0, 1);
        return types::Function::Error;
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

