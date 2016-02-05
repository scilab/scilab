/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

#ifndef __XMLXPATH_HXX__
#define __XMLXPATH_HXX__

#include "xml.h"

namespace org_modules_xml
{
class XMLDocument;
class XMLObject;
class XMLNodeSet;

class XMLXPath: public XMLObject
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
