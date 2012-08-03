/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __XMLNS_HXX__
#define __XMLNS_HXX__

#include <string>

#include "xml.h"

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
    class XMLNs:public XMLObject
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
