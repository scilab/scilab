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
#include "XMLElemFieldsGetter.hxx"
#include "XMLAttrFieldsGetter.hxx"
#include "XMLNsFieldsGetter.hxx"
#include "XMLListFieldsGetter.hxx"


using namespace org_modules_xml;

namespace org_modules_completion
{

const char ** XMLElemFieldsGetter::getFieldsName(const std::string & typeName, int * mlist, char ** fieldPath, const int fieldPathLen, int * fieldsSize) const
{
    int id = getXMLObjectId(mlist, pvApiCtx);
    XMLElement * e = XMLObject::getFromId<XMLElement>(id);

    return getFieldsName(e, fieldPath + 1, fieldPathLen - 1, fieldsSize);
}

const char ** XMLElemFieldsGetter::getFieldsName(const XMLElement * e, char ** fieldPath, const int fieldPathLen, int * fieldsSize)
{
    if (!e)
    {
        return 0;
    }

    if (fieldPathLen == 0)
    {
        *fieldsSize = 7;
        const char * fieldsName[7] = {"name", "namespace", "content", "type", "parent", "attributes", "children"};
        return XMLFieldsGetter::copy(fieldsName, 7);
    }

    if (!strcmp(fieldPath[0], "namespace"))
    {
        return XMLNsFieldsGetter::getFieldsName(e->getNodeNameSpace(), fieldPath + 1, fieldPathLen - 1, fieldsSize);
    }

    if (!strcmp(fieldPath[0], "parent"))
    {
        return XMLElemFieldsGetter::getFieldsName(e->getParentElement(), fieldPath + 1, fieldPathLen - 1, fieldsSize);
    }

    if (!strcmp(fieldPath[0], "attributes"))
    {
        return XMLAttrFieldsGetter::getFieldsName(e->getAttributes(), fieldPath + 1, fieldPathLen - 1, fieldsSize);
    }

    if (!strcmp(fieldPath[0], "children"))
    {
        return XMLListFieldsGetter::getFieldsName(e->getChildren(), fieldPath + 1, fieldPathLen - 1, fieldsSize);
    }

    return 0;
}
}
