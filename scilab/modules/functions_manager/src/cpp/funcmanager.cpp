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
}



using namespace std;
using namespace ast;

#define BASENAMEMODULESFILE "etc/yasp_modules.xml"

bool FileExist(std::string _szFile);
char *GetXmlFileEncoding(string _filename);

FuncManager::FuncManager(void)
{
	/*parse all modules directories*/
	string szPath = ConfigVariable::getInstance()->get("SCI");
	szPath += "/";
	szPath += BASENAMEMODULESFILE;

	CreateModuleList();

	/*get module activation list from xml file*/
	if(GetModules() == true)
	{
		AppendModules();
	}
	CreateActivModuleList();

	LoadFuncByModule();
}

FuncManager::~FuncManager(void)
{
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
				free(encoding);
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
						m_ModuleName.push_back(name);
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
		std::cout << "Error: Not a valid module file " << m_szXmlFile << " (encoding not 'utf-8') Encoding '" << encoding << "' found." << std::endl;
	}
	if (encoding)
	{
		free(encoding);
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
				free(encoding);
				encoding = NULL;
			}
			encoding = strdup((char*)doc->encoding);
		}
	}

	xmlFreeDoc(doc);
	return encoding;
}

bool FuncManager::CreateModuleList(void)
{
	bool bRet = true;
	m_ModuleMap.insert(pair<string, GW_MOD>("elementary_functions", &ElemFuncModule::Load));
	m_ModuleMap.insert(pair<string, GW_MOD>("types", &TypesModule::Load));
	m_ModuleMap.insert(pair<string, GW_MOD>("boolean", &BooleanModule::Load));
	m_ModuleMap.insert(pair<string, GW_MOD>("integer", &IntegerModule::Load));
	m_ModuleMap.insert(pair<string, GW_MOD>("core", &CoreModule::Load));
	m_ModuleMap.insert(pair<string, GW_MOD>("io", &IoModule::Load));
	m_ModuleMap.insert(pair<string, GW_MOD>("functions", &FunctionsModule::Load));
	m_ModuleMap.insert(pair<string, GW_MOD>("output_stream", &OutputStreamModule::Load));
	m_ModuleMap.insert(pair<string, GW_MOD>("matio", &MatioModule::Load));
    m_ModuleMap.insert(pair<string, GW_MOD>("fileio", &FileioModule::Load));
	m_ModuleMap.insert(pair<string, GW_MOD>("overloading", NULL));
	m_ModuleMap.insert(pair<string, GW_MOD>("gui", &GuiModule::Load));
	return bRet;
}

bool FuncManager::CreateActivModuleList(void)
{
	bool bRet	= true;
	list<string>::const_iterator itName;
	for(itName = m_ModuleName.begin() ; itName != m_ModuleName.end() ; itName++)
	{
		map<string, GW_MOD>::iterator itModule = m_ModuleMap.find(*itName);
		if(itModule != m_ModuleMap.end())
		{
			m_ActivModuleMap.insert(pair<string, GW_MOD>(itModule->first,itModule->second));
		}
	}
	return bRet;
}

bool FuncManager::LoadFuncByModule(void)
{
	bool bRet	= true;
	map<string, GW_MOD>::const_iterator itMod;
	for(itMod = m_ActivModuleMap.begin() ; itMod != m_ActivModuleMap.end() ; itMod++)
	{
		//call Load function

        //check if module have gateways
        if(itMod->second != NULL)
        {
		    itMod->second();
        }
		LoadMacroFile(itMod->first);
	}
	return bRet;
}

bool FuncManager::LoadMacroFile(string _stModule)
{
	//macros
	string stPath = ConfigVariable::getInstance()->get("SCI");
	stPath += MODULE_DIR;
	stPath += _stModule;
	stPath += MACRO_DIR;
	int iSize = 0;
	char **pstPath = findfiles(const_cast<char*>(stPath.c_str()), const_cast<char*>("*.sci"), &iSize, FALSE);

	if(pstPath)
	{
		for(int k = 0 ; k < iSize ; k++)
		{
			//version with direct parsing
			//parse the file to find all functions
			//string stFullPath = stPath + string(pstPath[k]);
			//Parser::getInstance()->parseFile(stFullPath, ConfigVariable::getInstance()->get("SCI"));
			//FunctionDec* pFD = NULL;

			//std::list<Exp *>::iterator j;
			//std::list<Exp *>LExp = ((SeqExp*)Parser::getInstance()->getTree())->exps_get();
			//if(Parser::getInstance()->getExitStatus() == Parser::Failed)
			//{
			//	continue;
			//}

			//for(j = LExp.begin() ; j != LExp.end() ; j++)
			//{
			//	pFD = dynamic_cast<FunctionDec*>(*j);
			//	if(pFD)
			//	{
			//		std::list<ast::Var *>::const_iterator	i;

			//		//get input parameters list
			//		std::list<symbol::Symbol> *pVarList = new std::list<symbol::Symbol>();
			//		ArrayListVar *pListVar = (ArrayListVar *)&pFD->args_get();
			//		for(i = pListVar->vars_get().begin() ; i != pListVar->vars_get().end() ; i++)
			//		{
			//			string sz = ((SimpleVar*)(*i))->name_get();
			//			pVarList->push_back(((SimpleVar*)(*i))->name_get());
			//		}

			//		//get output parameters list
			//		std::list<symbol::Symbol> *pRetList = new std::list<symbol::Symbol>();
			//		ArrayListVar *pListRet = (ArrayListVar *)&pFD->returns_get();
			//		for(i = pListRet->vars_get().begin() ; i != pListRet->vars_get().end() ; i++)
			//		{
			//			pRetList->push_back(((SimpleVar*)(*i))->name_get());
			//		}

			//		//types::Macro macro(VarList, RetList, (SeqExp&)e.body_get());
			//		types::Macro *pMacro = new types::Macro(pFD->name_get(), *pVarList, *pRetList, (SeqExp&)pFD->body_get(), _stModule);
			//		symbol::Context::getInstance()->AddMacro(pMacro);
			//	}
			//}

			//version with macrofile

			string stFullPath = stPath + string(pstPath[k]);
			string stTemp = pstPath[k];
			size_t iPos = stTemp.find(".sci");
			if(iPos < stTemp.size())
			{
				string stFullName = stTemp.substr(0,iPos);
				symbol::Context::getInstance()->AddMacroFile(new MacroFile(stFullName, stFullPath, _stModule));
			}
			
			FREE(pstPath[k]);
		}
		FREE(pstPath);
	}
	return true;
}

