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

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_removelinehistory(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() == 1)
    {
        if ((in[0]->isDouble() == false) || (in[0]->getAs<types::Double>()->isScalar() == false))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A double expected.\n"), "removelinehistory", 1);
            return types::Function::Error;
        }

        types::Double* pDbl = in[0]->getAs<types::Double>();
        int size = HistoryManager::getInstance()->getNumberOfLines() - 1;

        if ((pDbl->get(0) < 0) || (pDbl->get(0) > size))
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the interval [%d, %d].\n"), "removelinehistory", 1, 0, size);
            return types::Function::Error;
        }

        HistoryManager::getInstance()->deleteNthLine(pDbl->get(0));
    }
    else
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "removelinehistory", 1);
        return types::Function::Error;
    }
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

