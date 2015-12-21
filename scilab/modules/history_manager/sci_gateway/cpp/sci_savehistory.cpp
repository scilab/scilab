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
#include "HistoryManager.hxx"
#include "string.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "localization.h"
#include "Scierror.h"
#include "expandPathVariable.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_savehistory(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() == 0)
    {
        char* pstFilename = HistoryManager::getInstance()->getFilename();
        if (pstFilename)
        {
            HistoryManager::getInstance()->writeToFile(pstFilename);
            FREE(pstFilename);
            pstFilename = NULL;
        }
    }
    else if (in.size() == 1)
    {
        if ((in[0]->isString() == false) || (in[0]->getAs<types::String>()->isScalar() == false))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "savehistory", 1);
            return types::Function::Error;
        }

        wchar_t* pwscsPath = in[0]->getAs<types::String>()->get(0);
        wchar_t* pwcsLine = expandPathVariableW(pwscsPath);
        if (pwcsLine)
        {
            char* pstLine = wide_string_to_UTF8(pwcsLine);
            if (pstLine)
            {
                HistoryManager::getInstance()->writeToFile(pstLine);
                FREE(pstLine);
                pstLine = NULL;

            }
            FREE(pwcsLine);
            pwcsLine = NULL;
        }
    }
    else
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "savehistory", 0, 1);
        return types::Function::Error;
    }
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

