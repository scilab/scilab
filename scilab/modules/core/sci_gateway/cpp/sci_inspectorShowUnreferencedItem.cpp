/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
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

#include <string>
#include "core_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "inspector.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "os_string.h"
}

types::Function::ReturnValue sci_inspectorShowUnreferencedItem(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() > 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d or %d expected.\n"), "inspectorShowUnreferencedItem", 0, 1);
        return types::Function::Error;
    }

    if (in.size() == 0)
    {
        int iCount = types::Inspector::getUnreferencedItemCount();
        types::String* pS = new types::String(iCount, 1);
        for (int i = 0 ; i < iCount ; i++)
        {
            pS->set(i, types::Inspector::showUnreferencedItem(i).c_str());
        }
        out.push_back(pS);
    }
    else
    {
        if (in[0]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "inspectorShowItem", 1);
            return types::Function::Error;
        }

        types::Double *pD = in[0]->getAs<types::Double>();
        if (pD->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "inspectorShowItem", 1);
            return types::Function::Error;
        }

        int iPos = (int)pD->get(0) - 1;
        out.push_back(new types::String(types::Inspector::showUnreferencedItem(iPos).c_str()));
    }
    return types::Function::OK;
}
