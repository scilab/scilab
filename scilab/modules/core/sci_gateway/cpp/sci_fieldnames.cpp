/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
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

#include "core_gw.hxx"
#include "function.hxx"
#include "struct.hxx"
#include "double.hxx"
#include "string.hxx"
#include "list.hxx"
#include "user.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
}

types::Function::ReturnValue sci_fieldnames(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "fieldnames", 1);
        return types::Function::Error;
    }


    // FIXME : iso-functionnal to Scilab < 6
    // Works on other types except userType, {m,t}list and struct
    if (in[0]->isStruct() == false && in[0]->isMList() == false && in[0]->isTList() == false && in[0]->isUserType() == false)
    {
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }

    // STRUCT
    if (in[0]->isStruct() == true)
    {
        types::String* pFields = in[0]->getAs<types::Struct>()->getFieldNames();
        if (pFields)
        {
            out.push_back(pFields);
            //delete pFields;
        }
        else
        {
            out.push_back(types::Double::Empty());
        }
        return types::Function::OK;
    }

    types::InternalType* pIT = nullptr;

    // TLIST or MLIST
    if (in[0]->isList() == true)
    {
        // We only need list capabilities to retrieve first argument as List.
        types::List *pInList = in[0]->getAs<types::List>();
        pIT = pInList->get(0);

        if (pIT == nullptr || pIT->isString() == false)
        {
            // FIXME : iso-functionnal to Scilab < 6
            // Works on other types except userType, {m,t}list and struct
            out.push_back(types::Double::Empty());
            return types::Function::OK;
        }
    }

    // USER-TYPE (typically an Xcos object)
    if (in[0]->isUserType() == true)
    {
        // We only need userType capabilities to retrieve first argument as UserType.
        types::UserType *pInUser = in[0]->getAs<types::UserType>();

        // Extract the sub-type
        std::wstring subType (pInUser->getShortTypeStr());

        // Extract the properties
        types::typed_list one(1, new types::Double(1));
        types::InternalType* pProperties = pInUser->extract(&one);
        if (pProperties == nullptr || pProperties->isString() == false)
        {
            // FIXME : iso-functionnal to Scilab < 6
            // Works on other types except userType, {m,t}list and struct
            out.push_back(types::Double::Empty());
            return types::Function::OK;
        }
        int nProp = ((types::String*) pProperties)->getSize();

        pIT = new types::String(nProp + 1, 1);
        ((types::String*) pIT)->set(0, subType.data());
        for (int i = 0; i < nProp; ++i)
        {
            ((types::String*) pIT)->set(i + 1, ((types::String*)pProperties)->get(i));
        }
    }

    types::String *pAllFields;
    if (pIT)
    {
        pAllFields = pIT->getAs<types::String>();
    }
    else
    {
        Scierror(999, _("Could not retrieve sub-type.\n"));
        return types::Function::Error;
    }
    wchar_t **pwcsAllStrings =  pAllFields->get();
    // shift to forget first value corresponding to type.
    //    ++pwcsAllStrings;


    types::String *pNewString = new types::String(pAllFields->getSize() - 1, 1, pwcsAllStrings + 1);

    out.push_back(pNewString);

    return types::Function::OK;
}
