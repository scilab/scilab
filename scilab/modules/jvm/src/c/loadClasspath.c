/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) 2008-2008 - INRIA - Sylvestre LEDRU
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

/*--------------------------------------------------------------------------*/
#include <libxml/xpath.h>
#include <libxml/xmlreader.h>
#include <stdio.h>
#include <string.h>
#include "loadClasspath.h"
#include "GetXmlFileEncoding.h"
#include "FileExist.h"
#include "addToClasspath.h"
#include "sci_path.h"
#include "sci_malloc.h"
#include "localization.h"
#include "configvariable_interface.h"
#include "os_string.h"
#include "getshortpathname.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
static xmlDocPtr ClassPathxmlDocPtr = NULL;
/*--------------------------------------------------------------------------*/
xmlDocPtr getClassPathxmlDocPtr(void)
{
    return ClassPathxmlDocPtr;
}
/*--------------------------------------------------------------------------*/
void freeClassPathxmlDocPtr(void)
{
    if (ClassPathxmlDocPtr)
    {
        xmlFreeDoc (ClassPathxmlDocPtr);
        ClassPathxmlDocPtr = NULL;
    }
}
/*--------------------------------------------------------------------------*/
BOOL LoadClasspath(char *xmlfilename)
{
    BOOL bOK = FALSE;
    BOOL errorOnLoad = FALSE;
    if ( FileExist(xmlfilename) )
    {
        char *encoding = GetXmlFileEncoding(xmlfilename);

        /* Don't care about line return / empty line */
        xmlKeepBlanksDefault(0);
        /* check if the XML file has been encoded with utf8 (unicode) or not */
        if ( stricmp("utf-8", encoding) == 0 )
        {
            xmlXPathContextPtr xpathCtxt = NULL;
            xmlXPathObjectPtr xpathObj = NULL;
            char *classpath = NULL;
            char *load = "";
            typeOfLoad eLoad = STARTUP;
            const char *currentMode = getScilabModeString();
            /* Xpath Query :
             * Retrieve all the path which are not disabled in our mode
             */
#define XPATH "//classpaths/path[not(@disableUnderMode='%s')]"
            char * XPath = (char*)MALLOC(sizeof(char) * (strlen(XPATH) + strlen(currentMode) - 2 + 1)); /* -2 = strlen(%s) */
            sprintf(XPath, XPATH, currentMode);

            {
                BOOL bConvert = FALSE;
                char *shortxmlfilename = getshortpathname(xmlfilename, &bConvert);
                if (shortxmlfilename)
                {
                    ClassPathxmlDocPtr = xmlParseFile (shortxmlfilename);
                    FREE(shortxmlfilename);
                    shortxmlfilename = NULL;
                }
            }

            if (ClassPathxmlDocPtr == NULL)
            {
                fprintf(stderr, _("Error: could not parse file %s\n"), xmlfilename);
                FREE(encoding);
                encoding = NULL;
                FREE(XPath);
                return bOK;
            }

            xpathCtxt = xmlXPathNewContext(ClassPathxmlDocPtr);
            xpathObj = xmlXPathEval((const xmlChar*)XPath, xpathCtxt);

            if (xpathObj && xpathObj->nodesetval->nodeMax)
            {
                /* the Xpath has been understood and there are node */
                int	i;
                for (i = 0; i < xpathObj->nodesetval->nodeNr; i++)
                {

                    xmlAttrPtr attrib = xpathObj->nodesetval->nodeTab[i]->properties;
                    /* Get the properties of <path>  */
                    while (attrib != NULL)
                    {
                        /* loop until when have read all the attributes */
                        if (xmlStrEqual (attrib->name, (const xmlChar*) "value"))
                        {
                            /* we found the tag value */
                            classpath = (char*)attrib->children->content;
                        }
                        if (xmlStrEqual (attrib->name, (const xmlChar*) "load"))
                        {
                            /* we found the tag load */
                            load = (char*)attrib->children->content;

                            /* By default, it is startup */
                            if (stricmp(load, "background") == 0)
                            {
                                eLoad = BACKGROUND;
                            }
                            else
                            {
                                if (stricmp(load, "onuse") == 0)
                                {
                                    eLoad = ONUSE;
                                }
                            }
                        }
                        else
                        {
                            eLoad = STARTUP;
                        }
                        attrib = attrib->next;
                    }

                    if ( (classpath) && (strlen(classpath) > 0) && (strncmp(classpath, "@", 1) != 0) ) /* If it starts by a @ that means it hasn't been able to find it... which is normal... for example with the documentation */
                    {
#define KEYWORDSCILAB "$SCILAB"
                        char *sciPath = getSCI();
                        char *FullClasspath = NULL;

                        if (strncmp(classpath, KEYWORDSCILAB, strlen(KEYWORDSCILAB)) == 0)
                        {
                            FullClasspath = (char*)MALLOC(sizeof(char) * (strlen(sciPath) + strlen(classpath) + 1));
                            if (FullClasspath)
                            {
                                strcpy(FullClasspath, sciPath);
                                strcat(FullClasspath, &classpath[strlen(KEYWORDSCILAB)]);
                            }
                        }
                        else
                        {
                            FullClasspath = os_strdup(classpath);
                        }

                        if (FullClasspath)
                        {
                            if (!addToClasspath(FullClasspath, eLoad))
                            {
                                errorOnLoad = TRUE;
                            }
                            FREE(FullClasspath);
                            FullClasspath = NULL;
                        }

                        if (sciPath)
                        {
                            FREE(sciPath);
                            sciPath = NULL;
                        }
                        classpath = NULL;
                    }
                }
                bOK = TRUE;
            }
            else
            {
                fprintf(stderr, _("Wrong format for %s.\n"), xmlfilename);
            }

            if (xpathObj)
            {
                xmlXPathFreeObject(xpathObj);
            }
            if (xpathCtxt)
            {
                xmlXPathFreeContext(xpathCtxt);
            }
            FREE(XPath);
            XPath = NULL;
        }
        else
        {
            fprintf(stderr, _("Error : Not a valid classpath file %s (encoding not 'utf-8') Encoding '%s' found\n"), xmlfilename, encoding);
        }
        FREE(encoding);
        encoding = NULL;
    }
    else
    {
        fprintf(stderr, _("Warning: could not find classpath declaration file %s.\n"), xmlfilename);
    }
    if (errorOnLoad)
    {
        fprintf(stderr, _("Some problems during the loading of the Java libraries occurred.\nThis could lead to inconsistent behaviours.\nPlease check SCI/etc/classpath.xml.\n"));
    }

    return bOK;
}
/*--------------------------------------------------------------------------*/
