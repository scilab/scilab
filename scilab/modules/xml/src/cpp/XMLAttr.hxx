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

#ifndef __XMLATTR_HXX__
#define __XMLATTR_HXX__

#include <string>

#include "xml.h"

namespace org_modules_xml
{
    class XMLElement;
    class XMLObject;

    class XMLAttr : public XMLObject
    {
        const XMLElement & elem;

    public :

        /**
         * Default constructor
         * @param elem the element which has this attributes
         */
        XMLAttr(const XMLElement & elem);

        ~XMLAttr();

        void * getRealXMLPointer() const;

        /**
         * Get the number of attributes
         * @return the attributes number
         */
        int getSize() const;

        /**
         * Gets the attribute value.
         * @param name the attribute name
         * @return the attribute value
         */
        const char * getAttributeValue(const char * name) const;

        /**
         * Gets the attribute value with a prefix namespace.
         * @param prefix the namespace prefix or the namespace itself
         * @param name the attribute name
         * @return the attribute value
         */
        const char * getAttributeValue(const char * prefix, const char * name) const;

        /**
         * Sets the attribute value.
         * @param name the attribute name
         * @param value the attribute value
         */
        void setAttributeValue(const char * name, const char * value) const;

        /**
         * Sets the attribute value with a prefix namespace.
         * @param prefix the namespace prefix or the namespace itself
         * @param name the attribute name
         * @param value the attribute value
         */
        void setAttributeValue(const char * prefix, const char * name, const char * value) const;

        /**
         * Gets the element associated with this object
         * @return the associated object
         */
        const XMLElement & getElement() const { return elem; }

        const XMLObject * getXMLObjectParent() const;
        const std::string toString() const;
    };
}

#endif
