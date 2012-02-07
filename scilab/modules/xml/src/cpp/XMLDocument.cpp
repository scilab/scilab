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

#include "XMLObject.hxx"
#include "XMLDocument.hxx"
#include "XMLElement.hxx"
#include "XMLXPath.hxx"
#include "XMLValidation.hxx"
#include "XMLValidationRelaxNG.hxx"
#include "VariableScope.hxx"

extern "C"
{
#include "expandPathVariable.h"
#include "MALLOC.h"
#include "localization.h"
}

#define BUFFER_SIZE 1024

namespace org_modules_xml
{

    std::string * XMLDocument::errorBuffer = 0;
    std::string * XMLDocument::errorXPathBuffer = 0;
    std::list < XMLDocument * >&XMLDocument::openDocs = *new std::list < XMLDocument * >();

    XMLDocument::XMLDocument(const char *path, bool validate, std::string * error):XMLObject()
    {
        char *expandedPath = expandPathVariable(const_cast<char *>(path));
        if (expandedPath)
        {
            document = readDocument(const_cast<const char *>(expandedPath), validate, error);
            FREE(expandedPath);
            if (document)
            {
                openDocs.push_back(this);
                scope->registerPointers(document, this);
            }
        }
        else
        {
            document = 0;
            *error = std::string(gettext("Invalid file name: ")) + std::string(path);
        }

        id = scope->getVariableId(*this);
        scilabType = XMLDOCUMENT;
    }

    XMLDocument::XMLDocument(const std::string & xmlCode, bool validate, std::string * error):XMLObject()
    {
        document = readDocument(xmlCode, validate, error);
        if (document)
        {
            openDocs.push_back(this);
        }
        scope->registerPointers(document, this);
        id = scope->getVariableId(*this);
        scilabType = XMLDOCUMENT;
    }

    XMLDocument::XMLDocument(char *uri, char *version):XMLObject()
    {
        char *newUri = 0;
        char *expandedPath = 0;

        if (!version)
        {
            version = const_cast < char *>("1.0");
        }
        document = xmlNewDoc((xmlChar *) version);
        openDocs.push_back(this);
        scope->registerPointers(document, this);
        id = scope->getVariableId(*this);
        scilabType = XMLDOCUMENT;

        expandedPath = expandPathVariable(const_cast < char *>(uri));

        if (expandedPath)
        {
            newUri = (char *)xmlMalloc(sizeof(char *) * (strlen(expandedPath) + 1));
            memcpy(newUri, expandedPath, sizeof(char) * (strlen(expandedPath) + 1));
            document->URL = (xmlChar *) newUri;
            FREE(expandedPath);
        }
    }

    XMLDocument::~XMLDocument()
    {
        scope->unregisterPointer(document);
        scope->removeId(id);
        if (document)
        {
            openDocs.remove(this);
            if (openDocs.size() == 0 && XMLValidation::getOpenValidationFiles().size() == 0)
            {
                resetScope();
            }
            xmlFreeDoc(document);
        }
        if (errorBuffer)
        {
            delete errorBuffer;
            errorBuffer = 0;
        }
        if (errorXPathBuffer)
        {
            delete errorXPathBuffer;
            errorXPathBuffer = 0;
        }
    }

    void *XMLDocument::getRealXMLPointer() const
    {
        return static_cast < void *>(document);
    }

    const XMLXPath *XMLDocument::makeXPathQuery(const char *query, char **namespaces, int length, const XMLElement * e, std::string * error)
    {
        if (errorXPathBuffer)
        {
            delete errorXPathBuffer;
        }
        errorXPathBuffer = new std::string();

        xmlXPathContext *ctxt = xmlXPathNewContext(document);

        if (!ctxt)
        {
            errorXPathBuffer->append(gettext("Cannot create a parser context"));
            *error = *errorXPathBuffer;
            return 0;
        }

        if (e)
        {
            ctxt->node = (xmlNode *) e->getRealXMLPointer();
        }

        if (namespaces)
        {
            for (int i = 0; i < length; i++)
            {
                xmlXPathRegisterNs(ctxt, (const xmlChar *)namespaces[i], (const xmlChar *)namespaces[i + length]);
            }
        }

        xmlSetStructuredErrorFunc(ctxt, XMLDocument::errorXPathFunction);
        xmlXPathCompExpr *expr = xmlXPathCtxtCompile(ctxt, (const xmlChar *)query);

        if (!expr)
        {
            xmlSetStructuredErrorFunc(ctxt, 0);
            xmlXPathFreeContext(ctxt);
            *error = *errorXPathBuffer;
            return 0;
        }

        xmlXPathObject *xpath = xmlXPathCompiledEval(expr, ctxt);

        xmlSetStructuredErrorFunc(ctxt, 0);
        xmlXPathFreeContext(ctxt);
        xmlXPathFreeCompExpr(expr);
        if (!xpath)
        {
            *error = *errorXPathBuffer;
            return 0;
        }

        return new XMLXPath(*this, xpath);
    }

    const XMLObject *XMLDocument::getXMLObjectParent() const
    {
        return 0;
    }

    const std::string XMLDocument::toString() const
    {
        std::ostringstream oss;

        oss << "XML Document" << std::endl;
        oss << "URL: " << getDocumentURL();

        return oss.str();
    }

