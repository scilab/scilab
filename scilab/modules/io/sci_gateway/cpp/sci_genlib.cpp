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

#define FILE_SEPARATOR L"/"

//XML API
#include <libxml/xpath.h>
#include <libxml/xmlwriter.h>

#include <string.h>
#include "parser.hxx"
#include "funcmanager.hxx"
#include "context.hxx"
#include "io_gw.hxx"
#include "setenvvar.hxx"
#include "yaspio.hxx"
#include "expandPathVariable.h"
#include "configvariable.hxx"

extern "C"
{
#include "MALLOC.h"
#include "localization.h"
#include "PATH_MAX.h"
#include "findfiles.h"
#include "FileExist.h"
#include "deleteafile.h"
}


xmlTextWriterPtr openXMLFile(const wchar_t *_pstFilename, const wchar_t* _pstLibName);
void closeXMLFile(xmlTextWriterPtr _pWriter);
bool AddMacroToXML(xmlTextWriterPtr _pWriter, pair<wstring, wstring> _pair);


using namespace types;
/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_genlib(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
	wchar_t pstParseFile[PATH_MAX + FILENAME_MAX];
	wchar_t pstVerbose[65535];
	
    int iNbFile	            = 0;
	wchar_t *pstParsePath      = NULL;
	int iParsePathLen		= 0;
	wchar_t* pstLibName		= NULL;
    bool bVerbose           = false;

	if(in.size() < 2 && in.size() > 4)
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

    if(in.size() > 2)
    {//force flag, do nothing but keep for compatibility
    }

    if(in.size() > 3)
    {//versbose flag
        pIT = in[2];
	    if(pIT->getType() != InternalType::RealBool)
	    {
		    return Function::Error;
	    }

        bVerbose = pIT->getAsBool()->bool_get()[0] == 1;
    }

	wchar_t* pstFile = pS->string_get(0);
    pstParsePath = expandPathVariableW(pstFile);

#ifdef _MSC_VER
	swprintf_s(pstParseFile, PATH_MAX + FILENAME_MAX, L"%s%slib", pstParsePath, FILE_SEPARATOR);
#else
	swprintf(pstParseFile, PATH_MAX + FILENAME_MAX, L"%S%Slib", pstParsePath, FILE_SEPARATOR);
#endif

    if(bVerbose)
    {
#ifdef _MSC_VER
        swprintf_s(pstVerbose, 65535, _W("-- Creation of [%s] (Macros) --\n"), pstLibName);
#else
        swprintf(pstVerbose, 65535, _W("-- Creation of [%S] (Macros) --\n"), pstLibName);
#endif
        YaspWriteW(pstVerbose);
    }

    if(FileExistW(pstParseFile))
    {
        deleteafileW(pstParseFile);
    }

	xmlTextWriterPtr pWriter = openXMLFile(pstParseFile, pstLibName);

    if(pWriter == NULL)
    {
#ifdef _MSC_VER
        swprintf_s(pstVerbose, 65535, _W("%s: Cannot open file ''%s''.\n"), L"genlib", pstParseFile);
#else
        swprintf(pstVerbose, 65535, _W("%S: Cannot open file ''%S''.\n"), L"genlib", pstParseFile);
#endif
        YaspWriteW(pstVerbose);

        out.push_back(new Bool(0));
        FREE(pstParsePath);
        return Function::OK;
    }


	wchar_t **pstPath = findfilesW(pstParsePath, L"*.sci", &iNbFile, FALSE);

	if(pstPath)
	{
		for(int k = 0 ; k < iNbFile ; k++)
		{
			//version with direct parsing
			//parse the file to find all functions
			wstring stFullPath = wstring(pstParsePath) + wstring(FILE_SEPARATOR) + wstring(pstPath[k]);

            Parser parser;
			parser.parseFile(stFullPath, ConfigVariable::getSCIPath());
            if(parser.getExitStatus() !=  Parser::Succeded)
            {
#ifdef _MSC_VER
                swprintf_s(pstVerbose, 65535, _W("%s: Warning: Error in file %s : %s. File ignored\n"), L"genlib", pstPath[k], parser.getErrorMessage());
#else
                swprintf(pstVerbose, 65535, _W("%S: Warning: Error in file %S : %S. File ignored\n"), L"genlib", pstPath[k], parser.getErrorMessage());
#endif
                YaspWriteW(pstVerbose);
                parser.freeTree();
                continue;
            }

			std::list<ast::Exp *>::iterator j;
			std::list<ast::Exp *>LExp = ((ast::SeqExp*)parser.getTree())->exps_get();

			for(j = LExp.begin() ; j != LExp.end() ; j++)
			{
				ast::FunctionDec* pFD = dynamic_cast<ast::FunctionDec*>(*j);
				if(pFD)
				{
					if(AddMacroToXML(pWriter, pair<wstring, wstring>(pFD->name_get(), pstPath[k])) == false)
                    {
#ifdef _MSC_VER
                        swprintf_s(pstVerbose, 65535, _W("%s: Warning: %s information cannot be added to file %s. File ignored\n"), L"genlib", pFD->name_get() , pstPath[k]);
#else
                        swprintf(pstVerbose, 65535, _W("%S: Warning: Error in file %S : %S. File ignored\n"), L"genlib", pstPath[k], parser.getErrorMessage());
#endif
                        YaspWriteW(pstVerbose);
                    }
				}
			}

			parser.freeTree();
		}
	}

    out.push_back(new Bool(1));
    FREE(pstParsePath);
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

xmlTextWriterPtr openXMLFile(const wchar_t *_pstFilename, const wchar_t* _pstLibName)
{
	int iLen;
	xmlTextWriterPtr pWriter = NULL;;
    char *pstFilename = wide_string_to_UTF8(_pstFilename);
    char *pstLibName = wide_string_to_UTF8(_pstLibName);


	//create a writer
	pWriter = xmlNewTextWriterFilename(pstFilename, 0);
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
	iLen = xmlTextWriterWriteAttribute(pWriter, (xmlChar*)"name", (xmlChar*)pstLibName);
	if (iLen < 0)
	{
		return false;
	}

    FREE(pstFilename);
    FREE(pstLibName);

	return pWriter;
}

bool AddMacroToXML(xmlTextWriterPtr _pWriter, pair<wstring, wstring> _pair)
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
    char* pstFirst = wide_string_to_UTF8(_pair.first.c_str());
	iLen = xmlTextWriterWriteAttribute(_pWriter, (xmlChar*)"name", (xmlChar*)pstFirst);
	if (iLen < 0)
	{
		return false;
	}
    FREE(pstFirst);

	//Add attribute "file"
    char* pstSecond = wide_string_to_UTF8(_pair.second.c_str());
	iLen = xmlTextWriterWriteAttribute(_pWriter, (xmlChar*)"file", (xmlChar*)pstSecond);
	if (iLen < 0)
	{
		return false;
	}
    FREE(pstSecond);

	//close "macro" node
	iLen = xmlTextWriterEndElement(_pWriter);
	if(iLen < 0)
	{
		return false;
	}
	return true;
}
