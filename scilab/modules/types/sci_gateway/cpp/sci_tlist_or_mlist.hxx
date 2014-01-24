/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SCI_TLIST_OR_MLIST_HXX__
#define __SCI_TLIST_OR_MLIST_HXX__

#include "string.hxx"
#include "double.hxx"
#include "funcmanager.hxx"

extern "C"
{
#include "Scierror.h"
#include "MALLOC.h"
#include "localization.h"
#include "charEncoding.h"
}

using namespace types;

template <typename TorMList>
Function::ReturnValue sci_tlist_or_mlist(typed_list &in, int _piRetCount, typed_list &out, wchar_t *_pstrFunName)
{
    TorMList* pRetVal = NULL;

    //check input parameters
    if (in.size() < 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: At least %d expected.\n"), _pstrFunName , 1);
        return Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), _pstrFunName, 1);
        return Function::Error;
    }

    //check uniqueness of fields name
    String* pS = in[0]->getAs<types::String>();

    //check for rational type
    if(pS->getSize() > 0 && wcscmp(pS->get(0), L"r") == 0)
    {
        Scierror(999, _("%ls: Can not create a %ls with input argument #%d.\n"), _pstrFunName, _pstrFunName, 1);
        return Function::Error;
    }

    //first string is the tlist type
    list<wstring> fieldNames;
    for (int i = 1 ; i < pS->getSize() ; i++)
    {
        list<wstring>::iterator it;
        for (it = fieldNames.begin() ; it != fieldNames.end() ; it++)
        {
            if (*it == wstring(pS->get(i)))
            {
                char* pstFunName = wide_string_to_UTF8(_pstrFunName);
                Scierror(999, _("%s : Fields names must be unique"), pstFunName);
                FREE(pstFunName);
                return Function::Error;
            }
        }
        fieldNames.push_back(pS->get(i));
    }

    pRetVal = new TorMList();
    for (unsigned int i = 0 ; i < in.size() ; i++)
    {
        pRetVal->append(in[i]);
    }

    //fill empty field with []
    while (pRetVal->getSize() < pS->getSize())
    {
        pRetVal->append(Double::Empty());
    }

    out.push_back(pRetVal);
    return Function::OK;
}

#endif /* !__SCI_TLIST_OR_MLIST_HXX__ */
