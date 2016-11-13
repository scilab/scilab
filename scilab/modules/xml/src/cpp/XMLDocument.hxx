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

#ifndef __XMLDOCUMENT_HXX__
#define __XMLDOCUMENT_HXX__

#include <cstdio>
#include <list>
#include <cstring>
#include <string>

#include "dynlib_xml_scilab.h"

extern "C"
{
#include "xml.h"
#ifndef XML_XPATH_CHECKNS
#define XML_XPATH_CHECKNS
#endif
}

#include "XMLObject.hxx"

namespace org_modules_xml
{
class XMLElement;
class XMLObject;
class XMLXPath;
class XMLValidation;

/**
 * @file
 * @author Calixte DENIZET <calixte.denizet@scilab.org>
 *
 * Class to wrap a xmlDoc
 * @see http://xmlsoft.org/html/libxml-tree.html#xmlDoc
 */
class XML_SCILAB_IMPEXP XMLDocument: public XMLObject
{
    static std::list < XMLDocument * > openDocs;
    xmlDoc *document;

public:
    /**
     * Gets the list of open docs
     * @return the list
     */
    static const std::list < XMLDocument * >&getOpenDocuments();

    /**
     * Closes all the open documents
     */
    static void closeAllDocuments();

    /**
     * Builds a document with a given path (can be an url)
     * @param path the document path
     * @param validate a boolean to indicate if the document must be validated in using a DTD
     * @param error a pointer to a string which will receive the error message
     */
    XMLDocument(const char *path, bool validate, std::string * error, const char * encoding = 0, const bool html = false);

    /**
     * Builds a document with a given code
     * @param xmlCode the XML code
     * @param validate a boolean to indicate if the document must be validated in using a DTD
     * @param error a pointer to a string which will receive the error message
     */
    XMLDocument(const std::string & xmlCode, bool validate, std::string * error, const char * encoding = 0, const bool html = false);

    /**
     * Builds a simple document
     * @param uri the document uri
     * @param version the xml version
     */
    XMLDocument(char *uri, char *version);

    ~XMLDocument();

    void *getRealXMLPointer() const;

    /**
     * @return the xmlDoc behind this XMLDocument
     */
    xmlDoc *getRealDocument() const
    {
        return document;
    }

    /**
     * @param filename the file where to write xml
     * @param indent if true, the xml is indented
     * @return true if all is ok
     */
    bool saveToFile(const std::string & filename, const bool indent) const;

    /**
     * @param filename the file where to write xml
     * @param indent if true, the xml is indented
     * @return true if all is ok
     */
    bool saveToHTMLFile(const std::string & filename, const bool indent) const;

    /**
     * @return the document root
     */
    const XMLElement *getRoot() const;

    /**
     * @param value the root to set
     */
    void setRoot(const XMLElement & value) const;

    /**
     * Replaces the root element by the root of the xmlCode/
     * @param xmlCode the XML code
     * @param error a pointer to a string which will receive the error message
     */
    void setRoot(const std::string & xmlCode, std::string * error) const;

    /**
     * @return the document URL
     */
    const char *getDocumentURL() const;

    /**
     * @param value the document URL to set
     */
    void setDocumentURL(const std::string & value) const;

    /**
     * Makes an XPath query on the document
     * @param query the XPath query
     * @param namespaces an a matrix nx2 containing mapping between prefix and href
     * @param length the number of namespaces
     * @param the node from where start the query
     * @param error a pointer to a string which will receive the error message
     * @return a pointer on a XPath object
     */
    const XMLXPath *makeXPathQuery(const char *query, char **namespaces, int length, const XMLElement * e, std::string * error);

    const XMLObject *getXMLObjectParent() const;
    const std::string dump(bool indent) const;
    const std::string dumpHTML(bool indent) const;
    const std::string toString() const;

private:
    /**
     * Error function for the XML parser
     * @see http://xmlsoft.org/html/libxml-xmlerror.html#xmlGenericErrorFunc
     */
    static void errorFunction(void *ctx, const char *msg, ...);

    /**
     * Error function which does nothing for the XML parser
     * @see http://xmlsoft.org/html/libxml-xmlerror.html#xmlGenericErrorFunc
     */
    static void errorFunctionWithoutOutput(void *ctx, const char *msg, ...)
    {
    }

    /**
     * Error function used when the XPath query is compiled/
     * @see http://xmlsoft.org/html/libxml-xmlerror.html#xmlStructuredErrorFunc
     */
    static void errorXPathFunction(void *ctx, xmlError * error);

    /**
     * Reads and parses a document given in a file.
     * @param filename the file name
     * @param encoding the file encoding
     * @param validate a boolean to indicate if the document must be validated in using a DTD
     * @param error a string where to write the parsing errors
     * @return a pointer on a xmlDoc
     */
    static xmlDoc *readDocument(const char *filename, const char * encoding, bool validate, std::string * error);

    /**
     * Read and parse a document given in a string.
     * @param xmlCode the XML code
     * @param validate a boolean to indicate if the document must be validated in using a DTD
     * @param error a string where to write the parsing errors
     * @return a pointer on a xmlDoc
     */
    static xmlDoc *readDocument(const std::string & xmlCode, const char * encoding, bool validate, std::string * error);

    /**
     * Reads and parses a document given in a file.
     * @param filename the file name
     * @param validate a boolean to indicate if the document must be validated in using a DTD
     * @param error a string where to write the parsing errors
     * @return a pointer on a xmlDoc
     */
    static xmlDoc *readHTMLDocument(const char *filename, const char * encoding, std::string * error);

    /**
     * Read and parse a document given in a string.
     * @param xmlCode the XML code
     * @param validate a boolean to indicate if the document must be validated in using a DTD
     * @param error a string where to write the parsing errors
     * @return a pointer on a xmlDoc
     */
    static xmlDoc *readHTMLDocument(const std::string & xmlCode, const char * encoding, std::string * error);

    /**
     * Initializes the context
     * @param error a string where to write the parsing errors
     * @param validate a boolean to indicate if the document must be validated in using a DTD
     * @return a pointer on a context
     */
    static xmlParserCtxt *initContext(std::string * error, bool validate);

    /**
     * Initializes the context
     * @param error a string where to write the parsing errors
     * @param validate a boolean to indicate if the document must be validated in using a DTD
     * @return a pointer on a context
     */
    static htmlParserCtxt *initHTMLContext(std::string * error);

    static std::string errorBuffer;
    static std::string errorXPathBuffer;
};
}

#endif
