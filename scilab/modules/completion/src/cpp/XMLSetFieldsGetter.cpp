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
#include "XMLSetFieldsGetter.hxx"


using namespace org_modules_xml;

namespace org_modules_completion
{

const char ** XMLSetFieldsGetter::getFieldsName(const std::string & typeName, int * mlist, char ** fieldPath, const int fieldPathLen, int * fieldsSize) const
{
    int id = getXMLObjectId(mlist, pvApiCtx);
    XMLNodeSet * s = XMLObject::getFromId<XMLNodeSet>(id);

    return getFieldsName(s, fieldPath + 1, fieldPathLen - 1, fieldsSize);
}

const char ** XMLSetFieldsGetter::getFieldsName(const XMLNodeSet * s, char ** fieldPath, const int fieldPathLen, int * fieldsSize)
{
    if (!s)
    {
        return 0;
    }

    if (fieldPathLen == 0)
    {
        *fieldsSize = 1;
        const char * fieldsName[1] = {"size"};
        return copy(fieldsName, 1);
    }

    return 0;
}
}
