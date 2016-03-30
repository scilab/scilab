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

#ifndef __XMLNS_HXX__
#define __XMLNS_HXX__

#include <string>

#include "xml.h"

#include "dynlib_xml_scilab.h"

namespace org_modules_xml
{

class XMLElement;
class XMLObject;

/**
 * @file
 * @author Calixte DENIZET <calixte.denizet@scilab.org>
 *
 * Class to wrap a namespace xmlNs.
 * @see http://xmlsoft.org/html/libxml-tree.html#xmlNs
 */
class XML_SCILAB_IMPEXP XMLNs: public XMLObject
{
    const XMLObject & parent;
    xmlNs *ns;

public:

    /**
     * @param parent the parent XML object
     * @param ns the xml namespace
     */
    XMLNs(const XMLObject & parent, xmlNs * ns);

    /**
     * @param elem the parent XMLElement
     * @param prefix the namespace prefix
     * @param href the namespace href
     */
    XMLNs(const XMLElement & elem, char *prefix, char *href);

    ~XMLNs();

    void *getRealXMLPointer() const;

    /**
     * @return the namespace href
     */
    const char *getHref() const
    {
        return ns == 0 ? "" : (const char *)ns->href;
    }

    /**
     * @return the namespace prefix
     */
    const char *getPrefix() const
    {
        return ns == 0 ? "" : (const char *)ns->prefix;
    }

    /**
     * @return the xmlNs behind this object
     */
    xmlNs *getRealNs() const
    {
        return ns;
    }

    const XMLObject *getXMLObjectParent() const;
    const std::string toString() const;
};
}

#endif
