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

#ifndef __XMLATTR_HXX__
#define __XMLATTR_HXX__

#include <string>
#include "dynlib_xml_scilab.h"
#include "xml.h"

namespace org_modules_xml
{
class XMLElement;
class XMLObject;

class XML_SCILAB_IMPEXP XMLAttr : public XMLObject
{
    const XMLElement & elem;

public:
    /**
     * Default constructor
     * @param elem the element which has this attributes
     */
    XMLAttr(const XMLElement & elem);

    ~XMLAttr();

    void *getRealXMLPointer() const;

    /**
     * Get the number of attributes
     * @return the attributes number
     */
    int getSize() const;

    /**
     * Gets the attribute value with the given index.
     * @param index the attribute index
     * @return the attribute value
     */
    const char *getAttributeValue(int index) const;

    /**
     * Gets the attribute value.
     * @param name the attribute name
     * @return the attribute value
     */
    const char *getAttributeValue(const char *name) const;

    /**
     * Gets the attribute value with a prefix namespace.
     * @param prefix the namespace prefix or the namespace itself
     * @param name the attribute name
     * @return the attribute value
     */
    const char *getAttributeValue(const char *prefix, const char *name) const;

    /**
     * Sets the attribute value.
     * @param name the attribute name
     * @param value the attribute value
     */
    void setAttributeValue(const char *name, const char *value) const;

    /**
     * Sets the attribute value.
     * @param node the node where to set the attributes
     * @param name the attribute name
     * @param value the attribute value
     */
    static void setAttributeValue(xmlNode * node, const char *name, const char *value);

    /**
     * Sets the attribute value.
     * @param name the attribute names
     * @param value the attribute values
     * @param size the number of names
     */
    void setAttributeValue(const char **name, const char **value, int size) const;

    /**
     * Sets the attribute value.
     * @param node the node where to set the attributes
     * @param name the attribute names
     * @param value the attribute values
     * @param size the number of names
     */
    static void setAttributeValue(xmlNode * node, const char **name, const char **value, int size);

    /**
     * Sets the attribute value at the given index.
     * @param index the attribute index
     * @param value the attribute value
     */
    void setAttributeValue(int index, const char *value) const;

    /**
     * Sets the attribute value at the given index.
     * @param node the node where to set the attributes
     * @param index the attribute index
     * @param value the attribute value
     */
    static void setAttributeValue(xmlNode * node, int index, const char *value);

    /**
     * Sets the attribute value with a prefix namespace.
     * @param prefix the namespace prefix or the namespace itself
     * @param name the attribute name
     * @param value the attribute value
     */
    void setAttributeValue(const char *prefix, const char *name, const char *value) const;

    /**
     * Sets the attribute value with a prefix namespace.
     * @param node the node where to set the attributes
     * @param prefix the namespace prefix or the namespace itself
     * @param name the attribute name
     * @param value the attribute value
     */
    static void setAttributeValue(xmlNode * node, const char *prefix, const char *name, const char *value);

    /**
     * Sets the attribute value with a prefix namespace.
     * @param prefix the namespace prefix or the namespace itself
     * @param name the attribute names
     * @param value the attribute values
     * @param size the number of names
     */
    void setAttributeValue(const char **prefix, const char **name, const char **value, int size) const;

    /**
     * Sets the attribute value with a prefix namespace.
     * @param node the node where to set the attributes
     * @param prefix the namespace prefix or the namespace itself
     * @param name the attribute names
     * @param value the attribute values
     * @param size the number of names
     */
    static void setAttributeValue(xmlNode * node, const char **prefix, const char **name, const char **value, int size);

    /**
     * @return the names of the attributes
     */
    const char ** getNames() const;

    /**
     * Gets the element associated with this object
     * @return the associated object
     */
    const XMLElement & getElement() const
    {
        return elem;
    }

    const XMLObject *getXMLObjectParent() const;
    const std::string toString() const;

    /**
     * Get the number of attributes
     * @return the attributes number
     */
    static int getSize(xmlAttr * attr);
};
}

#endif
