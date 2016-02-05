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

types::Function::ReturnValue sci_inspectorShowItem(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() > 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d or %d expected.\n"), "inspectorShowItem", 0, 1);
        return types::Function::Error;
    }

    if (in.size() == 0)
    {
        int iCount = types::Inspector::getItemCount();
        types::String* pS = new types::String(iCount, 2);
        for (int i = 0 ; i < iCount ; i++)
        {
            wchar_t pstRef[10];
            pS->set(i, 0, types::Inspector::showItem(i).c_str());

            os_swprintf(pstRef, 10, L"%d", types::Inspector::getItem(i)->getRef());
            pS->set(i, 1, pstRef);
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
        if (pD->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "inspectorShowItem", 1);
            return types::Function::Error;
        }

        int iPos = (int)pD->get(0) - 1;
        out.push_back(new types::String(types::Inspector::showItem(iPos).c_str()));
    }
    return types::Function::OK;
}
