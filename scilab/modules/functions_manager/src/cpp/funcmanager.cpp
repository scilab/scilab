/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#pragma warning(disable : 4996) //It's not beautifull but that works !

//#include <iostream>
#include <fstream>
//#include <sstream>

//XML API
#include <libxml/xpath.h>
#include <libxml/xmlreader.h>

#include "funcmanager.hxx"
#include "foo.hxx"
#include "cosinus.hxx"
#include "configvariable.hxx"

using namespace std;

#define BASENAMEMODULESFILE "etc/yasp_modules.xml"

bool FileExist(std::string _szFile);
char *GetXmlFileEncoding(string _filename);

FuncManager::FuncManager(void)
{
	/*parse all modules directories*/
	string szPath = ConfigVariable::getInstance()->get("SCI");
	szPath += "/";
	szPath += BASENAMEMODULESFILE;
	cout << szPath << endl;

	/*get module list from xml file*/
	if(GetModules() == true)
	{
		AppendModules();
	}
}

FuncManager::~FuncManager(void)
{
	map <string, FuncInfo*>::iterator itMap;
	//clear containt of m_mapFuncList map
	for (itMap = m_mapFuncList.begin( ); itMap != m_mapFuncList.end( ); itMap++ )
		delete m_mapFuncList[itMap->first];
}

bool FuncManager::GetModules()
{
	string szModulesFilename;

	string szPath = ConfigVariable::getInstance()->get("SCI");
	//	SciPath=getSCIpath();
	if(szPath == "")
	{
		std::cout << "The SCI environment variable is not set." << std::endl;
		return false;
	}

	szModulesFilename = szPath + "/";
	szModulesFilename += BASENAMEMODULESFILE;

	if(FileExist(szModulesFilename))
	{
		m_szXmlFile = szModulesFilename;
	}
	else
	{
		std::cout << "Cannot load the module declaration file: " << szModulesFilename << std::endl;
		return false;
	}
	return true;
}

bool FuncManager::AppendModules()
{
	char *encoding=GetXmlFileEncoding(m_szXmlFile);

	/* Don't care about line return / empty line */
	xmlKeepBlanksDefault(0);
	/* check if the XML file has been encoded with utf8 (unicode) or not */
	if(stricmp("utf-8", encoding)==0)
	{
		xmlDocPtr doc;
		xmlXPathContextPtr xpathCtxt	= NULL;
		xmlXPathObjectPtr xpathObj		= NULL;
		char *name										= NULL;
		int activate									= 0;
		int indice										= 0;

		doc = xmlParseFile (m_szXmlFile.c_str());

		if(doc == NULL) 
		{
			std::cout << "Error: Could not parse file " << m_szXmlFile << std::endl;
			if(encoding) 
			{
				delete encoding;
				encoding=NULL;
			}
			return false;
		}

		xpathCtxt = xmlXPathNewContext(doc);
		xpathObj = xmlXPathEval((const xmlChar*)"//modules/module", xpathCtxt);

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
						name = strdup(str);
					}
					else if(xmlStrEqual(attrib->name, (const xmlChar*)"activate"))
					{ 
						/* we found the tag activate */
						const char *str = (const char*)attrib->children->content;
						if(stricmp(str,"yes") == 0 || strcmp(str,"1") == 0)
						{
							activate=1;
						}
					}
					attrib = attrib->next;
				}

				if((name) && (strlen(name) > 0) && (activate))
				{
					if(VerifyModule(name))
					{
						m_ModuleList.push_back(name);
/*
						if (indice==0)
						{
							ScilabModules->ModuleList = new char*[indice+1]; 
						}
						else 
						{
							ScilabModules->ModuleList=(char**)REALLOC(ScilabModules->ModuleList,sizeof(char*)*(indice+1));
						}

						ScilabModules->numberofModules=indice+1;

						ScilabModules->ModuleList[indice]= strdup(name);
						indice++;
*/					}
					else
					{
						std::cout << name << " module not found." << std::endl;
					}
				}
				if(name)
				{
					delete name;
					name = NULL;
				}
				activate = 0;
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
		std::cout << "Error: Not a valid module file " << m_szXmlFile << " (encoding not 'utf-8') Encoding '" << encoding << "' found." << std::endl;
	}
	if (encoding) 
	{
		delete encoding;
		encoding = NULL;
	}
	return true;
}

bool FuncManager::VerifyModule(char *_pszModuleName)
{
	string SciPath = ConfigVariable::getInstance()->get("SCI");
	if(SciPath == "")
	{
		std::cout << "The SCI environment variable is not set." << std::endl;
		return false;
	}

	string FullPathModuleName = SciPath + "/modules/" + _pszModuleName + "/etc/" + _pszModuleName + ".start";

	/* ajouter d'autres tests d'existences */

	if(!FileExist(FullPathModuleName))
	{
		return false;
	}
	return true;
}

/*Out of class*/
bool FileExist(std::string _szFile)
{
	bool bReturn = false;
	fstream filestr(_szFile.c_str(), fstream::in);
	bReturn = !filestr.fail();
	filestr.close();
	return bReturn;
}

char *GetXmlFileEncoding(string _filename)
{
#define DEFAULT_ENCODING "UTF-8"

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
				delete encoding;
				encoding = NULL;
			}
			encoding = strdup((char*)doc->encoding);
		}
	}

	xmlFreeDoc(doc);
	return encoding;
}
