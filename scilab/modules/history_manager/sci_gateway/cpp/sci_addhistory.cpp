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
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_addhistory(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pStr = NULL;
    BOOL bOK            = FALSE;

    if (in.size() == 1)
    {
        if ((in[0]->isString() == false))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of string expected.\n"), "addhistory", 1);
            return types::Function::Error;
        }
        pStr = in[0]->getAs<types::String>();
        int iSize = pStr->getSize();

        for (int i = 0 ; i < iSize ; i++)
        {
            char* pstLine = wide_string_to_UTF8(pStr->get(i));
            if (pstLine)
            {
                bOK = HistoryManager::getInstance()->appendLine(pstLine);
                FREE(pstLine);
                pstLine = NULL;
            }
        }

        if (!bOK)
        {
            Scierror(999, _("%s: Append lines in Scilab history failed.\n"), "addhistory");
            return types::Function::Error;
        }
    }
    else
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "addhistory", 1);
        return types::Function::Error;
    }
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

