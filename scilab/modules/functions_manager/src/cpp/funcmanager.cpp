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

#ifdef _MSC_VER
#pragma warning(disable : 4996) //It's not beautifull but that works !
#endif


//#include <iostream>
#include <fstream>
//#include <sstream>

//XML API
#include <libxml/xpath.h>
#include <libxml/xmlreader.h>

#include "MALLOC.h"
#include "funcmanager.hxx"
#include "configvariable.hxx"
#include "module_declaration.hxx"
#include "parser.hxx"
#include "execvisitor.hxx"

extern "C"
{
	#include "findfiles.h"
#ifndef _MSC_VER
	#include "stricmp.h"
#endif
#include "os_strdup.h"
}



using namespace std;
using namespace ast;

#define BASENAMEMODULESFILE L"etc/yasp_modules.xml"

bool FileExist(std::string _szFile);
bool FileExist(std::wstring _szFile);
char *GetXmlFileEncoding(string _filename);

FuncManager::FuncManager(void)
{
    m_bNoStart = false;
}

FuncManager::~FuncManager(void)
{
}

bool FuncManager::LoadModules(bool _bNoStart)
{
    m_bNoStart = _bNoStart;
	if(CreateModuleList() == false)
    {
        return false;
    }

	/*get module activation list from xml file*/
	if(GetModules() == true)
	{
		if(AppendModules() == false)
        {
            return false;
        }
	}

    return LoadFuncByModule();
}

bool FuncManager::GetModules()
{
	wstring szModulesFilename;

	wstring szPath = ConfigVariable::getSCIPath();
	if(szPath == L"")
	{
		std::wcout << L"The SCI environment variable is not set." << std::endl;
		return false;
	}

	szModulesFilename = szPath + L"/";
	szModulesFilename += BASENAMEMODULESFILE;

	if(FileExist(szModulesFilename))
	{
		m_szXmlFile = szModulesFilename;
	}
	else
	{
		std::wcout << L"Cannot load the module declaration file: " << szModulesFilename << std::endl;
		return false;
	}
	return true;
}

bool FuncManager::AppendModules()
{
    char* pstTemp = wide_string_to_UTF8(m_szXmlFile.c_str());

	char *encoding=GetXmlFileEncoding(pstTemp);

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

		doc = xmlParseFile (pstTemp);

		if(doc == NULL)
		{
			std::cout << "Error: Could not parse file " << pstTemp << std::endl;
			if(encoding)
			{
				free(encoding);
				encoding=NULL;
			}
            FREE(pstTemp);
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
						name = os_strdup(str);
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
                    wchar_t* pstName = to_wide_string(name);
					if(VerifyModule(pstName))
					{
						m_ModuleName.push_back(pstName);
                    }
					else
					{
						std::wcout << pstName << " module not found." << std::endl;
					}
                    FREE(pstName);
				}

				if(name)
				{
					free(name);
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
		std::cout << "Error: Not a valid module file " << pstTemp << " (encoding not 'utf-8') Encoding '" << encoding << "' found." << std::endl;
	}
	if (encoding)
	{
		free(encoding);
		encoding = NULL;
	}

    String *pS  = new String(static_cast<int>(m_ModuleName.size()), 1);

    list<wstring>::iterator it = m_ModuleName.begin();
    for(int i = 0; it != m_ModuleName.end() ; it++,i++)
    {
        pS->string_set(i, it->c_str());
    }

    symbol::Context::getInstance()->put(L"modules_list", *pS);

    FREE(pstTemp);

	return true;
}

