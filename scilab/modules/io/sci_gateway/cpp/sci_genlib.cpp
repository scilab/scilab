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

#ifdef _MSC_VER
#define FILE_SEPARATOR "\\"
#else
#define FILE_SEPARATOR "/"
#endif

//XML API
#include <libxml/xpath.h>
#include <libxml/xmlwriter.h>

#include <cstring>
#include "string.hxx"
#include "parser.hxx"
#include "context.hxx"
#include "io_gw.hxx"
#include "scilabWrite.hxx"
#include "configvariable.hxx"
#include "library.hxx"
#include "macrofile.hxx"
#include "serializervisitor.hxx"
#include "loadlib.hxx"

extern "C"
{
#include "setenvvar.h"
#include "sci_malloc.h"
#include "localization.h"
#include "PATH_MAX.h"
#include "findfiles.h"
#include "FileExist.h"
#include "deleteafile.h"
#include "os_string.h"
#include "splitpath.h"
#include "os_wfopen.h"
#include "sciprint.h"
#include "freeArrayOfString.h"
#include "Scierror.h"
#include "scicurdir.h"
#include "md5.h"
#include "pathconvert.h"
}


xmlTextWriterPtr openXMLFile(const char* _pstFilename, const char* _pstLibName);
void closeXMLFile(xmlTextWriterPtr _pWriter);
bool AddMacroToXML(xmlTextWriterPtr _pWriter, const std::string& name, const std::string& file, const std::string& md5);


