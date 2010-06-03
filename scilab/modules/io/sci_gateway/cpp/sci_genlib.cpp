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
#include "yaspio.hxx"
#include "expandPathVariable.h"

extern "C"
{
#include "MALLOC.h"
#include "localization.h"
#include "PATH_MAX.h"
#include "findfiles.h"
#include "FileExist.h"
#include "deleteafile.h"
}


xmlTextWriterPtr openXMLFile(const char *_pstFilename, const char* _pstLibName);
void closeXMLFile(xmlTextWriterPtr _pWriter);
bool AddMacroToXML(xmlTextWriterPtr _pWriter, pair<string, string> _pair);


using namespace types;
/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_genlib(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
	char pstParseFile[PATH_MAX + FILENAME_MAX];
	char pstVerbose[4096];

    int iNbFile	            = 0;
	char *pstParsePath      = NULL;
	int iParsePathLen		= 0;
	char* pstLibName		= NULL;
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

	char* pstFile = pS->string_get(0);
    pstParsePath = expandPathVariable(pstFile);

#ifdef _MSC_VER
	sprintf_s(pstParseFile, PATH_MAX + FILENAME_MAX, "%s%slib", pstParsePath, DIR_SEPARATOR);
#else
	sprintf(pstParseFile, "%s%slib", pstParsePath, DIR_SEPARATOR);
#endif

    if(bVerbose)
    {
#ifdef _MSC_VER
        sprintf_s(pstVerbose, 4096, _("-- Creation of [%s] (Macros) --\n"), pstLibName);
#else
        sprintf(pstVerbose, _("-- Creation of [%s] (Macros) --\n"), pstLibName);
#endif
        YaspWrite(pstVerbose);
    }

    if(FileExist(pstParseFile))
    {
        deleteafile(pstParseFile);
    }

	xmlTextWriterPtr pWriter = openXMLFile(pstParseFile, pstLibName);
    if(pWriter == NULL)
    {
#ifdef _MSC_VER
        sprintf_s(pstVerbose, 4096, _("%s: Cannot open file ''%s''.\n"), "genlib", pstParseFile);
#else
        sprintf(pstVerbose, _("%s: Cannot open file ''%s''.\n"), pstParseFile);
#endif
        YaspWrite(pstVerbose);

        out.push_back(new Bool(0));
        FREE(pstParsePath);
        return Function::OK;
    }


	char **pstPath = findfiles(pstParsePath, const_cast<char*>("*.sci"), &iNbFile, FALSE);

	if(pstPath)
	{
		for(int k = 0 ; k < iNbFile ; k++)
		{
			//version with direct parsing
			//parse the file to find all functions
			string stFullPath = string(pstParsePath) + string(DIR_SEPARATOR) + string(pstPath[k]);

            Parser *pParser = Parser::getInstance();
			pParser->parseFile(stFullPath, ConfigVariable::getInstance()->get("SCI"));
            if(Parser::getInstance()->getExitStatus() !=  Parser::Succeded)
            {
#ifdef _MSC_VER
                sprintf_s(pstVerbose, 4096, _("%s: Warning: Error in file %s : %s. File ignored\n"), "genlib", pstPath[k], pParser->getErrorMessage());
#else
                sprintf(pstVerbose, _("%s: Warning: Error in file %s : %s. File ignored\n"), "genlib", pstPath[k], pParser->getErrorMessage());
#endif
                YaspWrite(pstVerbose);
                //pParser->freeTree();
                continue;
            }

			std::list<ast::Exp *>::iterator j;
			std::list<ast::Exp *>LExp = ((ast::SeqExp*)pParser->getTree())->exps_get();

			for(j = LExp.begin() ; j != LExp.end() ; j++)
			{
				ast::FunctionDec* pFD = dynamic_cast<ast::FunctionDec*>(*j);
				if(pFD)
				{
					if(AddMacroToXML(pWriter, pair<string, string>(string(pFD->name_get()), string(pstPath[k]))) == false)
                    {
#ifdef _MSC_VER
                        sprintf_s(pstVerbose, 4096, _("%s: Warning: %s information cannot be added to file %s. File ignored\n"), "genlib", pFD->name_get() , pstPath[k]);
#else
                        sprintf(pstVerbose, _("%s: Warning: Error in file %s : %s. File ignored\n"), "genlib", pstPath[k], pParser->getErrorMessage());
#endif
                        YaspWrite(pstVerbose);
                    }
				}
			}

			pParser->freeTree();
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
