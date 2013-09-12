/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <libxml/xpath.h>
#include <libxml/xmlreader.h>
#include <stdio.h>
#include "loadLibrarypath.h"
#include "GetXmlFileEncoding.h"
#include "FileExist.h"
#include "addToLibrarypath.h"
#include "setgetSCIpath.h"
#include "MALLOC.h"
#include "localization.h"
#include "stricmp.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "BOOL.h"
#include "getshortpathname.h"
/*--------------------------------------------------------------------------*/
BOOL LoadLibrarypath(char *xmlfilename)
{
    BOOL bOK = FALSE;
    if ( FileExist(xmlfilename) )
    {
        char *encoding = GetXmlFileEncoding(xmlfilename);

        /* Don't care about line return / empty line */
        xmlKeepBlanksDefault(0);
        /* check if the XML file has been encoded with utf8 (unicode) or not */
        if ( stricmp("utf-8", encoding) == 0 )
        {
            xmlDocPtr doc = NULL;
            xmlXPathContextPtr xpathCtxt = NULL;
            xmlXPathObjectPtr xpathObj = NULL;
            char *libraryPath = NULL;

            {
                BOOL bConvert = FALSE;
                char *shortxmlfilename = getshortpathname(xmlfilename, &bConvert);
                if (shortxmlfilename)
                {
                    doc = xmlParseFile (shortxmlfilename);
                    FREE(shortxmlfilename);
                    shortxmlfilename = NULL;
                }
            }

            if (doc == NULL)
            {
                fprintf(stderr, _("Error: could not parse file %s\n"), xmlfilename);
                if (encoding)
                {
                    FREE(encoding);
                    encoding = NULL;
                }
                return bOK;
            }

            xpathCtxt = xmlXPathNewContext(doc);
            xpathObj = xmlXPathEval((const xmlChar*)"//librarypaths/path", xpathCtxt);

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
                            libraryPath = (char*)attrib->children->content;
                        }
                        attrib = attrib->next;
                    }

                    if ( (libraryPath) && (strlen(libraryPath) > 0) )
                    {
#define KEYWORDSCILAB "$SCILAB"
                        char *FullLibrarypath = NULL;
                        char *sciPath = getSCIpath();

                        if (strncmp(libraryPath, KEYWORDSCILAB, strlen(KEYWORDSCILAB)) == 0)
                        {
                            FullLibrarypath = (char*)MALLOC(sizeof(char) * (strlen(sciPath) + strlen(libraryPath) + 1));
                            if (FullLibrarypath)
                            {
                                strcpy(FullLibrarypath, sciPath);
                                strcat(FullLibrarypath, &libraryPath[strlen(KEYWORDSCILAB)]);
                            }
                        }
                        else
                        {
                            FullLibrarypath = strdup(libraryPath);
                        }


                        if (FullLibrarypath)
                        {
                            addToLibrarypath(FullLibrarypath);
                            FREE(FullLibrarypath);
                            FullLibrarypath = NULL;
                        }

                        if (sciPath)
                        {
                            FREE(sciPath);
                            sciPath = NULL;
                        }
                        libraryPath = NULL;
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
            xmlFreeDoc (doc);
        }
        else
        {
            fprintf(stderr, _("Error : Not a valid path file %s (encoding not 'utf-8') Encoding '%s' found\n"), xmlfilename, encoding);
        }
        if (encoding)
        {
            FREE(encoding);
            encoding = NULL;
        }
    }
    return bOK;
}
/*--------------------------------------------------------------------------*/
