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

extern "C"
{
#include "MALLOC.h"
#include "stricmp.h"
#include "os_strdup.h"
#include "expandPathVariable.h"
#include "PATH_MAX.h"
#include <libxml/xpath.h>
#include <libxml/xmlreader.h>
}

#define DEFAULT_ENCODING "UTF-8"

static char *GetXmlFileEncoding(string _filename);

types::Library* loadlib(std::wstring _wstXML, bool _isFile, bool _bAddInContext)
{
    types::Library* lib = NULL;

    wstring wstFile(_wstXML);
    wstring wstPath(_wstXML);

    if (_isFile)
    {
        size_t pos = wstPath.find_last_of(L"/\\");
        wstPath = wstPath.substr(0, pos);
    }
    else
    {
        if (wstFile.empty() == false && *wstFile.rbegin() != DIR_SEPARATORW[0])
        {
            wstFile += DIR_SEPARATORW;
        }

        wstFile += L"lib";
    }

    char* pstFile = wide_string_to_UTF8(wstFile.c_str());
    char *encoding = GetXmlFileEncoding(pstFile);

    /* Don't care about line return / empty line */
    xmlKeepBlanksDefault(0);
    /* check if the XML file has been encoded with utf8 (unicode) or not */
    if (stricmp("utf-8", encoding))
    {
        std::wcout << "Error: Not a valid module file " << pstFile << " (encoding not 'utf-8') Encoding '" << encoding << "' found." << std::endl;
        FREE(pstFile);
        free(encoding);
        return NULL;
    }

    xmlDocPtr doc;
    xmlXPathContextPtr xpathCtxt    = NULL;
    xmlXPathObjectPtr xpathObj      = NULL;
    wchar_t* pstName                = NULL;
    wchar_t* pstLibName             = NULL;
    wchar_t* pstFileName            = NULL;

    free(encoding);

    doc = xmlParseFile(pstFile);

    if (doc == NULL)
    {
        std::cout << "Error: Could not parse file " << pstFile << std::endl;
        FREE(pstFile);
        return NULL;
    }

    FREE(pstFile);

    lib = new types::Library(wstPath);

    xpathCtxt = xmlXPathNewContext(doc);
    xpathObj = xmlXPathEval((const xmlChar*)"//scilablib", xpathCtxt);
    if (xpathObj && xpathObj->nodesetval->nodeMax)
    {
        xmlAttrPtr attrib = xpathObj->nodesetval->nodeTab[0]->properties;
        if (xmlStrEqual (attrib->name, (const xmlChar*)"name"))
        {
            /* we found the tag name */
            const char *str = (const char*)attrib->children->content;
            pstLibName = to_wide_string(str);
        }
        else
        {
            return NULL;
        }
    }

    xpathObj = xmlXPathEval((const xmlChar*)"//scilablib/macro", xpathCtxt);
    if (xpathObj && xpathObj->nodesetval->nodeMax)
    {
        /* the Xpath has been understood and there are node */
        for (int i = 0 ; i < xpathObj->nodesetval->nodeNr ; i++)
        {
            xmlAttrPtr attrib = xpathObj->nodesetval->nodeTab[i]->properties;
            /* Get the properties of <module>  */
            while (attrib != NULL)
            {
                /* loop until when have read all the attributes */
                if (xmlStrEqual (attrib->name, (const xmlChar*)"name"))
                {
                    /* we found the tag name */
                    const char *str = (const char*)attrib->children->content;
                    pstName = to_wide_string(str);
                }
                else if (xmlStrEqual(attrib->name, (const xmlChar*)"file"))
                {
                    /* we found the tag activate */
                    const char *str = (const char*)attrib->children->content;
                    pstFileName = to_wide_string(str);
                }
                attrib = attrib->next;
            }

            if (pstName && pstFileName)
            {
                wstring stFilename(wstPath);
                if (stFilename.empty() == false && *stFilename.rbegin() != DIR_SEPARATORW[0])
                {
                    stFilename += DIR_SEPARATORW;
                }

                stFilename += pstFileName;
                lib->add(pstName, new types::MacroFile(pstName, stFilename, pstLibName));
            }
            else
            {
                std::cout << pstName << " module not found." << std::endl;
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

    if (_bAddInContext)
    {
        symbol::Context::getInstance()->put(symbol::Symbol(pstLibName), *lib);
    }

    xmlFreeDoc(doc);

    return lib;
}

static char *GetXmlFileEncoding(string _filename)
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
