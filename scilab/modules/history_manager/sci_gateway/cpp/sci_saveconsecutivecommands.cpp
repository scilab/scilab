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
#include "HistoryManager.hxx"
#include "function.hxx"
#include "bool.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_saveconsecutivecommands(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() == 0)
    {
        BOOL bSave = HistoryManager::getInstance()->getSaveConsecutiveDuplicateLines();
        out.push_back(new types::Bool(bSave));
    }
    else if (in.size() == 1)
    {
        wchar_t* line = NULL;

        if ((in[0]->isBool() == false) || (in[0]->getAs<types::Bool>()->isScalar() == false))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), "saveconsecutivecommands", 1);
            return types::Function::Error;
        }
        BOOL bSave = (BOOL)in[0]->getAs<types::Bool>()->get(0);
        HistoryManager::getInstance()->setSaveConsecutiveDuplicateLines(bSave);
    }
    else
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "saveafterncommands", 0, 1);
        return types::Function::Error;
    }
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

