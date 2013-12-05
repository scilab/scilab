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
#include "XMLAttrFieldsGetter.hxx"


using namespace org_modules_xml;

namespace org_modules_completion
{

const char ** XMLAttrFieldsGetter::getFieldsName(const std::string & typeName, int * mlist, char ** fieldPath, const int fieldPathLen, int * fieldsSize) const
{
    int id = getXMLObjectId(mlist, pvApiCtx);
    XMLAttr * a = XMLObject::getFromId<XMLAttr>(id);

    return getFieldsName(a, fieldPath + 1, fieldPathLen - 1, fieldsSize);
}

const char ** XMLAttrFieldsGetter::getFieldsName(const XMLAttr * a, char ** fieldPath, const int fieldPathLen, int * fieldsSize)
{
    if (!a)
    {
        return 0;
    }

    if (fieldPathLen == 0)
    {
        const char ** fieldsName = a->getNames();
        int size = 0;
        for (; fieldsName[size]; size++)
        {
            ;
        }

        const char ** cpy = copy(fieldsName, size);
        delete [] fieldsName;
        *fieldsSize = size;

        return cpy;
    }

    return 0;
}
}
