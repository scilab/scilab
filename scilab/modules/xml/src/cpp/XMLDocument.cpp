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

#include "XMLObject.hxx"
#include "XMLDocument.hxx"
#include "XMLElement.hxx"
#include "XMLXPath.hxx"
#include "VariableScope.hxx"

extern "C" {
#include "expandPathVariable.h"
#include "MALLOC.h"
}

#define BUFFER_SIZE 1024

namespace org_modules_xml
{
    std::string * XMLDocument::errorBuffer = 0;
    std::string * XMLDocument::errorXPathBuffer = 0;
    std::list<XMLDocument *> & XMLDocument::openDocs = *new std::list<XMLDocument *>();

    /**
     * Default constructor
     * @param path the path to the file (can be an url)
     * @param error will contain parsing errors if exists
     */
    XMLDocument::XMLDocument(const char * path, char ** error)
        : XMLObject()
    {
        char * expandedPath = expandPathVariable(const_cast<char *>(path));
        document = readDocument(const_cast<const char *>(expandedPath), error);
        FREE(expandedPath);
        if (document)
        {
            openDocs.push_back(this);
        }
	scilabType = XMLDOCUMENT;
    }

    /**
     * Destructor
     */
    XMLDocument::~XMLDocument()
    {
        scope.removeId<XMLDocument>(id);
        if (document)
        {
            openDocs.remove(this);
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

    XMLXPath * XMLDocument::makeXPathQuery(const char * query, char ** error)
    {
        if (errorXPathBuffer)
        {
            delete errorXPathBuffer;
        }
        errorXPathBuffer = new std::string();

        xmlXPathContext * ctxt = xmlXPathNewContext(document);
        if (!ctxt)
        {
            *error = const_cast<char *>("Cannot create a parser context");
            return 0;
        }

        xmlSetStructuredErrorFunc(ctxt, XMLDocument::errorXPathFunction);
        xmlXPathCompExpr * expr = xmlXPathCtxtCompile(ctxt, (const xmlChar *)query);
        if (!expr)
        {
            *error = const_cast<char *>(errorXPathBuffer->c_str());
            return 0;
        }

        xmlXPathObject * xpath = xmlXPathCompiledEval(expr, ctxt);
        if (!xpath)
        {
            *error = const_cast<char *>(errorXPathBuffer->c_str());
            return 0;
        }

        return new XMLXPath(this, xpath);
    }

    XMLObject * XMLDocument::getXMLObjectParent()
    {
        return 0;
    }

    std::string  * XMLDocument::toString()
    {
        std::string str = "XML Document\n";
        str += "URL: " + std::string(getDocumentURL());

        return new std::string(str);
    }

    std::string * XMLDocument::dump()
    {
        xmlChar * buffer = 0;
        int size = 0;
        xmlDocDumpFormatMemory(document, &buffer, &size, 1);
        std::string * str = new std::string((const char *)buffer);
        xmlFree(buffer);

        return str;
    }

    /**
     * @return the root element of this XMLDocument
     */
    XMLElement * XMLDocument::getRoot(void)
    {
        return new XMLElement(this, xmlDocGetRootElement(document));
    }

    std::list<XMLDocument *> & XMLDocument::getOpenDocuments()
    {
        return openDocs;
    }

    /**
     * Read and parse a document
     * @param filename the file name
     * @param error a string where to write the parsing errors
     * @return a pointer on a xmlDoc
     */
    xmlDoc * XMLDocument::readDocument(const char * filename, char **error)
    {
        xmlParserCtxtPtr ctxt;
        xmlDoc * doc;

        if (errorBuffer)
        {
            delete errorBuffer;
        }
        errorBuffer = new std::string();

        ctxt = xmlNewParserCtxt();
        if (!ctxt)
        {
            *error = const_cast<char *>("Cannot create a parser context");
            return 0;
        }

        xmlSetGenericErrorFunc(ctxt, XMLDocument::errorFunction);

        doc = xmlCtxtReadFile(ctxt, filename, 0, 0);
        if (!doc)
        {
            *error = const_cast<char *>(errorBuffer->c_str());
        }

        return doc;
    }

    /**
     * Error function to use in the parser (see xmlSetGenericErrorFunc)
     * @param ctx the context
     * @param msg the error message
     * @param ... the arguments to format the error message
     */
    void XMLDocument::errorFunction(void * ctx, const char * msg, ...)
    {
        char str[BUFFER_SIZE];
        va_list args;

        va_start(args, msg);
        vsnprintf(str, BUFFER_SIZE, msg, args);
        va_end(args);
        errorBuffer->append(str);
    }

    void XMLDocument::errorXPathFunction(void * ctx, xmlError * error)
    {
        errorXPathBuffer->append(error->message);
    }
}
