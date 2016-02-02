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
    int id = getXMLObjectId(mlist, NULL);
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
