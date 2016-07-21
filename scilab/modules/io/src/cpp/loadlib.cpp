/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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
#include <fstream>
#include "configvariable.hxx"
#include "context.hxx"
#include "loadlib.hxx"
#include "macrofile.hxx"
extern "C"
{
#include "FileExist.h"
#include "sci_malloc.h"
#include "os_string.h"
#include "expandPathVariable.h"
#include "fullpath.h"
#include "PATH_MAX.h"
#include <libxml/xpath.h>
#include <libxml/xmlreader.h>
}

#define DEFAULT_ENCODING "UTF-8"

static char *GetXmlFileEncoding(std::string _filename);

types::Library* loadlib(const std::wstring& _wstXML, int* err, bool _isFile, bool _bAddInContext)
{
    types::Library* lib = NULL;

    wchar_t* pwstPathLib = expandPathVariableW((wchar_t*)_wstXML.c_str());

    wchar_t* pwstTemp = (wchar_t*)MALLOC(sizeof(wchar_t) * (PATH_MAX * 2));
    get_full_pathW(pwstTemp, pwstPathLib, PATH_MAX * 2);
    FREE(pwstPathLib);

    std::wstring wstOriginalPath(_wstXML);
    std::wstring wstFile(pwstTemp);
    std::wstring wstPath(pwstTemp);
    FREE(pwstTemp);

    if (_isFile)
    {
        //remove / or \ at the end
        size_t pos = wstPath.find_last_of(L"/\\");
        wstPath = wstPath.substr(0, pos);
        pos = wstOriginalPath.find_last_of(L"/\\");
        wstOriginalPath = wstOriginalPath.substr(0, pos + 1); //with ending /
    }
    else
    {
        if (wstFile.empty() == false && *wstFile.rbegin() != DIR_SEPARATORW[0])
        {
            wstFile += DIR_SEPARATORW;
        }

        wstFile += L"lib";
    }

    std::wstring libname;
    MacroInfoList lst;
    *err = parseLibFile(wstFile, lst, libname);
    if (*err)
    {
        return lib;
    }

    lib = new types::Library(wstOriginalPath);

    std::wstring stFilename(wstPath);
    if (stFilename.empty() == false && *stFilename.rbegin() != DIR_SEPARATORW[0])
    {
        stFilename += DIR_SEPARATORW;
    }


    for (const auto & macro : lst)
    {
        lib->add(macro.second.name, new types::MacroFile(macro.second.name, stFilename + macro.second.file, libname));
    }


    if (_bAddInContext)
    {
        symbol::Context* ctx = symbol::Context::getInstance();
        symbol::Symbol sym = symbol::Symbol(libname);
        if (ctx->isprotected(sym) == false)
        {
            ctx->put(sym, lib);
        }
        else
        {
            *err = 2;
            delete lib;
            lib = NULL;
        }
    }

    return lib;
}

