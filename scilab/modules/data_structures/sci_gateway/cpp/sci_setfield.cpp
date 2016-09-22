/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Antoine Elias
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
#include "data_structures_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "list.hxx"
#include "mlist.hxx"
#include "tlist.hxx"

extern "C"
{
#include "Scierror.h"
#include "sci_malloc.h"
#include "localization.h"
#include "freeArrayOfString.h"
}

/*-----------------------------------------------------------------------------------*/
types::Function::ReturnValue sci_setfield(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "setfield", 3);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "setfield", 1);
        return types::Function::Error;
    }


    types::InternalType* pIndex = in[0];
    types::InternalType* pData = in[1];
    types::List* pL = in[2]->getAs<types::List>();
    if (pL->isDeletable() == false)
    {
        pL = pL->clone();
    }

    if (pL->isList() == false && pL->isMList() == false && pL->isTList() == false)
    {
        Scierror(999, _("%s:  Wrong type for input argument #%d: List expected.\n"), "setfield", 3);
        return types::Function::Error;
    }

    if (pIndex->isString())
    {
        //insertion by fieldname
        types::String* pS = pIndex->getAs<types::String>();
        if ((pL->isMList() == false && pL->isTList() == false) || pS->getSize() != 1)
        {
            Scierror(999, _("%s: Not implemented in scilab...\n"), "setfield");
            return types::Function::Error;
        }

        types::TList* pT = pL->getAs<types::TList>();

        std::wstring stField = pS->get(0);
        types::TList* pRet = pT->set(stField, pData);
        if (pRet == nullptr)
        {
            Scierror(999, _("%s: Invalid index.\n"), "setfield");
            return types::Function::Error;
        }

        out.push_back(pRet);
    }
    else
    {
        //insertion by index
        types::typed_list Args;
        Args.push_back(pIndex);
        types::InternalType* pRet = pL->insert(&Args, pData);
        if (pRet == nullptr)
        {
            Scierror(999, _("%s: Invalid index.\n"), "setfield");
            return types::Function::Error;
        }

        out.push_back(pRet);
    }

    return types::Function::OK;
}
/*-----------------------------------------------------------------------------------*/
