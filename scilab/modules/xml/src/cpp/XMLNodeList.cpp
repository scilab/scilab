/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012-2014 - Scilab Enterprises - Calixte DENIZET
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

#include <string>

#include "XMLObject.hxx"
#include "XMLDocument.hxx"
#include "XMLAttr.hxx"
#include "XMLElement.hxx"
#include "XMLNodeList.hxx"
#include "VariableScope.hxx"

namespace org_modules_xml
{

XMLNodeList::XMLNodeList(const XMLDocument & _doc, xmlNode * _parent): XMLList(), doc(_doc)
{
    parent = _parent;
    size = getNodeListSize(parent->children);
    prev = 1;
    prevNode = parent->children;
    scope->registerPointers(parent->children, this);
    id = scope->getVariableId(*this);
}

XMLNodeList::~XMLNodeList()
{
    scope->unregisterNodeListPointer(parent->children);
    scope->removeId(id);
}

void *XMLNodeList::getRealXMLPointer() const
{
    return static_cast < void *>(parent->children);
}

const char **XMLNodeList::getContentFromList() const
{
    const char **list = new const char *[size];
    int i = 0;
    for (xmlNode * cur = parent->children; cur; cur = cur->next, i++)
    {
        list[i] = (const char *)xmlNodeGetContent(cur);
    }

    return list;
}

const char **XMLNodeList::getNameFromList() const
{
    const char **list = new const char *[size];
    int i = 0;
    for (xmlNode * cur = parent->children; cur; cur = cur->next, i++)
    {
        list[i] = cur->name ? (const char *)cur->name : "";
    }

    return list;
}

void XMLNodeList::setAttributeValue(const char **prefix, const char **name, const char **value, int lsize) const
{
    for (xmlNode * cur = parent->children; cur; cur = cur->next)
    {
        XMLAttr::setAttributeValue(cur, prefix, name, value, lsize);
    }
}

void XMLNodeList::setAttributeValue(const char **name, const char **value, int lsize) const
{
    for (xmlNode * cur = parent->children; cur; cur = cur->next)
    {
        XMLAttr::setAttributeValue(cur, name, value, lsize);
    }
}

void XMLNodeList::remove() const
{
    xmlNode *cur = parent->children;

    while (cur != NULL)
    {
        xmlNode *nxt = cur->next;
        xmlUnlinkNode(cur);
        xmlFreeNode(cur);
        cur = nxt;
    }
}

const XMLObject *XMLNodeList::getXMLObjectParent() const
{
    return &doc;
}

const std::string XMLNodeList::dump() const
{
    xmlBufferPtr buffer = xmlBufferCreate();
    for (xmlNode * cur = parent->children; cur; cur = cur->next)
    {
        xmlNodeDump(buffer, doc.getRealDocument(), cur, 0, 1);
        xmlBufferAdd(buffer, (xmlChar *) "\n", (int)strlen("\n"));
    }
    std::string str = std::string((const char *)buffer->content);

    xmlBufferFree(buffer);

    return str;
}

const XMLObject *XMLNodeList::getListElement(int index)
{
    xmlNode *n = getListNode(index);

    if (n)
    {
        XMLObject *obj = scope->getXMLObjectFromLibXMLPtr(n);

        if (obj)
        {
            return static_cast < XMLElement * >(obj);
        }

        return new XMLElement(doc, n);
    }

    return 0;
}

void XMLNodeList::removeElementAtPosition(int index)
{
    if (size && index >= 1 && index <= size)
    {
        if (index == 1)
        {
            xmlNode *n = parent->children;

            scope->unregisterNodeListPointer(n);
            xmlUnlinkNode(n);
            xmlFreeNode(n);
            size--;
            if (size == 0)
            {
                parent->children = 0;
            }
            prevNode = parent->children;
            scope->registerPointers(parent->children, this);
            prev = 1;
        }
        else
        {
            xmlNode *n = getListNode(index);

            if (n)
            {
                xmlNode *next = n->next;

                prevNode = prevNode->prev;
                prev--;
                xmlUnlinkNode(n);
                xmlFreeNode(n);
                prevNode->next = next;
                size--;
            }
        }
    }
}

void XMLNodeList::setElementAtPosition(double index, const XMLElement & elem)
{
    if (size == 0)
    {
        insertAtEnd(elem);
        prevNode = parent->children;
        prev = 1;
    }
    else if (index < 1)
    {
        insertAtBeginning(elem);
    }
    else if (index > size)
    {
        insertAtEnd(elem);
    }
    else if ((int)index == index)
    {
        replaceAtIndex((int)index, elem);
    }
    else
    {
        insertAtIndex((int)index, elem);
    }
}

void XMLNodeList::setElementAtPosition(double index, const XMLDocument & document)
{
    const XMLElement *e = document.getRoot();

    setElementAtPosition(index, *e);
    delete e;
}

void XMLNodeList::setElementAtPosition(double index, const std::string & xmlCode)
{
    std::string error;
    XMLDocument document = XMLDocument(xmlCode, false, &error);

    if (error.empty())
    {
        setElementAtPosition(index, document);
    }
    else
    {
        xmlNode *n = xmlNewText((xmlChar *) xmlCode.c_str());

        setElementAtPosition(index, XMLElement(doc, n));
    }
}

void XMLNodeList::setElementAtPosition(double index, const XMLNodeList & list)
{
    if (list.getSize() && list.getRealNode() != parent)
    {
        xmlNode * node = 0;
        xmlNode * snode = 0;
        int pos = (int)index;

        if (index < 1)
        {
            pos = 1;
        }
        else if (index > size)
        {
            pos = size + 1;
        }
        else if ((int)index != index)
        {
            pos++;
        }

        if (&list == this)
        {
            snode = node = xmlCopyNode(list.getRealNode(), 1);
            for (xmlNode * cur = list.getRealNode()->next; cur; cur = cur->next)
            {
                node->next = xmlCopyNode(cur, 1);
                node = node->next;
            }
            node = snode;
        }
        else
        {
            node = list.getRealNode();
        }

        setElementAtPosition(index, XMLElement(doc, node));
        for (xmlNode * cur = node->next; cur; cur = cur->next)
        {
            setElementAtPosition((double)(pos++) + 0.5, XMLElement(doc, cur));
        }
    }
}

void XMLNodeList::replaceAtIndex(int index, const XMLElement & elem)
{
    xmlNode *n = getListNode(index);

    if (n && n != elem.getRealNode())
    {
        if (index == 1)
        {
            scope->unregisterNodeListPointer(parent->children);
        }
        xmlNode *previous = n->prev;
        xmlNode *next = n->next;
        xmlNode *cpy = xmlCopyNode(elem.getRealNode(), 1);
        xmlUnlinkNode(cpy);
        xmlReplaceNode(n, cpy);
        xmlFreeNode(n);
        prevNode = cpy;
        cpy->prev = previous;
        cpy->next = next;
        if (index == 1)
        {
            scope->registerPointers(parent->children, this);
        }
    }
}

void XMLNodeList::insertAtEnd(const XMLElement & elem)
{
    xmlNode *cpy = xmlCopyNode(elem.getRealNode(), 1);

    xmlUnlinkNode(cpy);
    xmlAddChild(parent, cpy);
    size++;
}

void XMLNodeList::insertAtBeginning(const XMLElement & elem)
{
    xmlNode *cpy = xmlCopyNode(elem.getRealNode(), 1);

    xmlUnlinkNode(cpy);
    scope->unregisterNodeListPointer(parent->children);
    xmlAddPrevSibling(parent->children, cpy);
    scope->registerPointers(parent->children, this);
    size++;
}

void XMLNodeList::insertAtIndex(int index, const XMLElement & elem)
{
    xmlNode *n = getListNode(index);

    if (n)
    {
        xmlNode *cpy = xmlCopyNode(elem.getRealNode(), 1);

        xmlUnlinkNode(cpy);
        xmlAddNextSibling(n, cpy);
        size++;
    }
}

xmlNode *XMLNodeList::getListNode(int index)
{
    return XMLList::getListElement < xmlNode > (index, size, &prev, &prevNode);
}

void XMLNodeList::revalidateSize()
{
    size = getNodeListSize(parent->children);
    prevNode = parent->children;
    prev = 1;
}

int XMLNodeList::getNodeListSize(xmlNode * node)
{
    int i = 0;

    for (xmlNode * n = node; n; n = n->next, i++)
    {
        ;
    }

    return i;
}
}
