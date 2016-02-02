/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
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
    int id = getXMLObjectId(mlist, NULL);
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
