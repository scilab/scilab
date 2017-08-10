/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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

#ifndef __SCI_TLIST_OR_MLIST_HXX__
#define __SCI_TLIST_OR_MLIST_HXX__

#include "string.hxx"
#include "double.hxx"
#include "funcmanager.hxx"

extern "C"
{
#include "Scierror.h"
#include "sci_malloc.h"
#include "localization.h"
#include "charEncoding.h"
}

template <typename TorMList>
types::Function::ReturnValue sci_tlist_or_mlist(types::typed_list &in, int _piRetCount, types::typed_list &out, const wchar_t *_pstrFunName)
{
    TorMList* pRetVal = NULL;

    char* pstrFunName = wide_string_to_UTF8(_pstrFunName);

    //check input parameters
    if (in.size() < 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: At least %d expected.\n"), pstrFunName , 1);
        FREE(pstrFunName);
        return types::Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), pstrFunName, 1);
        FREE(pstrFunName);
        return types::Function::Error;
    }

    //check uniqueness of fields name
    types::String* pS = in[0]->getAs<types::String>();

    //check for rational type
    if (pS->getSize() > 0 && wcscmp(pS->get(0), L"r") == 0)
    {
        Scierror(999, _("%s: Can not create a %s with input argument #%d.\n"), pstrFunName, pstrFunName, 1);
        FREE(pstrFunName);
        return types::Function::Error;
    }

    FREE(pstrFunName);

    pRetVal = new TorMList();
    for (unsigned int i = 0 ; i < in.size() ; i++)
    {
        pRetVal->append(in[i]);
    }

    out.push_back(pRetVal);
    return types::Function::OK;
}

#endif /* !__SCI_TLIST_OR_MLIST_HXX__ */
