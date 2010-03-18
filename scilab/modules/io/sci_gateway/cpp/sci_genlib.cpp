/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
* 
7* This file must be used under the terms of the CeCILL.
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

//XML API
#include <libxml/xpath.h>
#include <libxml/xmlwriter.h>

#include <string.h>
#include "parser.hxx"
#include "funcmanager.hxx"
#include "context.hxx"
#include "io_gw.hxx"
#include "setenvvar.hxx"

extern "C"
{
#include "cluni0.h"
#include "PATH_MAX.h"
#include "findfiles.h"
}


xmlTextWriterPtr openXMLFile(const char *_pstFilename, const char* _pstLibName);
void closeXMLFile(xmlTextWriterPtr _pWriter);
bool AddMacroToXML(xmlTextWriterPtr _pWriter, pair<string, string> _pair);


using namespace types;
/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_genlib(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
	int iNbFile	= 0;
	char pstParsePath[PATH_MAX + FILENAME_MAX];
	char pstParseFile[PATH_MAX + FILENAME_MAX];
	int iParsePathLen		= 0;
	char* pstLibName		= NULL;

	if(in.size() < 2)
	{
		return Function::Error;
	}

	//param 1, library name
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
	pstLibName = pS->string_get(0);

	//param 2, library path
	pIT = in[1];
	if(pIT->getType() != InternalType::RealString)
	{
		return Function::Error;
	}

	pS = pIT->getAsString();
	if(pS->size_get() != 1)
	{
		return Function::Error;
	}

	char* pstFile = pS->string_get(0);
	C2F(cluni0)(pstFile, pstParsePath, &iParsePathLen, (long)strlen(pstFile), PATH_MAX + FILENAME_MAX);


#ifdef _MSC_VER
	sprintf_s(pstParseFile, PATH_MAX + FILENAME_MAX, "%s%s%s.xml", pstParsePath, DIR_SEPARATOR, pstLibName);
#else
	sprintf(pstParseFile, "%s%s%s.xml", pstParsePath, DIR_SEPARATOR, pstLibName);
#endif
	xmlTextWriterPtr pWriter = openXMLFile(pstParseFile, pstLibName);


	char **pstPath = findfiles(pstParsePath, const_cast<char*>("*.sci"), &iNbFile, FALSE);

	if(pstPath)
	{
		for(int k = 0 ; k < iNbFile ; k++)
		{
			//version with direct parsing
			//parse the file to find all functions
			string stFullPath = string(pstParsePath) + string(DIR_SEPARATOR) + string(pstPath[k]);
			Parser::getInstance()->parseFile(stFullPath, ConfigVariable::getInstance()->get("SCI"));
			ast::FunctionDec* pFD = NULL;

			std::list<ast::Exp *>::iterator j;
			std::list<ast::Exp *>LExp = ((ast::SeqExp*)Parser::getInstance()->getTree())->exps_get();
			if(Parser::getInstance()->getExitStatus() == Parser::Failed)
			{
				Parser::getInstance()->freeTree();
				continue;
			}

			for(j = LExp.begin() ; j != LExp.end() ; j++)
			{
				pFD = dynamic_cast<ast::FunctionDec*>(*j);
				if(pFD)
				{
					AddMacroToXML(pWriter, pair<string, string>(string(pFD->name_get()), string(pstPath[k])));
				}
			}

			Parser::getInstance()->freeTree();
		}
	}


	closeXMLFile(pWriter);
	return Function::OK;
}

void closeXMLFile(xmlTextWriterPtr _pWriter)
{
	int iLen;

	//close opened nodes
	iLen = xmlTextWriterEndElement(_pWriter);
	if(iLen < 0)
	{
		return;
	}

	//close document
	iLen = xmlTextWriterEndDocument(_pWriter);
	if(iLen < 0)
	{
		return;
	}

	//close xml writer
	xmlFreeTextWriter(_pWriter);
}

xmlTextWriterPtr openXMLFile(const char *_pstFilename, const char* _pstLibName)
{
	int iLen;
	xmlTextWriterPtr pWriter = NULL;;

	//create a writer
	pWriter = xmlNewTextWriterFilename(_pstFilename, 0);
	if(pWriter == NULL)
	{
		return NULL;
	}

	//setup indentation
	xmlTextWriterSetIndent (pWriter, 1);
	xmlTextWriterSetIndentString (pWriter, (xmlChar*)"  ");

	//create a new document
	iLen = xmlTextWriterStartDocument(pWriter, NULL, DEFAULT_ENCODING, "no");
	if(iLen < 0)
	{
		return NULL;
	}

	//add a node "scilablib"
	iLen = xmlTextWriterStartElement(pWriter, (xmlChar*)"scilablib");
	if(iLen < 0)
	{
		return NULL;
	}

	//Add attribute "name"
	iLen = xmlTextWriterWriteAttribute(pWriter, (xmlChar*)"name", (xmlChar*)_pstLibName);
	if (iLen < 0)
	{
		return false;
	}

	return pWriter;
}

bool AddMacroToXML(xmlTextWriterPtr _pWriter, pair<string, string> _pair)
{
	int iLen;

	if(_pWriter == NULL)
	{
		return false;
	}

	//create node "macro"
	iLen = xmlTextWriterStartElement(_pWriter, (xmlChar*)"macro");
	if(iLen < 0)
	{
		return false;
	}

	//Add attribute "name"
	iLen = xmlTextWriterWriteAttribute(_pWriter, (xmlChar*)"name", (xmlChar*)_pair.first.c_str());
	if (iLen < 0)
	{
		return false;
	}

	//Add attribute "file"
	iLen = xmlTextWriterWriteAttribute(_pWriter, (xmlChar*)"file", (xmlChar*)_pair.second.c_str());
	if (iLen < 0)
	{
		return false;
	}

	//close "macro" node
	iLen = xmlTextWriterEndElement(_pWriter);
	if(iLen < 0)
	{
		return false;
	}
	return true;
}
