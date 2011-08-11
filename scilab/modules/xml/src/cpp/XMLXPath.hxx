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

#include <string>

#include "xml.h"

namespace org_modules_xml
{
    class XMLDocument;
    class XMLObject;
    class XMLNodeSet;

    class XMLXPath : public XMLObject
    {
        XMLDocument * doc;
        xmlXPathObject * xpath;

    public:
        XMLXPath(XMLDocument * doc, xmlXPathObject * xpath);
        ~XMLXPath();

        int getBooleanValue(void) { return xpath->boolval; }
        double getFloatValue(void) { return xpath->floatval; }
        const char * getStringValue(void) { return (const char *)xpath->stringval; }
        int getResultType(void) { return xpath->type; }
        XMLNodeSet * getNodeSet(void);
        XMLObject * getXMLObjectParent();
    };
}
