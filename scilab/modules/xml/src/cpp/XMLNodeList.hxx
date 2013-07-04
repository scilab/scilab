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

#ifndef __XMLNODELIST_HXX__
#define __XMLNODELIST_HXX__

#include "XMLList.hxx"
#include "xml.h"

namespace org_modules_xml
{
class XMLElement;
class XMLDocument;

/**
 * @file
 * @author Calixte DENIZET <calixte.denizet@scilab.org>
 *
 * Class to wrap the list of the children of a node
 */
class XMLNodeList: public XMLList
{

    const XMLDocument & doc;
    xmlNode *parent;
    int prev;
    xmlNode *prevNode;

public:
    /**
     * Default constructor
     * @param doc the XMLDocument where this nodelist is
     * @param parent the parent node
     */
    XMLNodeList(const XMLDocument & doc, xmlNode * parent);

    ~XMLNodeList();

    void *getRealXMLPointer() const;

    const char **getContentFromList() const;

    const char **getNameFromList() const;

    void remove() const;

    /**
     * Removes an element
     * @param index the index of the element to remove
     */
    void removeElementAtPosition(int index);

    /**
     * @anchor setElementAtPosition
     * Sets an element at a given index.
     * If the index is an integer, then according to the index value the element
     * will be inserted at tail, or will replaced the existing one.
     * If the index is not an integer, then the element will be inserted between the
     * elements with indexes floor(index) and floor(index)+1.
     * @param index the index
     * @param elem the element to insert
     */
    void setElementAtPosition(double index, const XMLElement & elem);

    /**
     * See @ref setElementAtPosition
     * @param index the index
     * @param document the document containing the root which will be inserted
     */
    void setElementAtPosition(double index, const XMLDocument & document);

    /**
     * See @ref setElementAtPosition
     * @param index the index
     * @param xmlCode the XML code for the element which will be inserted
     */
    void setElementAtPosition(double index, const std::string & xmlCode);

    /**
     * See @ref setElementAtPosition
     * @param index the index
     * @param list the list of the elements which will be inserted
     */
    void setElementAtPosition(double index, const XMLNodeList & list);

    /**
     * Gets the parent node behind this children list.
     * @return the parent node
     */
    xmlNode *getRealNode() const
    {
        return parent->children;
    }

    void setAttributeValue(const char **prefix, const char **name, const char **value, int lsize) const;
    void setAttributeValue(const char **name, const char **value, int lsize) const;
    const XMLObject *getXMLObjectParent() const;
    const std::string dump() const;
    const XMLObject *getListElement(int index);

    /**
     * Gets the node list size
     * @param node the xmlNode
     * @return the size
     */
    static int getNodeListSize(xmlNode * node);
protected:
    /**
     * Replaces an element at a given index
     * @param index the index
     * @param elem the new element
     */
    void replaceAtIndex(int index, const XMLElement & elem);

    /**
     * Inserts an element at beginning
     * @param elem the element to insert
     */
    void insertAtEnd(const XMLElement & elem);

    /**
     * Inserts an element at end
     * @param elem the element to insert
     */
    void insertAtBeginning(const XMLElement & elem);

    /**
     * Inserts an element at a given index
     * @param index the index
     * @param elem the element to insert
     */
    void insertAtIndex(int index, const XMLElement & elem);

private:
    /**
     * Gets a node at a given index
     * @param index the index
     * @return the correspionding node
     */
    xmlNode *getListNode(int index);

};
}

#endif