    const std::string XMLDocument::dump(bool indent) const
    {
        xmlChar *buffer = 0;
        int size = 0;
        xmlDocDumpFormatMemory(document, &buffer, &size, indent ? 1 : 0);
        std::string str = std::string((const char *)buffer);
        xmlFree(buffer);

        return str;
    }

    const XMLElement *XMLDocument::getRoot() const
    {
        xmlNode *root = xmlDocGetRootElement(document);
        if (!root)
        {
            return 0;
        }

        XMLObject *obj = scope->getXMLObjectFromLibXMLPtr(root);

        if (obj)
        {
            return static_cast < XMLElement * >(obj);
        }

        return new XMLElement(*this, root);
    }

    void XMLDocument::setRoot(const XMLElement & elem) const
    {
        xmlNode *root = xmlDocGetRootElement(document);
        if (root != elem.getRealNode())
        {
            xmlNode *cpy = xmlCopyNodeList(elem.getRealNode());
            xmlUnlinkNode(cpy);
            xmlDocSetRootElement(document, cpy);
        }
    }

    void XMLDocument::setRoot(const std::string & xmlCode, std::string * error) const
    {
        XMLDocument doc = XMLDocument(xmlCode, false, error);

        if (error->empty())
        {
            setRoot(*doc.getRoot());
        }
    }

    const char *XMLDocument::getDocumentURL() const
    {
        if (document->URL)
        {
            return (const char *)document->URL;
        }
        else
        {
            return "Undefined";
        }
    }

    void XMLDocument::setDocumentURL(const std::string & url) const
    {
        char *expandedPath = 0;
        char *newURL = 0;
        expandedPath = expandPathVariable(const_cast < char *>(url.c_str()));

        if (expandedPath)
        {
            xmlFree((void *)document->URL);
            newURL = (char *)xmlMalloc(sizeof(char *) * (strlen(expandedPath) + 1));
            memcpy(newURL, expandedPath, sizeof(char) * (strlen(expandedPath) + 1));
            document->URL = (xmlChar *) newURL;
            FREE(expandedPath);
        }
    }

    const std::list < XMLDocument * >&XMLDocument::getOpenDocuments()
    {
        return openDocs;
    }

    void XMLDocument::closeAllDocuments()
    {
        int size = (int)openDocs.size();
        XMLDocument **arr = new XMLDocument *[size];
        int j = 0;

        for (std::list < XMLDocument * >::iterator i = openDocs.begin(); i != openDocs.end(); i++, j++)
        {
            arr[j] = *i;
        }
        for (j = 0; j < size; j++)
        {
            delete arr[j];
        }
        delete[]arr;
    }

    xmlDoc *XMLDocument::readDocument(const char *filename, bool validate, std::string * error)
    {
        xmlParserCtxt *ctxt = initContext(error, validate);
        xmlDoc *doc = 0;
        int options = XML_PARSE_NSCLEAN;

        if (validate)
        {
            options = options | XML_PARSE_DTDVALID;
        }

        if (!ctxt)
        {
            xmlSetGenericErrorFunc(0, errorFunctionWithoutOutput);
            return 0;
        }

        doc = xmlCtxtReadFile(ctxt, filename, 0, options);
        if (!doc || !ctxt->valid)
        {
            *error = *errorBuffer;
        }

        xmlSetGenericErrorFunc(0, errorFunctionWithoutOutput);
        xmlFreeParserCtxt(ctxt);

        return doc;
    }

    xmlDoc *XMLDocument::readDocument(const std::string & xmlCode, bool validate, std::string * error)
    {
        xmlParserCtxt *ctxt = initContext(error, validate);
        xmlDoc *doc = 0;
        int options = XML_PARSE_NSCLEAN;

        if (validate)
        {
            options = options | XML_PARSE_DTDVALID;
        }

        if (!ctxt)
        {
            xmlSetGenericErrorFunc(0, errorFunctionWithoutOutput);
            return 0;
        }

        doc = xmlCtxtReadDoc(ctxt, (const xmlChar *)xmlCode.c_str(), 0, 0, options);
        if (!doc || !ctxt->valid)
        {
            *error = *errorBuffer;
        }

        xmlSetGenericErrorFunc(0, errorFunctionWithoutOutput);
        xmlFreeParserCtxt(ctxt);

        return doc;
    }

    xmlParserCtxt *XMLDocument::initContext(std::string * error, bool validate)
    {
        xmlParserCtxt *ctxt;

        if (errorBuffer)
        {
            delete errorBuffer;
        }
        errorBuffer = new std::string();

        ctxt = xmlNewParserCtxt();
        if (!ctxt)
        {
            errorBuffer->append(gettext("Cannot create a parser context"));
            *error = *errorBuffer;
            return 0;
        }

        if (validate)
        {
            ctxt->vctxt.error = (xmlValidityErrorFunc) errorFunction;
        }

        xmlSetGenericErrorFunc(ctxt, errorFunction);

        return ctxt;
    }

    void XMLDocument::errorFunction(void *ctx, const char *msg, ...)
    {
        char str[BUFFER_SIZE];
        va_list args;

        va_start(args, msg);
        vsnprintf(str, BUFFER_SIZE, msg, args);
        va_end(args);
        errorBuffer->append(str);
    }

    void XMLDocument::errorXPathFunction(void *ctx, xmlError * error)
    {
        errorXPathBuffer->append(error->message);
    }
}
