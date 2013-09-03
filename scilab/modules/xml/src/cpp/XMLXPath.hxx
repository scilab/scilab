/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __XMLXPATH_HXX__
#define __XMLXPATH_HXX__

#include "xml.h"

namespace org_modules_xml
{
    class XMLDocument;
    class XMLObject;
    class XMLNodeSet;

    class XMLXPath:public XMLObject
    {
        const XMLDocument & doc;
        xmlXPathObject *xpath;

public:
          XMLXPath(const XMLDocument & doc, xmlXPathObject * xpath);
         ~XMLXPath();

        void *getRealXMLPointer() const;

        int getBooleanValue() const
        {
            return xpath->boolval;
        }
        double getFloatValue() const
        {
            return xpath->floatval;
        }
        const char *getStringValue() const
        {
            return (const char *)xpath->stringval;
        }
        int getResultType() const
        {
            return xpath->type;
        }
        const XMLNodeSet *getNodeSet() const;
        const XMLObject *getXMLObjectParent() const;

    };
}

#endif
