/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
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

#ifdef _MSC_VER
#pragma warning(disable : 4996) //It's not beautifull but that works !
#endif

#define DEFAULT_ENCODING "UTF-8"

#ifdef _MSC_VER
#define FILE_SEPARATOR L"\\"
#else
#define FILE_SEPARATOR L"/"
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


xmlTextWriterPtr openXMLFile(const wchar_t *_pstFilename, const wchar_t* _pstLibName);
void closeXMLFile(xmlTextWriterPtr _pWriter);
bool AddMacroToXML(xmlTextWriterPtr _pWriter, const std::wstring& name, const std::wstring& file, const std::wstring& md5);


/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_genlib(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int succes = 1;
    std::vector<std::wstring> failed_files;
    std::vector<std::wstring> success_files;
    std::vector<std::wstring> funcs;

    wchar_t pstParseFile[PATH_MAX + FILENAME_MAX];
    wchar_t pstVerbose[65535];

    int iNbFile = 0;
    wchar_t *pstParsePath = NULL;
    int iParsePathLen = 0;
    wchar_t* pstLibName = NULL;
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

    wchar_t* pstFile = pS->get(0);
    pstParsePath = pathconvertW(pstFile, TRUE, TRUE, AUTO_STYLE);

    if (in.size() == 1)
    {
        delete pS;
    }

    os_swprintf(pstParseFile, PATH_MAX + FILENAME_MAX, L"%lslib", pstParsePath);

    if (bVerbose)
    {
        os_swprintf(pstVerbose, 65535, _W("-- Creation of [%ls] (Macros) --\n").c_str(), pstLibName);

        //save current prompt mode
        int oldVal = ConfigVariable::getPromptMode();
        //set mode silent for errors
        ConfigVariable::setPromptMode(0);
        scilabWriteW(pstVerbose);
        //restore previous prompt mode
        ConfigVariable::setPromptMode(oldVal);
    }

    MacroInfoList lstOld;
    if (FileExistW(pstParseFile))
    {
        //read it to get previous information like md5
        std::wstring libname;
        parseLibFile(pstParseFile, lstOld, libname);
        deleteafileW(pstParseFile);
    }

    xmlTextWriterPtr pWriter = openXMLFile(pstParseFile, pstLibName);

    if (pWriter == NULL)
    {
        os_swprintf(pstVerbose, 65535, _W("%ls: Cannot open file ''%ls''.\n").c_str(), L"genlib", pstParseFile);
        scilabWriteW(pstVerbose);

        out.push_back(new types::Bool(0));
        FREE(pstParsePath);
        return types::Function::OK;
    }


    wchar_t **pstPath = findfilesW(pstParsePath, L"*.sci", &iNbFile, FALSE);

    if (pstPath)
    {
        types::Library* pLib = new types::Library(pstParsePath);
        for (int k = 0 ; k < iNbFile ; k++)
        {
            //version with direct parsing
            //parse the file to find all functions
            std::wstring stFullPath = std::wstring(pstParsePath) + std::wstring(pstPath[k]);
            std::wstring stFullPathBin(stFullPath);
            stFullPathBin.replace(stFullPathBin.end() - 3, stFullPathBin.end(), L"bin");
            std::wstring pstPathBin(pstPath[k]);
            pstPathBin.replace(pstPathBin.end() - 3, pstPathBin.end(), L"bin");

            //compute file md5
            FILE* fmdf5 = os_wfopen(stFullPath.data(), L"rb");
            if (fmdf5 == NULL)
            {
                char* pstr = wide_string_to_UTF8(stFullPath.data());
                Scierror(999, _("%s: Cannot open file ''%s''.\n"), "genlib", pstr);
                FREE(pstr);
                FREE(pstParsePath);
                freeArrayOfWideString(pstPath, iNbFile);
                pLib->killMe();
                return types::Function::Error;
            }

            char* md5 = md5_file(fmdf5);
            fclose(fmdf5);

            wchar_t* wmd5 = to_wide_string(md5);
            FREE(md5);
            std::wstring wide_md5(wmd5);
            FREE(wmd5);

            if (bForce == false)
            {
                //check if is exist in old file
                MacroInfoList::iterator it = lstOld.find(pstPathBin);
                if (it != lstOld.end())
                {
                    if (wide_md5 == (*it).second.md5)
                    {
                        //file not change, we can skip it
                        AddMacroToXML(pWriter, (*it).second.name, pstPathBin, wide_md5);
                        pLib->add((*it).second.name, new types::MacroFile((*it).second.name, stFullPathBin, pstLibName));
                        success_files.push_back(stFullPath);
                        funcs.push_back((*it).second.name);
                        continue;
                    }
                }
            }

            if (bVerbose)
            {
                sciprint(_("%ls: Processing file: %ls\n"), L"genlib", pstPath[k]);
            }

            Parser parser;
            parser.parseFile(stFullPath, ConfigVariable::getSCIPath());
            if (parser.getExitStatus() !=  Parser::Succeded)
            {
                if (_iRetCount != 4)
                {
                    std::wstring wstrErr = parser.getErrorMessage();

                    wchar_t errmsg[256];
                    os_swprintf(errmsg, 256, _W("%ls: Error in file %ls.\n").c_str(), L"genlib", stFullPath.data());
                    wstrErr += errmsg;

                    char* str = wide_string_to_UTF8(wstrErr.c_str());
                    Scierror(999, str);
                    FREE(str);

                    FREE(pstParsePath);
                    freeArrayOfWideString(pstPath, iNbFile);
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

            FILE* f = os_wfopen(stFullPathBin.c_str(), L"wb");
            fwrite(serialAst, 1, size, f);
            fclose(f);

            ast::exps_t LExp = parser.getTree()->getAs<ast::SeqExp>()->getExps();
            for (ast::exps_t::iterator j = LExp.begin(), itEnd = LExp.end() ; j != itEnd ; ++j)
            {
                if ((*j)->isFunctionDec())
                {
                    ast::FunctionDec* pFD = (*j)->getAs<ast::FunctionDec>();
                    const std::wstring& name = pFD->getSymbol().getName();
                    if (name + L".sci" == pstPath[k])
                    {
                        if (AddMacroToXML(pWriter, name, pstPathBin, wide_md5) == false)
                        {
                            os_swprintf(pstVerbose, 65535, _W("%ls: Warning: %ls information cannot be added to file %ls. File ignored\n").c_str(), L"genlib", pFD->getSymbol().getName().c_str(), pstPath[k]);
                            scilabWriteW(pstVerbose);
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

            freeArrayOfWideString(pstPath, iNbFile);
            FREE(pstParsePath);
            closeXMLFile(pWriter);
            delete pLib;
            return types::Function::Error;
        }
    }

    freeArrayOfWideString(pstPath, iNbFile);

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

xmlTextWriterPtr openXMLFile(const wchar_t *_pstFilename, const wchar_t* _pstLibName)
{
    int iLen;
    xmlTextWriterPtr pWriter = NULL;;
    char *pstFilename = wide_string_to_UTF8(_pstFilename);
    char *pstLibName = wide_string_to_UTF8(_pstLibName);


    //create a writer
    pWriter = xmlNewTextWriterFilename(pstFilename, 0);
    if (pWriter == NULL)
    {
        FREE(pstFilename);
        FREE(pstLibName);
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
        FREE(pstFilename);
        FREE(pstLibName);
        return NULL;
    }

    //add a node "scilablib"
    iLen = xmlTextWriterStartElement(pWriter, (xmlChar*)"scilablib");
    if (iLen < 0)
    {
        closeXMLFile(pWriter);
        FREE(pstFilename);
        FREE(pstLibName);
        return NULL;
    }

    //Add attribute "name"
    iLen = xmlTextWriterWriteAttribute(pWriter, (xmlChar*)"name", (xmlChar*)pstLibName);
    if (iLen < 0)
    {
        closeXMLFile(pWriter);
        FREE(pstFilename);
        FREE(pstLibName);
        return NULL;
    }

    FREE(pstFilename);
    FREE(pstLibName);

    return pWriter;
}

bool AddMacroToXML(xmlTextWriterPtr _pWriter, const std::wstring& name, const std::wstring& file, const std::wstring& md5)
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
    char* pst1 = wide_string_to_UTF8(name.data());
    iLen = xmlTextWriterWriteAttribute(_pWriter, (xmlChar*)"name", (xmlChar*)pst1);
    if (iLen < 0)
    {
        return false;
    }
    FREE(pst1);

    //Add attribute "file"
    char* pst2 = wide_string_to_UTF8(file.data());
    iLen = xmlTextWriterWriteAttribute(_pWriter, (xmlChar*)"file", (xmlChar*)pst2);
    if (iLen < 0)
    {
        return false;
    }
    FREE(pst2);

    //Add attribute "md5"
    char* pst3 = wide_string_to_UTF8(md5.data());
    iLen = xmlTextWriterWriteAttribute(_pWriter, (xmlChar*)"md5", (xmlChar*)pst3);
    if (iLen < 0)
    {
        return false;
    }
    FREE(pst3);

    //close "macro" node
    iLen = xmlTextWriterEndElement(_pWriter);
    if (iLen < 0)
    {
        return false;
    }

    return true;
}
