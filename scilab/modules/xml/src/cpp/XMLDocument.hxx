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

#include <cstdio>
#include <list>
#include <string>

#include "xml.h"

namespace org_modules_xml
{
    class XMLElement;
    class XMLObject;
    class XMLXPath;

    class XMLDocument : public XMLObject
    {
	static std::list<XMLDocument *> & openDocs;
        xmlDoc * document;
	
    public :
        XMLDocument(const char * path, char **error);
	~XMLDocument();

	static std::list<XMLDocument *> & getOpenDocuments();

	xmlDoc * getRealDocument() { return document; }
        XMLElement * getRoot();
	XMLXPath * makeXPathQuery(const char * query, char ** error);
        const char * getDocumentURL() { return (const char *)document->URL; }
        XMLObject * getXMLObjectParent();
	std::string * dump();
	std::string * toString();

    private :
	static void errorFunction(void * ctx, const char * msg, ...);
	static void errorXPathFunction(void * ctx, xmlError * error);
	static xmlDoc * readDocument(const char * filename, char ** error);

	static std::string * errorBuffer;
	static std::string * errorXPathBuffer;
    };
}
