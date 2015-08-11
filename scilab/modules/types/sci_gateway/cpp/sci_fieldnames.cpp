/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "types_gw.hxx"
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

using namespace types;

Function::ReturnValue sci_fieldnames(typed_list &in, int _iRetCount, typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "fieldnames", 1);
        return Function::Error;
    }


    // FIXME : iso-functionnal to Scilab < 6
    // Works on other types except userType, {m,t}list and struct
    if (in[0]->isStruct() == false && in[0]->isMList() == false && in[0]->isTList() == false && in[0]->isUserType() == false)
    {
        out.push_back(Double::Empty());
        return Function::OK;
    }

    // STRUCT
    if (in[0]->isStruct() == true)
    {
        String* pFields = in[0]->getAs<Struct>()->getFieldNames();
        if (pFields)
        {
            out.push_back(pFields);
            //delete pFields;
        }
        else
        {
            out.push_back(Double::Empty());
        }
        return Function::OK;
    }

    InternalType* pIT;

    // TLIST or MLIST
    if (in[0]->isList() == true)
    {
        // We only need list capabilities to retrieve first argument as List.
        List *pInList = in[0]->getAs<List>();
        pIT = pInList->get(0);

        if (pIT == nullptr || pIT->isString() == false)
        {
            // FIXME : iso-functionnal to Scilab < 6
            // Works on other types except userType, {m,t}list and struct
            out.push_back(Double::Empty());
            return Function::OK;
        }
    }

    // USER-TYPE (typically a Xcos object)
    if (in[0]->isUserType() == true)
    {
        // We only need userType capabilities to retrieve first argument as UserType.
        UserType *pInUser = in[0]->getAs<UserType>();

        // Extract "diagram" first then the properties, or all in one shot?? Should DiagramA implement such a routine or should fieldnames include DiagramA?
        //std::cout<<"diag "<<pInUser->getSharedTypeStr()<<std::endl;

        // Extract the properties
        typed_list one (1, new types::Double(1));
        InternalType* pProperties = pInUser->extract(&one);
        if (pProperties == nullptr || pProperties->isString() == false)
        {
            // FIXME : iso-functionnal to Scilab < 6
            // Works on other types except userType, {m,t}list and struct
            out.push_back(Double::Empty());
            return Function::OK;
        }
    }

    String *pAllFields = pIT->getAs<String>();
    wchar_t **pwcsAllStrings =  pAllFields->get();
    // shift to forget first value corresponding to type.
    //    ++pwcsAllStrings;


    String *pNewString = new String(pAllFields->getSize() - 1, 1, pwcsAllStrings + 1);

    out.push_back(pNewString);

    return Function::OK;
}
