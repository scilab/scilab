/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/

#include "boolean_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "bool.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_bool2s(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::InternalType* pOut = NULL;
    if(in.size() != 1)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"bool2s", 1);
        return types::Function::Error;
    }

    if(in[0]->isDouble())
    {
        types::Double* pIn = in[0]->getAs<types::Double>();

        types::Double* pD = new types::Double(pIn->getDims(), pIn->getDimsArray());
        for(int i = 0 ; i < pIn->getSize() ; i++)
        {
            pD->set(i, pIn->get(i) != 0);
        }
        pOut = pD;
    }
    else if(in[0]->isBool())
    {
        types::Bool* pIn = in[0]->getAs<types::Bool>();

        types::Double* pD = new types::Double(pIn->getDims(), pIn->getDimsArray());
        for(int i = 0 ; i < pIn->getSize() ; i++)
        {
            pD->set(i, pIn->get(i));
        }
        pOut = pD;
    }
    //else if(in[0]->isSparse())
    //{
    //}
    //else if(in[0]->isBooleanSparse())
    //{
    //}
    else
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: %s or %s expected.\n"), L"bool2s", 1, L"bool", L"double");
        return types::Function::Error;
    }

    out.push_back(pOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