/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_genlib(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int succes = 1;
    std::vector<std::string> failed_files;
    std::vector<std::string> success_files;
    std::vector<std::string> funcs;

    int size = PATH_MAX + FILENAME_MAX;
    char pstParseFile[PATH_MAX + FILENAME_MAX];
    char pstVerbose[65535];

    int iNbFile = 0;
    char *pstParsePath = NULL;
    int iParsePathLen = 0;
    char* pstLibName = NULL;
    bool bVerbose = false;
    bool bForce = false;

    if (in.size() < 1 || in.size() > 4)
    {
        Scierror(78, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "genlib", 1, 4);
        return types::Function::Error;
    }

    //param 1, library name
    types::InternalType* pIT = in[0];
    if (pIT->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "genlib", 1);
        return types::Function::Error;
    }

    types::String *pS = pIT->getAs<types::String>();
    if (pS->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), "genlib", 1);
        return types::Function::Error;
    }
    pstLibName = pS->get(0);

    //param 2, library path
    if (in.size() > 1)
    {
        pIT = in[1];
        if (pIT->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "genlib", 2);
            return types::Function::Error;
        }
    }
    else
    {
        int ierr = 0;
        char* pstr = scigetcwd(&ierr);
        pIT = new types::String(pstr);
        FREE(pstr);
    }

    pS = pIT->getAs<types::String>();
    if (pS->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), "genlib", 2);
        return types::Function::Error;
    }

    //param 3, force flag
    if (in.size() > 2)
    {
        pIT = in[2];
        if (pIT->isBool() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar boolean expected.\n"), "genlib", 3);
            return types::Function::Error;
        }

        types::Bool* p = pIT->getAs<types::Bool>();
        if (p->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar boolean expected.\n"), "genlib", 3);
            return types::Function::Error;
        }

        bForce = p->get()[0] == 1;
    }

    if (in.size() > 3)
    {
        //verbose flag
        pIT = in[3];
        if (pIT->isBool() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar boolean expected.\n"), "genlib", 3);
            return types::Function::Error;
        }

        types::Bool* p = pIT->getAs<types::Bool>();
        if (p->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar boolean expected.\n"), "genlib", 3);
            return types::Function::Error;
        }

        bVerbose = p->get()[0] == 1;
    }

    char* pstFile = pS->get(0);
    pstParsePath = pathconvert(pstFile, TRUE, TRUE, AUTO_STYLE);

    if (in.size() == 1)
    {
        delete pS;
    }

    os_sprintf(pstParseFile, size, "%slib", pstParsePath);

    if (bVerbose)
    {
        os_sprintf(pstVerbose, 65535, _("-- Creation of [%s] (Macros) --\n"), pstLibName);

        //save current prompt mode
        int oldVal = ConfigVariable::getPromptMode();
        //set mode silent for errors
        ConfigVariable::setPromptMode(0);
        scilabWrite(pstVerbose);
        //restore previous prompt mode
        ConfigVariable::setPromptMode(oldVal);
    }

    MacroInfoList lstOld;
    if (FileExist(pstParseFile))
    {
        //read it to get previous information like md5
        std::string libname;
        parseLibFile(pstParseFile, lstOld, libname);
        deleteafile(pstParseFile);
    }

    xmlTextWriterPtr pWriter = openXMLFile(pstParseFile, pstLibName);

    if (pWriter == NULL)
    {
        os_sprintf(pstVerbose, 65535, _("%s: Cannot open file ''%s''.\n"), "genlib", pstParseFile);
        scilabWrite(pstVerbose);

        out.push_back(new types::Bool(0));
        FREE(pstParsePath);
        return types::Function::OK;
    }


    char** pstPath = findfiles(pstParsePath, "*.sci", &iNbFile, FALSE);

    if (pstPath)
    {
        types::Library* pLib = new types::Library(pstParsePath);
        for (int k = 0 ; k < iNbFile ; k++)
        {
            //version with direct parsing
            //parse the file to find all functions
            std::string stFullPath = std::string(pstParsePath) + std::string(FILE_SEPARATOR) + std::string(pstPath[k]);
            std::string stFullPathBin(stFullPath);
            stFullPathBin.replace(stFullPathBin.end() - 3, stFullPathBin.end(), "bin");
            std::string pstPathBin(pstPath[k]);
            pstPathBin.replace(pstPathBin.end() - 3, pstPathBin.end(), "bin");

            //compute file md5
            FILE* fmdf5 = fopen(stFullPath.data(), "rb");
            if (fmdf5 == NULL)
            {
                Scierror(999, _("%s: Cannot open file ''%s''.\n"), "genlib", stFullPath.data());
                pLib->killMe();
                return types::Function::Error;
            }

            char* md5 = md5_file(fmdf5);
            std::string s_md5(md5);
            FREE(md5);

            fclose(fmdf5);

            if (bForce == false)
            {
                //check if is exist in old file
                MacroInfoList::iterator it = lstOld.find(pstPathBin);
                if (it != lstOld.end())
                {
                    if (s_md5 == (*it).second.md5)
                    {
                        //file not change, we can skip it
                        AddMacroToXML(pWriter, (*it).second.name, pstPathBin, s_md5);
                        pLib->add((*it).second.name, new types::MacroFile((*it).second.name, stFullPathBin, pstLibName));
                        success_files.push_back(stFullPath);
                        funcs.push_back((*it).second.name);
                        continue;
                    }
                }
            }

            if (bVerbose)
            {
                sciprint(_("%s: Processing file: %s\n"), "genlib", pstPath[k]);
            }

            Parser parser;
            parser.parseFile(stFullPath, ConfigVariable::getSCIPath());
            if (parser.getExitStatus() !=  Parser::Succeded)
            {
                if (_iRetCount != 4)
                {
                    std::string strErr(parser.getErrorMessage());

                    char errmsg[256];
                    os_sprintf(errmsg, 256, _("%s: Error in file %s.\n"), "genlib", stFullPath.data());
                    strErr += errmsg;

                    Scierror(999, strErr.data());

                    FREE(pstParsePath);
                    freeArrayOfString(pstPath, iNbFile);
                    closeXMLFile(pWriter);
                    delete pLib;
                    return types::Function::Error;
                }

                failed_files.push_back(stFullPath);
                succes = 0;
                continue;
            }

            //serialize ast
            ast::SerializeVisitor* s = new ast::SerializeVisitor(parser.getTree());

            unsigned char* serialAst = s->serialize();
            // Header is : buffer size (4 bytes) + scilab version (4 bytes)
            unsigned int size = *((unsigned int*)serialAst);

            FILE* f = fopen(stFullPathBin.c_str(), "wb");
            fwrite(serialAst, 1, size, f);
            fclose(f);

            ast::exps_t LExp = parser.getTree()->getAs<ast::SeqExp>()->getExps();
            for (ast::exps_t::iterator j = LExp.begin(), itEnd = LExp.end() ; j != itEnd ; ++j)
            {
                if ((*j)->isFunctionDec())
                {
                    ast::FunctionDec* pFD = (*j)->getAs<ast::FunctionDec>();
                    const std::string& name = pFD->getSymbol().getName();
                    if (name + ".sci" == pstPath[k])
                    {
                        if (AddMacroToXML(pWriter, name, pstPathBin, s_md5) == false)
                        {
                            os_sprintf(pstVerbose, 65535, _("%s: Warning: %s information cannot be added to file %s. File ignored\n"), "genlib", pFD->getSymbol().getName().c_str(), pstPath[k]);
                            scilabWrite(pstVerbose);
                        }

                        pLib->add(name, new types::MacroFile(name, stFullPathBin, pstLibName));
                        success_files.push_back(stFullPath);
                        funcs.push_back(name);
                        break;
                    }
                }
            }

            delete s;
            free(serialAst);
            delete parser.getTree();
        }

        symbol::Context* ctx = symbol::Context::getInstance();
        symbol::Symbol sym = symbol::Symbol(pstLibName);
        if (ctx->isprotected(sym) == false)
        {
            ctx->put(symbol::Symbol(pstLibName), pLib);
        }
        else
        {
            Scierror(999, _("Redefining permanent variable.\n"));

            freeArrayOfString(pstPath, iNbFile);
            FREE(pstParsePath);
            closeXMLFile(pWriter);
            delete pLib;
            return types::Function::Error;
        }
    }

    freeArrayOfString(pstPath, iNbFile);

    out.push_back(new types::Bool(succes));

    if (_iRetCount > 1)
    {
        int size = static_cast<int>(funcs.size());
        if (size == 0)
        {
            out.push_back(types::Double::Empty());
        }
        else
        {
            types::String* s = new types::String(size, 1);

            for (int i = 0; i < size; ++i)
            {
                s->set(i, funcs[i].data());
            }

            out.push_back(s);
        }
    }

    if (_iRetCount > 2)
    {
        int size = static_cast<int>(success_files.size());
        if (size == 0)
        {
            out.push_back(types::Double::Empty());
        }
        else
        {
            types::String* s = new types::String(size, 1);

            for (int i = 0; i < size; ++i)
            {
                s->set(i, success_files[i].data());
            }

            out.push_back(s);
        }
    }

    if (_iRetCount > 3)
    {
        int size = static_cast<int>(failed_files.size());
        if (size == 0)
        {
            out.push_back(types::Double::Empty());
        }
        else
        {
            types::String* s = new types::String(size, 1);

            for (int i = 0; i < size; ++i)
            {
                s->set(i, failed_files[i].data());
            }

            out.push_back(s);
        }
    }

    FREE(pstParsePath);
    closeXMLFile(pWriter);
    return types::Function::OK;
}

