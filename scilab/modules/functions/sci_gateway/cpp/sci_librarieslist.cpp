/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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

#include <string.h>
#include "funcmanager.hxx"
#include "context.hxx"
#include "functions_gw.hxx"
#include "string.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_librarieslist(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 0)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "librarieslist", 0);
        return types::Function::Error;
    }

    std::list<std::wstring> lst;
    int size = symbol::Context::getInstance()->getLibrariesList(lst);

    types::String* pOut = new types::String(size, 1);
    int i = 0;
    for (auto l : lst)
    {
        pOut->set(i++, l.c_str());
    }

    out.push_back(pOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
