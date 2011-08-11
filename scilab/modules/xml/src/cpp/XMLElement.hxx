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
    class XMLObject;
    class XMLDocument;
    class XMLNodeList;
    class XMLNs;
    class XMLAttr;

    class XMLElement : public XMLObject
    {
        xmlNode * node;
        XMLDocument * doc;

    public:
        XMLElement(XMLDocument * doc, xmlNode * node);
        ~XMLElement();

        const char * getNodeName(void) { return (const char *)node->name; }
        XMLNs * getNodeNameSpace(void);
        const char * getNodeContent(void);
        int getNodeType(void) { return node->type; }
        XMLAttr * getAttributes(void);
        XMLElement * getParentElement(void);
        XMLNodeList * getChildren(void);
        xmlNode * getRealNode() { return node; }
        XMLDocument * getXMLDocument() { return doc; }
	XMLObject * getXMLObjectParent();
	std::string * dump();
        std::string * toString();
    };
}
