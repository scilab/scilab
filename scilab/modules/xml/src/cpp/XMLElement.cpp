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

#include "XMLObject.hxx"
#include "XMLElement.hxx"
#include "XMLDocument.hxx"
#include "XMLNodeList.hxx"
#include "XMLNs.hxx"
#include "XMLAttr.hxx"
#include "VariableScope.hxx"

extern "C"
{
    extern const char *nodes_type[];
}

namespace org_modules_xml
{

XMLElement::XMLElement(const XMLDocument & _doc, xmlNode * _node): XMLObject(), allocated(false), doc(_doc)
{
    node = _node;
    scope->registerPointers(node, this);
    scilabType = XMLELEMENT;
    id = scope->getVariableId(*this);
}

XMLElement::XMLElement(const XMLDocument & _doc, const char *name): XMLObject(), allocated(true), doc(_doc)
{
    node = xmlNewNode(0, (const xmlChar *)name);
    scope->registerPointers(node, this);
    scilabType = XMLELEMENT;
    id = scope->getVariableId(*this);
}

XMLElement::~XMLElement()
{
    scope->unregisterPointer(node);
    scope->removeId(id);

    if (allocated)
    {
        xmlFreeNode(node);
    }
}

void *XMLElement::getRealXMLPointer() const
{
    return static_cast < void *>(node);
}

void XMLElement::remove() const
{
    XMLNodeList *obj = 0;

    if (node->parent && node->parent->children)
    {
        obj = scope->getXMLNodeListFromLibXMLPtr(node->parent->children);
        if (obj && node->parent->children == node)
        {
            // node->parent->children == node => we remove the first child so parent->children
            // needs to be correctly re-linked.
            obj->removeElementAtPosition(1);
            return;
        }
    }

    xmlUnlinkNode(node);
    xmlFreeNode(node);

    if (obj)
    {
        obj->revalidateSize();
    }
}

const XMLObject *XMLElement::getXMLObjectParent() const
{
    return &doc;
}

const char *XMLElement::getNodeContent() const
{
    return (const char *)xmlNodeGetContent(node);
}

void XMLElement::setNodeName(const std::string & name) const
{
    xmlNodeSetName(node, (const xmlChar *)name.c_str());
}

void XMLElement::setNodeNameSpace(const XMLNs & ns) const
{
    xmlNs *n = ns.getRealNs();
    if (n)
    {
        if (!n->prefix || !xmlSearchNs(doc.getRealDocument(), node, n->prefix))
        {
            n = xmlNewNs(node, (const xmlChar *)ns.getHref(), (const xmlChar *)ns.getPrefix());
        }
        xmlSetNs(node, n);
    }
}

void XMLElement::setNodeContent(const std::string & content) const
{
    xmlNodeSetContent(node, (const xmlChar *)content.c_str());
}

void XMLElement::setAttributes(const XMLAttr & attrs) const
{
    xmlNode *attrNode = attrs.getElement().getRealNode();
    if (node != attrNode)
    {
        xmlFreePropList(node->properties);
        node->properties = 0;
        xmlCopyPropList(node, attrNode->properties);
    }
}

void XMLElement::setAttributeValue(const char **prefix, const char **name, const char **value, int size) const
{
    XMLAttr::setAttributeValue(node, prefix, name, value, size);
}

void XMLElement::setAttributeValue(const char **name, const char **value, int size) const
{
    XMLAttr::setAttributeValue(node, name, value, size);
}

void XMLElement::append(const XMLElement & elem) const
{
    XMLNodeList * list = 0;
    xmlNode *cpy = xmlCopyNode(elem.getRealNode(), 1);
    xmlUnlinkNode(cpy);
    xmlAddChild(node, cpy);

    list = scope->getXMLNodeListFromLibXMLPtr(node->children);
    if (list)
    {
        list->incrementSize();
    }
}

void XMLElement::setChildren(const XMLElement & elem) const
{
    xmlNode *n = elem.getRealNode();
    if (n && n->parent != node)
    {
        xmlNode *cpy = xmlCopyNode(n, 1);
        xmlUnlinkNode(cpy);
        xmlUnlinkNode(node->children);
        xmlFreeNodeList(node->children);
        node->children = 0;
        xmlAddChild(node, cpy);
    }
}

void XMLElement::setChildren(const XMLNodeList & list) const
{
    xmlNode *n = list.getRealNode();
    if (n && n->parent != node)
    {
        xmlNode *cpy = xmlCopyNodeList(n);
        xmlUnlinkNode(node->children);
        xmlFreeNodeList(node->children);
        node->children = 0;
        xmlAddChildList(node, cpy);
    }
}

void XMLElement::setChildren(const std::string & xmlCode) const
{
    std::string error;
    XMLDocument document = XMLDocument(xmlCode, false, &error);

    if (error.empty())
    {
        setChildren(*document.getRoot());
    }
    else
    {
        xmlNode *n = xmlNewText((xmlChar *) xmlCode.c_str());

        setChildren(XMLElement(doc, n));
    }
}

void XMLElement::addNamespace(const XMLNs & ns) const
{
    xmlNewNs(node, (const xmlChar *)ns.getHref(), (const xmlChar *)ns.getPrefix());
}

const XMLNs *XMLElement::getNamespaceByPrefix(const char *prefix) const
{
    xmlNs *ns = xmlSearchNs(doc.getRealDocument(), node, (const xmlChar *)prefix);
    XMLObject *obj = scope->getXMLObjectFromLibXMLPtr(ns);
    if (obj)
    {
        return static_cast < XMLNs * >(obj);
    }

    return new XMLNs(*this, ns);
}

const XMLNs *XMLElement::getNamespaceByHref(const char *href) const
{
    xmlNs *ns = xmlSearchNsByHref(doc.getRealDocument(), node, (const xmlChar *)href);
    XMLObject *obj = scope->getXMLObjectFromLibXMLPtr(ns);
    if (obj)
    {
        return static_cast < XMLNs * >(obj);
    }

    return new XMLNs(*this, ns);
}

const std::string XMLElement::dump(bool indent) const
{
    xmlBufferPtr buffer = xmlBufferCreate();
    xmlNodeDump(buffer, doc.getRealDocument(), node, 0, indent ? 1 : 0);
    std::string str = std::string((const char *)buffer->content);
    xmlBufferFree(buffer);

    return str;
}

const std::string XMLElement::toString() const
{
    std::ostringstream oss;
    std::string ns = "";
    std::string prefix = "";

    if (node->ns)
    {
        if (node->ns->href)
        {
            ns = std::string((const char *)node->ns->href);
        }

        if (node->ns->prefix)
        {
            prefix = std::string((const char *)node->ns->prefix);
        }
    }

    oss << "XML Element" << std::endl
        << "name: " << getNodeName() << std::endl
        << "namespace: XML Namespace" << std::endl
        << "    href: " << ns << std::endl
        << "    prefix: " << prefix << std::endl
        << "type: " << nodes_type[getNodeType() - 1] << std::endl
        << "parent: XML Element" << std::endl
        << "attributes: [1 x " << XMLAttr::getSize(node->properties) << "]" << std::endl
        << "children: [1 x " << XMLNodeList::getNodeListSize(node->children) << "]" << std::endl
        << "content: ..." << std::endl
        << "line: " << node->line;

    return oss.str();
}

int XMLElement::getDefinitionLine() const
{
    return node->line;
}

const XMLNs *XMLElement::getNodeNameSpace() const
{
    if (node->ns)
    {
        XMLObject *obj = scope->getXMLObjectFromLibXMLPtr(node->ns);
        if (obj)
        {
            return static_cast < XMLNs * >(obj);
        }

        return new XMLNs(*this, node->ns);
    }
    else
    {
        return 0;
    }
}

const XMLNodeList *XMLElement::getChildren() const
{
    XMLNodeList *obj = scope->getXMLNodeListFromLibXMLPtr(node->children);
    if (obj)
    {
        return obj;
    }

    return new XMLNodeList(doc, node);
}

const XMLAttr *XMLElement::getAttributes() const
{
    XMLObject *obj = scope->getXMLObjectFromLibXMLPtr(node->properties);
    if (obj)
    {
        return static_cast < XMLAttr * >(obj);
    }

    return new XMLAttr(*this);
}

const XMLElement *XMLElement::getParentElement() const
{
    if (node->parent && node->parent->type == XML_ELEMENT_NODE)
    {
        XMLObject *obj = scope->getXMLObjectFromLibXMLPtr(node->parent);
        if (obj)
        {
            return static_cast < XMLElement * >(obj);
        }

        return new XMLElement(doc, node->parent);
    }
    else
    {
        return 0;
    }
}
}
