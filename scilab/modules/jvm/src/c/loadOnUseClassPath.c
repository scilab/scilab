/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
#include <jni.h>
#include <stdio.h>
#include <libxml/xpath.h>
#include <libxml/xmlreader.h>
#include "sci_malloc.h"
#include "scilabDefaults.h"
#include "getScilabJNIEnv.h"
#include "localization.h"
#include "sci_path.h"
#include "addToClasspath.h"
#include "loadOnUseClassPath.h"
#include "loadClasspath.h"
#include "FileExist.h"
#include "GetXmlFileEncoding.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
BOOL loadOnUseClassPath(char const* tag)
{
    BOOL bOK = FALSE;
    char *sciPath = getSCI();

    char *classpathfile = (char*)MALLOC(sizeof(char) * (strlen(sciPath) + strlen(XMLCLASSPATH) + 1));

    sprintf(classpathfile, XMLCLASSPATH, sciPath);

    // Load the XML
    if ( FileExist(classpathfile) )
    {
        xmlXPathContextPtr xpathCtxt = NULL;
        xmlXPathObjectPtr xpathObj = NULL;
        xmlDocPtr doc = NULL;

        char * XPath = NULL;

        /*
         * Xpath query: Retrieve all nodes what have the property
         * load="onUse" and the tag we are looking for
         */
#define XPATH "//classpaths/path[@load='onUse']/load[@on='%s']"
        XPath = (char*)MALLOC(sizeof(char) * (strlen(XPATH) + strlen(tag) - 2 + 1)); /* -2 = strlen(%s) */
        sprintf(XPath, XPATH, tag);

        doc = getClassPathxmlDocPtr();

        if (doc == NULL)
        {
            fprintf(stderr, _("Error: could not parse file %s\n"), classpathfile);
            FREE(XPath);
            XPath = NULL;

            FREE(classpathfile);
            classpathfile = NULL;

            FREE(sciPath);
            return bOK;
        }

        xpathCtxt = xmlXPathNewContext(doc);
        // Look for all the tag which matches
        xpathObj = xmlXPathEval((const xmlChar*)XPath, xpathCtxt);
        if (xpathObj && xpathObj->nodesetval->nodeMax)
        {
            /* the Xpath has been understood and there are node */
            int	i;
            for (i = 0; i < xpathObj->nodesetval->nodeNr; i++)
            {
                xmlAttrPtr attrib = xpathObj->nodesetval->nodeTab[i]->parent->properties;
                while (attrib != NULL)
                {
                    /* loop until when have read all the attributes */
                    if (xmlStrEqual (attrib->name, (const xmlChar*) "value"))
                    {
                        // @TODO Check if it has been loaded before
                        /* we found the tag value  & load the jar */
                        /* replaces $SCILAB by current path of SCI */
#define KEYWORDSCILAB "$SCILAB"
                        char *FullClasspath = NULL;
                        char *classpath = (char*)attrib->children->content;

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
                        addToClasspath(FullClasspath, STARTUP);
                        FREE(FullClasspath);
                    }
                    attrib = attrib->next;
                }
            }
        }
        else
        {
            fprintf(stderr, _("Wrong format for %s.\n"), classpathfile);
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
        fprintf(stderr, _("Warning: could not find classpath declaration file %s.\n"), classpathfile);
    }

    FREE(classpathfile);
    classpathfile = NULL;
    FREE(sciPath);
    sciPath = NULL;

    return FALSE;
}
/*--------------------------------------------------------------------------*/
