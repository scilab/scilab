/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

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

static char *GetXmlFileEncoding(const std::string& _filename);

types::Library* loadlib(const std::string& _stXML, int* err, bool _isFile, bool _bAddInContext)
{
    types::Library* lib = NULL;

    char* pstPathLib = expandPathVariable(_stXML.c_str());

    char* pstTemp = (char*)MALLOC(sizeof(char) * (PATH_MAX * 2));
    get_full_path(pstTemp, pstPathLib, PATH_MAX * 2);
    FREE(pstPathLib);

    std::string stOriginalPath(pstTemp);
    std::string stFile(pstTemp);
    std::string stPath(pstTemp);
    FREE(pstTemp);

    if (_isFile)
    {
        //remove / or \ at the end
        size_t pos = stPath.find_last_of("/\\");
        stPath = stPath.substr(0, pos);
        pos = stOriginalPath.find_last_of("/\\");
        stOriginalPath = stOriginalPath.substr(0, pos + 1); //with ending /
    }
    else
    {
        if (stFile.empty() == false && *stFile.rbegin() != DIR_SEPARATOR[0])
        {
            stFile += DIR_SEPARATOR;
        }

        stFile += "lib";
    }

    std::string libname;
    MacroInfoList lst;
    *err = parseLibFile(stFile, lst, libname);
    if (*err)
    {
        return lib;
    }

    lib = new types::Library(stOriginalPath);

    std::string stFilename(stPath);
    if (stFilename.empty() == false && *stFilename.rbegin() != DIR_SEPARATOR[0])
    {
        stFilename += DIR_SEPARATOR;
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

int parseLibFile(const std::string& _stXML, MacroInfoList& info, std::string& libname)
{
    info.clear();

    if (FileExist(_stXML.data()) == FALSE)
    {
        return 1;
    }

    char *encoding = GetXmlFileEncoding(_stXML);

    /* Don't care about line return / empty line */
    xmlKeepBlanksDefault(0);
    /* check if the XML file has been encoded with utf8 (unicode) or not */
    if (stricmp("utf-8", encoding))
    {
        free(encoding);
        return NULL;
    }

    xmlDocPtr doc;
    xmlXPathContextPtr xpathCtxt = NULL;
    xmlXPathObjectPtr xpathObj = NULL;
    std::string stName;
    std::string stLibName;
    std::string stFileName;
    std::string stMd5;

    free(encoding);

    doc = xmlParseFile(_stXML.data());

    if (doc == NULL)
    {
        return 1;
    }

    xpathCtxt = xmlXPathNewContext(doc);
    xpathObj = xmlXPathEval((const xmlChar*)"//scilablib", xpathCtxt);
    if (xpathObj && xpathObj->nodesetval->nodeMax)
    {
        xmlAttrPtr attrib = xpathObj->nodesetval->nodeTab[0]->properties;
        if (xmlStrEqual(attrib->name, (const xmlChar*)"name"))
        {
            /* we found the tag name */
            stLibName = (const char*)attrib->children->content;
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
                    stName = (const char*)attrib->children->content;
                }
                else if (xmlStrEqual(attrib->name, (const xmlChar*)"file"))
                {
                    /* we found the tag activate */
                    stFileName = (const char*)attrib->children->content;
                }
                else if (xmlStrEqual(attrib->name, (const xmlChar*)"md5"))
                {
                    /* we found the tag activate */
                    stMd5 = (const char*)attrib->children->content;
                }
                attrib = attrib->next;
            }

            if (stName.empty() == false && stFileName.empty() == false && stMd5.empty() == false)
            {
                info[stFileName] = MacroInfo(stName, stFileName, stMd5);
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

static char *GetXmlFileEncoding(const std::string& _filename)
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
