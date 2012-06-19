/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include <libxml/xpath.h>
#include <libxml/xmlreader.h>
#include "getScilabPreference.h"
#include "GetXmlFileEncoding.h"
#include "MALLOC.h"
#include "FileExist.h"
#include "stricmp.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "getshortpathname.h"
#include "BOOL.h"
#include "SCIHOME.h"

#define XCONF "%s/XConfiguration.xml"

/*--------------------------------------------------------------------------*/
static unsigned char isInit = 0;
static ScilabPreferences scilabPref;
/*--------------------------------------------------------------------------*/
static void getPrefs();
static char * getAttribute(xmlDocPtr doc, xmlXPathContextPtr xpathCtxt, const char * xpath);
static void initPrefs();
/*--------------------------------------------------------------------------*/
const ScilabPreferences * getScilabPreferences()
{
    getPrefs();
    return &scilabPref;
}
/*--------------------------------------------------------------------------*/
void initPrefs()
{
    scilabPref.heapSize = NULL;
    scilabPref.columnsToDisplay = NULL;
    scilabPref.linesToDisplay = NULL;
    scilabPref.historySaveAfter = NULL;
    scilabPref.historyFile = NULL;
    scilabPref.historyLines = NULL;
}
/*--------------------------------------------------------------------------*/
void reloadScilabPreferences()
{
    clearScilabPreferences();
    getPrefs();
}
/*--------------------------------------------------------------------------*/
void clearScilabPreferences()
{
    if (isInit == 1)
    {
        FREE(scilabPref.heapSize);
        FREE(scilabPref.columnsToDisplay);
        FREE(scilabPref.linesToDisplay);
        FREE(scilabPref.historySaveAfter);
        FREE(scilabPref.historyFile);
        FREE(scilabPref.historyLines);
        initPrefs();
    }
    isInit = 0;
}
/*--------------------------------------------------------------------------*/
void getPrefs()
{
    xmlDocPtr doc = NULL;
    xmlXPathContextPtr xpathCtxt = NULL;
    char * SCIHOME = NULL;
    char * path = NULL;
    BOOL bConvert = FALSE;
    char * shortfilename_xml_conf = NULL;

    if (!isInit)
    {
        initPrefs();

        SCIHOME = getSCIHOME();
        path = (char *)MALLOC(strlen(SCIHOME) + strlen(XCONF));

        sprintf(path, XCONF, SCIHOME);

        if (FileExist(path))
        {
            shortfilename_xml_conf = getshortpathname(path, &bConvert);
            if (shortfilename_xml_conf)
            {
                doc = xmlParseFile(shortfilename_xml_conf);
                FREE(shortfilename_xml_conf);
                shortfilename_xml_conf = NULL;
            }
            FREE(path);
            path = NULL;
        }

        if (doc == NULL)
        {
            return;
        }

        if (stricmp(doc->encoding, "utf-8"))
        {
            return;
        }

        xpathCtxt = xmlXPathNewContext(doc);

        if (xpathCtxt)
        {
            scilabPref.heapSize = strdup(getAttribute(doc, xpathCtxt, HEAPSIZE_XPATH));
            scilabPref.columnsToDisplay = strdup(getAttribute(doc, xpathCtxt, COLUMNSTODISPLAY_XPATH));
            scilabPref.linesToDisplay = strdup(getAttribute(doc, xpathCtxt, LINESTODISPLAY_XPATH));
            scilabPref.historySaveAfter = strdup(getAttribute(doc, xpathCtxt, HISTORYSAVEAFTER_XPATH));
            scilabPref.historyFile = strdup(getAttribute(doc, xpathCtxt, HISTORYFILE_XPATH));
            scilabPref.historyLines = strdup(getAttribute(doc, xpathCtxt, HISTORYLINES_XPATH));

            xmlXPathFreeContext(xpathCtxt);
        }
        xmlFreeDoc(doc);

        isInit = 1;
    }
}
/*--------------------------------------------------------------------------*/
char * getAttribute(xmlDocPtr doc, xmlXPathContextPtr xpathCtxt, const char * xpath)
{
    char * value = NULL;
    xmlXPathObjectPtr xpathObj = xmlXPathEval((const xmlChar*)xpath, xpathCtxt);
    if (xpathObj && xpathObj->nodesetval->nodeMax)
    {
        value = (char *)((xmlAttrPtr)xpathObj->nodesetval->nodeTab[0])->children->content;
    }

    if (xpathObj) xmlXPathFreeObject(xpathObj);

    return value;
}
/*--------------------------------------------------------------------------*/
