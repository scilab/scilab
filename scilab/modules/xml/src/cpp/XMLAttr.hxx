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
    class XMLElement;
    class XMLObject;

    class XMLAttr : public XMLObject
    {
	XMLElement * elem;

    public :
	XMLAttr(XMLElement * elem);
	~XMLAttr();

	const char * getAttributeValue(const char * name);
	const char * getAttributeValue(const char * prefix, const char * name);
	XMLObject * getXMLObjectParent();
	std::string * toString();
    };
}