int parseLibFile(const std::wstring& _wstXML, MacroInfoList& info, std::wstring& libname)
{
    info.clear();

    char* pstFile = wide_string_to_UTF8(_wstXML.data());

    if (FileExist(pstFile) == FALSE)
    {
        FREE(pstFile);
        return 1;
    }
    std::string s(_wstXML.begin(),_wstXML.end());
    std::ifstream file(s);
    if (file)
    {
        const std::string XMLDecl("<?xml");
        std::string readXMLDecl;
        readXMLDecl.resize(XMLDecl.length(),' ');//reserve space
        file.read(&*readXMLDecl.begin(),XMLDecl.length());
        if (XMLDecl != readXMLDecl)
        {
          FREE(pstFile);
          return 4;
        }
    }

    char *encoding = GetXmlFileEncoding(pstFile);

    /* Don't care about line return / empty line */
    xmlKeepBlanksDefault(0);
    /* check if the XML file has been encoded with utf8 (unicode) or not */
    if (stricmp("utf-8", encoding))
    {
        FREE(pstFile);
        free(encoding);
        return 3;
    }

    xmlDocPtr doc;
    xmlXPathContextPtr xpathCtxt = NULL;
    xmlXPathObjectPtr xpathObj = NULL;
    wchar_t* pstName = NULL;
    wchar_t* pstLibName = NULL;
    wchar_t* pstFileName = NULL;
    wchar_t* pstMd5 = NULL;

    free(encoding);

    doc = xmlParseFile(pstFile);

    if (doc == NULL)
    {
        FREE(pstFile);
        return 3;
    }

    FREE(pstFile);

    xpathCtxt = xmlXPathNewContext(doc);
    xpathObj = xmlXPathEval((const xmlChar*)"//scilablib", xpathCtxt);
    if (xpathObj && xpathObj->nodesetval->nodeMax)
    {
        xmlAttrPtr attrib = xpathObj->nodesetval->nodeTab[0]->properties;
        if (xmlStrEqual(attrib->name, (const xmlChar*)"name"))
        {
            /* we found the tag name */
            const char *str = (const char*)attrib->children->content;
            pstLibName = to_wide_string(str);
            libname = pstLibName;
            FREE(pstLibName);
            xmlXPathFreeObject(xpathObj);
        }
        else
        {
            if (xpathCtxt)
            {
                xmlXPathFreeContext(xpathCtxt);
            }
            xmlXPathFreeObject(xpathObj);
            return 1;
        }
    }

    xpathObj = xmlXPathEval((const xmlChar*)"//scilablib/macro", xpathCtxt);
    if (xpathObj && xpathObj->nodesetval->nodeMax)
    {
        /* the Xpath has been understood and there are node */
        for (int i = 0; i < xpathObj->nodesetval->nodeNr; i++)
        {
            xmlAttrPtr attrib = xpathObj->nodesetval->nodeTab[i]->properties;
            /* Get the properties of <module>  */
            while (attrib != NULL)
            {
                /* loop until when have read all the attributes */
                if (xmlStrEqual(attrib->name, (const xmlChar*)"name"))
                {
                    /* we found the tag name */
                    const char *str = (const char*)attrib->children->content;
                    if (pstName)
                    {
                        FREE(pstName);
                    }
                    pstName = to_wide_string(str);
                }
                else if (xmlStrEqual(attrib->name, (const xmlChar*)"file"))
                {
                    /* we found the tag activate */
                    const char *str = (const char*)attrib->children->content;
                    if (pstFileName)
                    {
                        FREE(pstFileName);
                    }
                    pstFileName = to_wide_string(str);
                }
                else if (xmlStrEqual(attrib->name, (const xmlChar*)"md5"))
                {
                    /* we found the tag activate */
                    const char *str = (const char*)attrib->children->content;
                    if (pstMd5)
                    {
                        FREE(pstMd5);
                    }
                    pstMd5 = to_wide_string(str);
                }
                attrib = attrib->next;
            }

            if (pstName && pstFileName && pstMd5)
            {
                info[pstFileName] = MacroInfo(pstName, pstFileName, pstMd5);
            }

            if (pstName)
            {
                FREE(pstName);
                pstName = NULL;
            }

            if (pstFileName)
            {
                FREE(pstFileName);
                pstFileName = NULL;
            }

            if (pstMd5)
            {
                FREE(pstMd5);
                pstMd5 = NULL;
            }
        }
    }

    if (xpathObj)
    {
        xmlXPathFreeObject(xpathObj);
    }
    if (xpathCtxt)
    {
        xmlXPathFreeContext(xpathCtxt);
    }

    xmlFreeDoc(doc);
    return 0;
}

static char *GetXmlFileEncoding(std::string _filename)
{
    char *encoding = NULL;
    xmlDocPtr doc = NULL;

    /* default */
    encoding = os_strdup(DEFAULT_ENCODING);

    doc = xmlParseFile(_filename.c_str());
    if (doc)
    {
        if (doc->encoding)
        {
            if (encoding)
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
