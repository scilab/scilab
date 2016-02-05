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
#include "XMLDocFieldsGetter.hxx"
#include "XMLElemFieldsGetter.hxx"


namespace org_modules_completion
{

const char ** XMLDocFieldsGetter::getFieldsName(const std::string & typeName, int * mlist, char ** fieldPath, const int fieldPathLen, int * fieldsSize) const
{
    int id = getXMLObjectId(mlist, NULL);
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
