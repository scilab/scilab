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
#include "double.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_saveafterncommands(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() == 0)
    {
        int num = HistoryManager::getInstance()->getAfterHowManyLinesHistoryIsSaved();
        out.push_back(new types::Double((double)num));
    }
    else if (in.size() == 1)
    {
        if ((in[0]->isDouble() == false) || (in[0]->getAs<types::Double>()->isScalar() == false))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A double expected.\n"), "saveafterncommands", 1);
            return types::Function::Error;
        }

        char* pstFilename = HistoryManager::getInstance()->getFilename();
        if (pstFilename == NULL)
        {
            HistoryManager::getInstance()->setDefaultFilename();
            pstFilename = HistoryManager::getInstance()->getFilename();
        }
        int iLine = (int)in[0]->getAs<types::Double>()->get(0);
        HistoryManager::getInstance()->setAfterHowManyLinesHistoryIsSaved(iLine);
        FREE(pstFilename);
        pstFilename = NULL;
    }
    else
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "saveafterncommands", 0, 1);
        return types::Function::Error;
    }
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

