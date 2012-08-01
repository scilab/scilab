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
static char * emptyAttribute = "";
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
    scilabPref.adaptToDisplay = NULL;
    scilabPref.columnsToDisplay = NULL;
    scilabPref.linesToDisplay = NULL;
    scilabPref.historySaveAfter = NULL;
    scilabPref.historyFile = NULL;
    scilabPref.historyLines = NULL;
    scilabPref.historyEnable = NULL;
    scilabPref.ieee = NULL;
    scilabPref.format = NULL;
    scilabPref.formatWidth = NULL;
    scilabPref.language = NULL;
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
        if (scilabPref.heapSize) FREE((void*)scilabPref.heapSize);
        if (scilabPref.adaptToDisplay) FREE((void*)scilabPref.adaptToDisplay);
        if (scilabPref.columnsToDisplay) FREE((void*)scilabPref.columnsToDisplay);
        if (scilabPref.linesToDisplay) FREE((void*)scilabPref.linesToDisplay);
        if (scilabPref.historySaveAfter) FREE((void*)scilabPref.historySaveAfter);
        if (scilabPref.historyFile) FREE((void*)scilabPref.historyFile);
        if (scilabPref.historyLines) FREE((void*)scilabPref.historyLines);
        if (scilabPref.historyEnable) FREE((void*)scilabPref.historyEnable);
        if (scilabPref.ieee) FREE((void*)scilabPref.ieee);
        if (scilabPref.format) FREE((void*)scilabPref.format);
        if (scilabPref.formatWidth) FREE((void*)scilabPref.formatWidth);
        if (scilabPref.language) FREE((void*)scilabPref.language);
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
    char * attr = NULL;

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
            scilabPref.adaptToDisplay = strdup(getAttribute(doc, xpathCtxt, ADAPTTODISPLAY_XPATH));
            scilabPref.columnsToDisplay = strdup(getAttribute(doc, xpathCtxt, COLUMNSTODISPLAY_XPATH));
            scilabPref.linesToDisplay = strdup(getAttribute(doc, xpathCtxt, LINESTODISPLAY_XPATH));
            scilabPref.historySaveAfter = strdup(getAttribute(doc, xpathCtxt, HISTORYSAVEAFTER_XPATH));
            scilabPref.historyFile = strdup(getAttribute(doc, xpathCtxt, HISTORYFILE_XPATH));
            scilabPref.historyLines = strdup(getAttribute(doc, xpathCtxt, HISTORYLINES_XPATH));
            scilabPref.historyEnable = strdup(getAttribute(doc, xpathCtxt, HISTORYENABLE_XPATH));
            scilabPref.ieee = strdup(getAttribute(doc, xpathCtxt, IEEE_XPATH));
            scilabPref.format = strdup(getAttribute(doc, xpathCtxt, FORMAT_XPATH));
            scilabPref.formatWidth = strdup(getAttribute(doc, xpathCtxt, FORMATWIDTH_XPATH));
            scilabPref.language = strdup(getAttribute(doc, xpathCtxt, LANGUAGE_XPATH));

            xmlXPathFreeContext(xpathCtxt);
        }
        xmlFreeDoc(doc);

        isInit = 1;
    }
}
/*--------------------------------------------------------------------------*/
char * getAttribute(xmlDocPtr doc, xmlXPathContextPtr xpathCtxt, const char * xpath)
{
    char * value = emptyAttribute;
    xmlXPathObjectPtr xpathObj = xmlXPathEval((const xmlChar*)xpath, xpathCtxt);
    if (xpathObj && xpathObj->nodesetval && xpathObj->nodesetval->nodeMax)
    {
        value = (char *)((xmlAttrPtr)xpathObj->nodesetval->nodeTab[0])->children->content;
    }

    if (xpathObj) xmlXPathFreeObject(xpathObj);

    return value;
}
/*--------------------------------------------------------------------------*/
