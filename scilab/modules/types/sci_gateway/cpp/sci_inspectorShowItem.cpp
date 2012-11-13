/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string>
#include "types_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "inspector.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "os_swprintf.h"
}

using namespace types;

Function::ReturnValue sci_inspectorShowItem(typed_list &in, int _iRetCount, typed_list &out)
{
    if(in.size() > 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d or %d expected.\n"), "inspectorShowItem", 0, 1);    
        return Function::Error;
    }

    if(in.size() == 0)
    {
        int iCount = Inspector::getItemCount();
        String* pS = new String(iCount, 2);
        for(int i = 0 ; i < iCount ; i++)
        {
            wchar_t pstRef[10];
            pS->set(i, 0, Inspector::showItem(i).c_str());

            os_swprintf(pstRef, 10, L"%d", Inspector::getItem(i)->getRef());
            pS->set(i, 1, pstRef);
        }
        out.push_back(pS);
    }
    else
    {
        if(in[0]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "inspectorShowItem", 1);
            return Function::Error;
        }

        Double *pD = in[0]->getAs<Double>();
        if(pD->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "inspectorShowItem", 1);
            Function::Error;
        }

        int iPos = (int)pD->get(0) - 1;
        out.push_back(new String(Inspector::showItem(iPos).c_str()));
    }
    return Function::OK;
}
