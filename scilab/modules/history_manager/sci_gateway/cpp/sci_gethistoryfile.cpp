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
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_gethistoryfile(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if(in.size() != 0)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "gethistoryfile", 0);
        return types::Function::Error;
    }

    char* pstFilename = HistoryManager::getInstance()->getFilename();

    if(pstFilename)
    {
        out.push_back(new types::String(pstFilename));
        FREE(pstFilename);
    }
    else
    {
		Scierror(999, _("%s: An error occurred: %s\n"), "gethistoryfile", _("filename not defined."));
        return types::Function::Error;
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

