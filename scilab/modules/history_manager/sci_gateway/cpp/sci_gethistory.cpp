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
#include "double.hxx"
#include "string.hxx"

extern "C"
{
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_gethistory(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if(in.size() > 1)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"gethistory", 0, 1);
        return types::Function::Error;
    }

    if(HistoryManager::getInstance()->getNumberOfLines() <= 0)
    {
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }

    if(in.size() == 1)
    {
        if((in[0]->isDouble() == false) || (in[0]->getAs<types::Double>()->isScalar() == false))
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A double expected.\n"), L"gethistory", 1);
            return types::Function::Error;
        }

        types::Double* pDbl = in[0]->getAs<types::Double>();
        char* pstLine = HistoryManager::getInstance()->getNthLine((int)pDbl->get(0));
        if(pstLine)
        {
            out.push_back(new types::String(pstLine));
            FREE(pstLine);
        }
        else
        {
		    ScierrorW(999,_W("%ls: An error occurred: Get line %d failed.\n"),L"gethistory", pDbl->get(0));
            return types::Function::Error;
        }
    }
    else
    {
        int nbElements = 0;
        char** pstLines = NULL;
        pstLines = HistoryManager::getInstance()->getAllLines(&nbElements);
        /* SWIG need array finish with NULL */
        if (pstLines)
        {
            types::String* pS = new types::String(nbElements, 1);
            for(int i = 0 ; i < nbElements ; i++)
            {
                wchar_t* pwstLine = to_wide_string(pstLines[i]);
                pS->set(i, pwstLine);
                FREE(pwstLine);
            }

            out.push_back(pS);
            freeArrayOfString(pstLines, nbElements);
        }
        else
        {
            out.push_back(types::Double::Empty());
        }
    }
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

