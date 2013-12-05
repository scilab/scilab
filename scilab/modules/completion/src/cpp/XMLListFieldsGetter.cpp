/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

extern "C"
{
#include "api_scilab.h"
}

#include "FieldsManager.hxx"
#include "XMLListFieldsGetter.hxx"


using namespace org_modules_xml;

namespace org_modules_completion
{

const char ** XMLListFieldsGetter::getFieldsName(const std::string & typeName, int * mlist, char ** fieldPath, const int fieldPathLen, int * fieldsSize) const
{
    int id = getXMLObjectId(mlist, pvApiCtx);
    XMLList * l = XMLObject::getFromId<XMLList>(id);

    return getFieldsName(l, fieldPath + 1, fieldPathLen - 1, fieldsSize);
}

const char ** XMLListFieldsGetter::getFieldsName(const XMLList * l, char ** fieldPath, const int fieldPathLen, int * fieldsSize)
{
    if (!l)
    {
        return 0;
    }

    if (fieldPathLen == 0)
    {
        *fieldsSize = 1;
        const char * fieldsName[1] = {"size"};
        return XMLFieldsGetter::copy(fieldsName, 1);
    }

    return 0;
}
}
