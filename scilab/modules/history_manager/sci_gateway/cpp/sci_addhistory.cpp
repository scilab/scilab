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
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_addhistory(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pStr = NULL;
    BOOL bOK            = FALSE;

    if(in.size() == 1)
    {
        if((in[0]->isString() == false))
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A matrix of string expected.\n"), L"addhistory", 1);
            return types::Function::Error;
        }
        pStr = in[0]->getAs<types::String>();
        int iSize = pStr->getSize();

        for(int i = 0 ; i < iSize ; i++)
        {
            char* pstLine = wide_string_to_UTF8(pStr->get(i));
            if(pstLine)
            {
                bOK = HistoryManager::getInstance()->appendLine(pstLine);
                FREE(pstLine);
                pstLine = NULL;
            }
        }

        if(!bOK)
        {
            ScierrorW(999, _W("%ls: Append lines in Scilab history failed.\n"), L"addhistory");
            return types::Function::Error;
        }
    }
    else
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"addhistory", 1);
        return types::Function::Error;
    }
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

