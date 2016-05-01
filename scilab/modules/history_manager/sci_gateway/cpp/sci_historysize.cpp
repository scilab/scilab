/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
* Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
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
#include "HistoryManager.hxx"
#include "double.hxx"
#include "string.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_historysize(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "historysize", 0, 1);
        return types::Function::Error;
    }

    if (in.size() == 0)
    {
        int size = HistoryManager::getInstance()->getNumberOfLines() - 1;
        out.push_back(new types::Double((double)size));
        return types::Function::OK;
    }

    types::InternalType* pIT = in[0];
    if (pIT->isString() == false && pIT->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d.\n"), "historysize", 1);
        return types::Function::Error;
    }

    if (pIT->isString())
    {
        types::String* pS = pIT->getAs<types::String>();
        if (pS->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d.\n"), "historysize", 1);
            return types::Function::Error;
        }

        if (wcscmp(pS->get(0), L"max"))
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: \"%s\" expected.\n"), "historysize", 1, "max");
            return types::Function::Error;
        }

        int ret = HistoryManager::getInstance()->getNumberOfLinesMax();
        out.push_back(new types::Double((double)ret));
    }
    else //if (pIT->isDouble())
    {
        types::Double* pD = pIT->getAs<types::Double>();
        if (pD->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d.\n"), "historysize", 1);
            return types::Function::Error;
        }

        if (HistoryManager::getInstance()->setNumberOfLinesMax((int)pD->get(0)) == FALSE)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d.\n"), "historysize", 1);
            return types::Function::Error;
        }

        out.push_back(pD);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

