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