void closeXMLFile(xmlTextWriterPtr _pWriter)
{
    int iLen;

    //close opened nodes
    iLen = xmlTextWriterEndElement(_pWriter);
    if (iLen < 0)
    {
        return;
    }

    //close document
    iLen = xmlTextWriterEndDocument(_pWriter);
    if (iLen < 0)
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
    if (pWriter == NULL)
    {
        return NULL;
    }

    //setup indentation
    xmlTextWriterSetIndent (pWriter, 1);
    xmlTextWriterSetIndentString (pWriter, (xmlChar*)"  ");

    //create a new document
    iLen = xmlTextWriterStartDocument(pWriter, NULL, DEFAULT_ENCODING, "no");
    if (iLen < 0)
    {
        closeXMLFile(pWriter);
        return NULL;
    }

    //add a node "scilablib"
    iLen = xmlTextWriterStartElement(pWriter, (xmlChar*)"scilablib");
    if (iLen < 0)
    {
        closeXMLFile(pWriter);
        return NULL;
    }

    //Add attribute "name"
    iLen = xmlTextWriterWriteAttribute(pWriter, (xmlChar*)"name", (xmlChar*)_pstLibName);
    if (iLen < 0)
    {
        closeXMLFile(pWriter);
        return NULL;
    }

    return pWriter;
}

bool AddMacroToXML(xmlTextWriterPtr _pWriter, const std::string& name, const std::string& file, const std::string& md5)
{
    int iLen;

    if (_pWriter == NULL)
    {
        return false;
    }

    //create node "macro"
    iLen = xmlTextWriterStartElement(_pWriter, (xmlChar*)"macro");
    if (iLen < 0)
    {
        return false;
    }

    //Add attribute "name"
    iLen = xmlTextWriterWriteAttribute(_pWriter, (xmlChar*)"name", (xmlChar*)name.data());
    if (iLen < 0)
    {
        return false;
    }

    //Add attribute "file"
    iLen = xmlTextWriterWriteAttribute(_pWriter, (xmlChar*)"file", (xmlChar*)file.data());
    if (iLen < 0)
    {
        return false;
    }

    //Add attribute "md5"
    iLen = xmlTextWriterWriteAttribute(_pWriter, (xmlChar*)"md5", (xmlChar*)md5.data());
    if (iLen < 0)
    {
        return false;
    }

    //close "macro" node
    iLen = xmlTextWriterEndElement(_pWriter);
    if (iLen < 0)
    {
        return false;
    }

    return true;
}
