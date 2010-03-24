/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifdef _MSC_VER
#pragma warning(disable : 4996) //It's not beautifull but that works !
#endif

#define DEFAULT_ENCODING "UTF-8"

#define FILE_SEPARATOR "/"

#include <string.h>
#include "funcmanager.hxx"
#include "context.hxx"
#include "io_gw.hxx"
#include "setenvvar.hxx"


extern "C"
{
#ifndef _MSC_VER
#include "stricmp.h"
#endif
#include "cluni0.h"
#include "PATH_MAX.h"
}

//XML API
#include <libxml/xpath.h>
#include <libxml/xmlreader.h>

char *GetXmlFileEncoding(string _filename);
bool getMacros(string _stXMLFile);

using namespace types;
/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_load(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
	int iXMLFileLen = 0;
	char pstXMLFile[PATH_MAX + FILENAME_MAX];

	if(in.size() != 1)
	{
		return Function::Error;
	}

	InternalType* pIT = in[0];

	if(pIT->getType() != InternalType::RealString)
	{
		return Function::Error;
	}

	String *pS = pIT->getAsString();

	if(pS->size_get() != 1)
	{
		return Function::Error;
	}

	char* pstFile = pS->string_get(0);
	C2F(cluni0)(pstFile, pstXMLFile, &iXMLFileLen, (long)strlen(pstFile), PATH_MAX + FILENAME_MAX);

	if(getMacros(pstXMLFile) == false)
	{
		return Function::Error;
	}
	return Function::OK;
}

bool getMacros(string _stXMLFile)
{
	symbol::Context* pContext = symbol::Context::getInstance();
	char *encoding=GetXmlFileEncoding(_stXMLFile);

	/* Don't care about line return / empty line */
	xmlKeepBlanksDefault(0);
	/* check if the XML file has been encoded with utf8 (unicode) or not */
	if(stricmp("utf-8", encoding)==0)
	{
		xmlDocPtr doc;
		xmlXPathContextPtr xpathCtxt	= NULL;
		xmlXPathObjectPtr xpathObj		= NULL;
		char *pstName									= NULL;
		char *pstLibName							= NULL;
		char *pstFileName							= NULL;

		int indice										= 0;

		doc = xmlParseFile (_stXMLFile.c_str());

		if(doc == NULL)
		{
			std::cout << "Error: Could not parse file " << _stXMLFile << std::endl;
			if(encoding)
			{
				free(encoding);
				encoding=NULL;
			}
			return false;
		}

		xpathCtxt = xmlXPathNewContext(doc);
		xpathObj = xmlXPathEval((const xmlChar*)"//scilablib", xpathCtxt);
		if(xpathObj && xpathObj->nodesetval->nodeMax)
		{
			xmlAttrPtr attrib = xpathObj->nodesetval->nodeTab[0]->properties;
			if(xmlStrEqual (attrib->name, (const xmlChar*)"name"))
			{
				/* we found the tag name */
				const char *str = (const char*)attrib->children->content;
				pstLibName = strdup(str);
			}
			else
			{
				return false;
			}
		}

		xpathObj = xmlXPathEval((const xmlChar*)"//scilablib/macro", xpathCtxt);
		if(xpathObj && xpathObj->nodesetval->nodeMax)
		{
			/* the Xpath has been understood and there are node */
			for(int	i = 0 ; i < xpathObj->nodesetval->nodeNr ; i++)
			{

				xmlAttrPtr attrib = xpathObj->nodesetval->nodeTab[i]->properties;
				/* Get the properties of <module>  */
				while(attrib != NULL)
				{
					/* loop until when have read all the attributes */
					if(xmlStrEqual (attrib->name, (const xmlChar*)"name"))
					{
						/* we found the tag name */
						const char *str = (const char*)attrib->children->content;
						pstName = strdup(str);
					}
					else if(xmlStrEqual(attrib->name, (const xmlChar*)"file"))
					{
						/* we found the tag activate */
						const char *str = (const char*)attrib->children->content;
						pstFileName = strdup(str);
					}
					attrib = attrib->next;
				}

				if(pstName && pstFileName)
				{
				  size_t iPos = _stXMLFile.rfind(DIR_SEPARATOR);
					string stFilename = _stXMLFile.substr(0,iPos + 1) + pstFileName;
					pContext->AddMacroFile(new MacroFile(pstName, stFilename, pstLibName));
				
				}
				else
				{
					std::cout << pstName << " module not found." << std::endl;
				}

				if(pstName)
				{
					free(pstName);
					pstName = NULL;
				}

				if(pstFileName)
				{
					free(pstFileName);
					pstFileName = NULL;
				}
			}
		}

		if(xpathObj) xmlXPathFreeObject(xpathObj);
		if(xpathCtxt) xmlXPathFreeContext(xpathCtxt);
		xmlFreeDoc (doc);
		/*
		* Cleanup function for the XML library.
		*/
		xmlCleanupParser();
	}
	else
	{
		std::cout << "Error: Not a valid module file " << _stXMLFile << " (encoding not 'utf-8') Encoding '" << encoding << "' found." << std::endl;
	}
	if (encoding)
	{
		free(encoding);
		encoding = NULL;
	}
	return true;
}

char *GetXmlFileEncoding(string _filename)
{
	char *encoding = NULL;
	xmlDocPtr doc = NULL;

	/* default */
	encoding = strdup(DEFAULT_ENCODING);

	doc = xmlParseFile(_filename.c_str());
	if (doc)
	{
		if (doc->encoding)
		{
			if(encoding)
			{
				free(encoding);
				encoding = NULL;
			}
			encoding = strdup((char*)doc->encoding);
		}
	}

	xmlFreeDoc(doc);
	return encoding;
}

/*--------------------------------------------------------------------------*/
