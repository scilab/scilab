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

#ifndef __XMLELEMENT_HXX__
#define __XMLELEMENT_HXX__

#include <string>
#include "dynlib_xml_scilab.h"
#include "XMLRemovable.hxx"

#include "xml.h"

namespace org_modules_xml
{
class XMLObject;
class XMLDocument;
class XMLNodeList;
class XMLNs;
class XMLAttr;

/**
 * @file
 * @author Calixte DENIZET <calixte.denizet@scilab.org>
 *
 * Class to wrap a xmlNode
 * @see http://xmlsoft.org/html/libxml-tree.html#xmlNode
 */
class XML_SCILAB_IMPEXP XMLElement: public XMLObject, public XMLRemovable
{
    bool allocated;
    xmlNode *node;
    const XMLDocument & doc;

public:

    /**
     * @param doc the owner document of this XMLElement
     * @param node the xmlNode to wrap
     */
    XMLElement(const XMLDocument & doc, xmlNode * node);

    /**
     * @param doc the owner document of this XMLElement
     * @param name the name of the XMLElement
     */
    XMLElement(const XMLDocument & doc, const char *name);

    ~XMLElement();

    void *getRealXMLPointer() const;

    void remove() const;

    /**
     * @return the node name
     */
    const char *getNodeName(void) const
    {
        return node->name ? (const char *)node->name : "";
    }

    /**
     * Sets the node name
     * @param name the node name
     */
    void setNodeName(const std::string & name) const;

    /**
     * @return the namespace associated to this node
     */
    const XMLNs *getNodeNameSpace() const;

    /**
     * Sets the namespace of this node
     * @param ns the namespace
     */
    void setNodeNameSpace(const XMLNs & ns) const;

    /**
     * Gets the node contents
     * @see http://xmlsoft.org/html/libxml-tree.html#xmlNodeGetContent
     * @return the node content
     */
    const char *getNodeContent() const;

    /**
     * Sets the node content
     * @param content the new node contents
     */
    void setNodeContent(const std::string & content) const;

    /**
     * @return the node type
     * @see http://xmlsoft.org/html/libxml-tree.html#xmlElementType
     */
    int getNodeType(void) const
    {
        return node->type;
    }

    /**
     * @return the attributes of this node
     */
    const XMLAttr *getAttributes(void) const;

    /**
     * Sets the attributes of this node
     * @param attrs the new attributes
     */
    void setAttributes(const XMLAttr & attrs) const;

    /**
     * @return the parent XMLElement
     */
    const XMLElement *getParentElement() const;

    /**
     * @return a list of the children of this node
     */
    const XMLNodeList *getChildren() const;

    /**
     * Replaces the children of this node by an XMLElement
     * @param elem the new child
     */
    void setChildren(const XMLElement & elem) const;

    /**
     * Replaces the children of this node by a list of nodes
     * @param list the new children
     */
    void setChildren(const XMLNodeList & list) const;

    /**
     * Replaces the children of this node by a new one given by a XML code
     * @param xmlCode the XML code of the new child
     */
    void setChildren(const std::string & xmlCode) const;

    /**
     * Adds a namespace to this node which can be used by the children
     * @param ns the namespace to add
     */
    void addNamespace(const XMLNs & ns) const;

    /**
     * Gets the namespace which has a given prefix. If it is not found in this
     * node, then it will be searched in the parents.
     * @see http://xmlsoft.org/html/libxml-tree.html#xmlSearchNs
     * @param prefix the prefix
     * @return the corresponding namespace or 0 if not found
     */
    const XMLNs *getNamespaceByPrefix(const char *prefix) const;

    /**
     * Gets the namespace which has a given href. If it is not found in this
     * node, then it will be searched in the parents.
     * @see http://xmlsoft.org/html/libxml-tree.html#xmlSearchNsByHref
     * @param href the href
     * @return the corresponding namespace or 0 if not found
     */
    const XMLNs *getNamespaceByHref(const char *href) const;

    /**
     * @return the libxml node behind this object
     */
    xmlNode *getRealNode() const
    {
        return node;
    }

    /**
     * @return the XMLDocument which is the parent or this XMLElement
     */
    const XMLDocument & getXMLDocument() const
    {
        return doc;
    }

    /**
     * @return the defintion line of this XMLElement
     */
    int getDefinitionLine() const;

    void setAttributeValue(const char **prefix, const char **name, const char **value, int size) const;
    void setAttributeValue(const char **name, const char **value, int size) const;
    void append(const XMLElement & elem) const;
    const XMLObject *getXMLObjectParent() const;
    const std::string dump(bool indent) const;
    const std::string toString() const;
};
}

#endif
