/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Antoine Elias
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include "data_structures_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "list.hxx"
#include "mlist.hxx"
#include "tlist.hxx"
#include "struct.hxx"

extern "C"
{
#include "Scierror.h"
#include "MALLOC.h"
#include "localization.h"
#include "freeArrayOfString.h"
}

static types::Function::ReturnValue sci_getfieldStruct(types::typed_list &in, int _iRetCount, types::typed_list &out);

/*-----------------------------------------------------------------------------------*/
types::Function::ReturnValue sci_getfield(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "getfield", 2);
        return types::Function::Error;
    }

    //special case for struct
    if (in[1]->isStruct())
    {
        return sci_getfieldStruct(in, _iRetCount, out);
    }

    types::InternalType* pIndex = in[0];
    types::List* pL = in[1]->getAs<types::List>();
    std::vector<types::InternalType*> vOut;

    if (pL->isList() == false && pL->isMList() == false && pL->isTList() == false)
    {
        Scierror(999, _("%s:  Wrong type for input argument #%d: List expected.\n"), "getfield", 2);
        return types::Function::Error;
    }

    if (pIndex->isString())
    {
        //extraction by fieldnames
        if (pL->isMList() == false && pL->isTList() == false)
        {
            Scierror(999, _("%s: Soft coded field names not yet implemented.\n"), "getfield");
            return types::Function::Error;
        }

        types::TList* pT = pL->getAs<types::TList>();
        types::String* pS = pIndex->getAs<types::String>();

        std::list<std::wstring> stFields;

        //check output arguments count
        for (int i = 0 ; i < pS->getSize() ; i++)
        {
            std::wstring wst = pS->get(i);
            if (pT->exists(wst) == false)
            {
                Scierror(999, _("%s: Invalid index.\n"), "getfield");
                return types::Function::Error;
            }

            stFields.push_back(pS->get(i));
        }

        vOut = pT->extractStrings(stFields);
    }
    else
    {
        //extraction by index
        types::typed_list Args;
        Args.push_back(pIndex);
        vOut = pL->extract(&Args);
    }


    if (_iRetCount < vOut.size())
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "getfield", vOut.size());
        return types::Function::Error;
    }

    for (int i = 0 ; i < vOut.size() ; i++)
    {
        out.push_back(vOut[i]);
    }

    return types::Function::OK;
}

static types::Function::ReturnValue sci_getfieldStruct(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::InternalType* pIndex = in[0];
    types::Struct* pSt = in[1]->getAs<types::Struct>();
    std::vector<types::InternalType*> vectResult;

    if (pIndex->isString())
    {
        types::String* pFields = pIndex->getAs<types::String>();
        std::vector<std::wstring> wstFields;

        for (int i = 0 ; i < pFields->getSize() ; i++)
        {
            wstFields.push_back(pFields->get(i));
        }

        vectResult = pSt->extractFields(wstFields);
    }
    else
    {
        //extraction by index
        vectResult = pSt->extractFields(&in);
    }

    if (vectResult.size() == 0)
    {
        Scierror(78, _("%s: Invalid index.\n"), "getfield");
        return types::Function::Error;
    }

    if (_iRetCount != vectResult.size())
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "getfield", vectResult.size());
        return types::Function::Error;
    }

    for (int i = 0 ; i < _iRetCount ; i++)
    {
        out.push_back(vectResult[i]);
    }

    return types::Function::OK;
}
/*-----------------------------------------------------------------------------------*/
