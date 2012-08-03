/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <cstring>
#include <cstdio>

#include "XMLObject.hxx"
#include "XMLAttr.hxx"

#include "FieldsManager.hxx"
#include "XMLFieldsGetter.hxx"

extern "C"
{
#include "api_scilab.h"
#include "MALLOC.h"
#include "xml_mlist.h"
}

using namespace org_modules_xml;

namespace org_modules_completion
{

    const char ** XMLFieldsGetter::getFieldsName(const std::string & typeName, int * mlist) const
    {
        if (typeName == std::string("XMLDoc"))
        {
            const char * fieldsName[3] = {"root", "url", 0};
            return copy("XMLDoc", fieldsName);
        }

        if (typeName == std::string("XMLElem"))
        {
            const char * fieldsName[8] = {"name", "namespace", "content", "type", "parent", "attributes", "children"};
            return copy("XMLElem", fieldsName);
        }

        if (typeName == std::string("XMLNs"))
        {
            const char * fieldsName[3] = {"href", "prefix", 0};
            return copy("XMLNs", fieldsName);
        }

        if (typeName == std::string("XMLAttr"))
        {
            int id = getXMLObjectId(mlist, pvApiCtx);
            XMLAttr * attr = XMLObject::getFromId<XMLAttr>(id);
            const char ** fieldsName = attr->getNames();
            const char ** cpy = copy("XMLAttr", fieldsName);
            delete [] fieldsName;

            return cpy;
        }

        if (typeName == std::string("XMLList"))
        {
            const char * fieldsName[2] = {"size", 0};
            return copy("XMLList", fieldsName);
        }

        if (typeName == std::string("XMLSet"))
        {
            const char * fieldsName[2] = {"size", 0};
            return copy("XMLList", fieldsName);
        }

        return 0;
    }
}