bool FuncManager::VerifyModule(wchar_t* _pszModuleName)
{
	wstring SciPath = ConfigVariable::getSCIPath();
	if(SciPath == L"")
	{
		std::wcout << L"The SCI environment variable is not set." << std::endl;
		return false;
	}

	wstring FullPathModuleName = SciPath + L"/modules/" + _pszModuleName + L"/etc/" + _pszModuleName + START_EXT;

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

bool FileExist(std::wstring _szFile)
{
	bool bReturn = false;
    char *pstFile = wide_string_to_UTF8(_szFile.c_str());
	wfstream filestr(pstFile, wfstream::in);
	bReturn = !filestr.fail();
	filestr.close();
    FREE(pstFile);
	return bReturn;
}

char *GetXmlFileEncoding(string _filename)
{
#define DEFAULT_ENCODING "UTF-8"

	char *encoding = NULL;
	xmlDocPtr doc = NULL;

	/* default */
	encoding = os_strdup(DEFAULT_ENCODING);

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
			encoding = os_strdup((char*)doc->encoding);
		}
	}

	xmlFreeDoc(doc);
	return encoding;
}

bool FuncManager::CreateModuleList(void)
{
	m_ModuleMap.insert(pair<wstring, GW_MOD>(L"elementary_functions", &ElemFuncModule::Load));
	m_ModuleMap.insert(pair<wstring, GW_MOD>(L"types", &TypesModule::Load));
	m_ModuleMap.insert(pair<wstring, GW_MOD>(L"boolean", &BooleanModule::Load));
	m_ModuleMap.insert(pair<wstring, GW_MOD>(L"integer", &IntegerModule::Load));
	m_ModuleMap.insert(pair<wstring, GW_MOD>(L"core", &CoreModule::Load));
	m_ModuleMap.insert(pair<wstring, GW_MOD>(L"io", &IoModule::Load));
	m_ModuleMap.insert(pair<wstring, GW_MOD>(L"functions", &FunctionsModule::Load));
	m_ModuleMap.insert(pair<wstring, GW_MOD>(L"output_stream", &OutputStreamModule::Load));
	m_ModuleMap.insert(pair<wstring, GW_MOD>(L"matio", &MatioModule::Load));
    m_ModuleMap.insert(pair<wstring, GW_MOD>(L"fileio", &FileioModule::Load));
	m_ModuleMap.insert(pair<wstring, GW_MOD>(L"gui", &GuiModule::Load));
	m_ModuleMap.insert(pair<wstring, GW_MOD>(L"time", &TimeModule::Load));
	m_ModuleMap.insert(pair<wstring, GW_MOD>(L"string", &StringModule::Load));
	return true;
}

bool FuncManager::LoadFuncByModule(void)
{
	bool bRet	= true;
	list<wstring>::const_iterator itName;

    //load gateways
	for(itName = m_ModuleName.begin() ; itName != m_ModuleName.end() ; itName++)
	{
		map<wstring, GW_MOD>::iterator itModule = m_ModuleMap.find(*itName);
		if(itModule != m_ModuleMap.end())
		{
            //check if module have gateways
            if(itModule->second != NULL)
            {
                //call ::Load function
                itModule->second();
            }
		}
	}

    if(m_bNoStart == false)
    {
        //excute .start file
	    for(itName = m_ModuleName.begin() ; itName != m_ModuleName.end() ; itName++)
	    {
		    ExecuteStartFile(*itName);
	    }
    }

	return bRet;
}

bool FuncManager::ExecuteStartFile(wstring _stModule)
{
    //build .start filename
	wstring stPath = ConfigVariable::getSCIPath();
	stPath += MODULE_DIR;
	stPath += _stModule;
	stPath += ETC_DIR;
	stPath += _stModule;
	stPath += START_EXT;

    Parser parser;

    parser.parseFile(stPath, ConfigVariable::getSCIPath());

    if(parser.getExitStatus() == Parser::Failed)
    {
        std::wostringstream ostr;
        ostr << _W("Unable to execute : ") << stPath << endl;
        YaspWriteW(ostr.str().c_str());
        parser.freeTree();
        return false;
    }

    ExecVisitor execStart;

    try
    {
        parser.getTree()->accept(execStart);
    }
    catch(wstring sz)
    {
        YaspWriteW(sz.c_str());
        YaspWriteW(L"\n");
        parser.freeTree();
        return false;
    }

    parser.freeTree();
    return true;
}
