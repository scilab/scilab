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

#include <cstring>

extern "C"
{
#include "api_scilab.h"
}

#include "FieldsManager.hxx"
#include "XMLDocFieldsGetter.hxx"
#include "XMLElemFieldsGetter.hxx"


namespace org_modules_completion
{

const char ** XMLDocFieldsGetter::getFieldsName(const std::string & typeName, int * mlist, char ** fieldPath, const int fieldPathLen, int * fieldsSize) const
{
    int id = getXMLObjectId(mlist, pvApiCtx);
    org_modules_xml::XMLDocument * doc = XMLObject::getFromId<org_modules_xml::XMLDocument>(id);

    return getFieldsName(doc, fieldPath + 1, fieldPathLen - 1, fieldsSize);
}

const char ** XMLDocFieldsGetter::getFieldsName(const org_modules_xml::XMLDocument * doc, char ** fieldPath, const int fieldPathLen, int * fieldsSize)
{
    if (!doc)
    {
        return 0;
    }

    if (fieldPathLen == 0)
    {
        *fieldsSize = 2;
        const char * fieldsName[2] = {"root", "url"};
        return XMLFieldsGetter::copy(fieldsName, 2);
    }

    if (!strcmp(fieldPath[0], "root"))
    {
        return XMLElemFieldsGetter::getFieldsName(doc->getRoot(), fieldPath + 1, fieldPathLen - 1, fieldsSize);
    }

    return 0;
}
}
