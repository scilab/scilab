/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libxml/xpath.h>
#include <libxml/xmlreader.h>
#include "loadversion.h"
#include "with_module.h"
#include "sci_path.h"
#include "sci_malloc.h"
#include "GetXmlFileEncoding.h"
#include "scilabDefaults.h"
#include "localization.h"
#include "FileExist.h"
#include "os_string.h"
#include "getshortpathname.h"
#include "charEncoding.h"
#include "version.h"
/*--------------------------------------------------------------------------*/
BOOL getversionmodule(wchar_t* _pwstModule,
                      int *sci_version_major,
                      int *sci_version_minor,
                      int *sci_version_maintenance,
                      wchar_t* _pwstSciVersionString,
                      int *sci_version_revision)
{
    BOOL bOK = FALSE;

    if (with_module(_pwstModule))
    {
        char* filename_VERSION_module = NULL;
        char* pstModule = wide_string_to_UTF8(_pwstModule);
        char* SciPath = NULL;
        int len = 0;

        SciPath = getSCI();
        len = (int)strlen(FORMATVERSIONFILENAME) + (int)strlen(SciPath) + (int)strlen(pstModule) + 1;
        filename_VERSION_module = (char*)MALLOC(sizeof(char) * len);
        sprintf(filename_VERSION_module, FORMATVERSIONFILENAME, SciPath, pstModule);
        FREE(pstModule);
        FREE(SciPath);
        SciPath = NULL;

        if (FileExist(filename_VERSION_module))
        {
            char *encoding = GetXmlFileEncoding(filename_VERSION_module);

            /* Don't care about line return / empty line */
            xmlKeepBlanksDefault(0);

            /* check if the XML file has been encoded with utf8 (unicode) or not */
            if ( stricmp("utf-8", encoding) == 0)
            {
                xmlDocPtr doc = NULL;
                xmlXPathContextPtr xpathCtxt = NULL;
                xmlXPathObjectPtr xpathObj = NULL;

                int version_major = 0;
                int version_minor = 0;
                int version_maintenance = 0;
                int version_revision = 0;
                wchar_t *pwstSciVersionString = 0;

                {
                    BOOL bConvert = FALSE;
                    char *shortfilename_VERSION_module = getshortpathname(filename_VERSION_module, &bConvert);
                    if (shortfilename_VERSION_module)
                    {
                        doc = xmlParseFile (shortfilename_VERSION_module);
                        FREE(shortfilename_VERSION_module);
                        shortfilename_VERSION_module = NULL;
                    }
                }

                if (doc == NULL)
                {
                    fprintf(stderr, _("Error: Could not parse file %s\n"), filename_VERSION_module);
                    FREE(encoding);
                    encoding = NULL;
                    return FALSE;
                }

                xpathCtxt = xmlXPathNewContext(doc);
                xpathObj = xmlXPathEval((const xmlChar*)"//MODULE_VERSION/VERSION", xpathCtxt);

                if (xpathObj && xpathObj->nodesetval->nodeMax)
                {

                    xmlAttrPtr attrib = xpathObj->nodesetval->nodeTab[0]->properties;
                    while (attrib != NULL)
                    {
                        if (xmlStrEqual (attrib->name, (const xmlChar*) "major"))
                        {
                            /* we found <major> */
                            const char *str = (const char*)attrib->children->content;
                            version_major = atoi(str);
                        }
                        else if (xmlStrEqual (attrib->name, (const xmlChar*)"minor"))
                        {
                            /* we found <minor> */
                            const char *str = (const char*)attrib->children->content;
                            version_minor = atoi(str);
                        }
                        else if (xmlStrEqual (attrib->name, (const xmlChar*)"maintenance"))
                        {
                            /* we found <maintenance> */
                            const char *str = (const char*)attrib->children->content;
                            version_maintenance = atoi(str);
                        }
                        else if (xmlStrEqual (attrib->name, (const xmlChar*)"revision"))
                        {
                            /* we found <revision> */
                            const char *str = (const char*)attrib->children->content;
                            version_revision = atoi(str);
                        }
                        else if (xmlStrEqual (attrib->name, (const xmlChar*)"string"))
                        {
                            /* we found <string> */
                            const char *str = (const char*)attrib->children->content;
                            if (pwstSciVersionString)
                            {
                                FREE(pwstSciVersionString);
                            }
                            pwstSciVersionString = to_wide_string(str);
                        }

                        attrib = attrib->next;
                    }

                    *sci_version_major = version_major;
                    *sci_version_minor = version_minor;
                    *sci_version_maintenance = version_maintenance;
                    *sci_version_revision = version_revision;
                    if (pwstSciVersionString)
                    {
                        wcscpy(_pwstSciVersionString, pwstSciVersionString);
                        FREE(pwstSciVersionString);
                        pwstSciVersionString = NULL;
                    }
                    else
                    {
                        _pwstSciVersionString = NULL;
                    }
                }
                else
                {
                    fprintf(stderr, _("Error: Not a valid version file %s (should start with <MODULE_VERSION> and contain <VERSION major='' minor='' maintenance='' revision='' string=''>)\n"), filename_VERSION_module);
                    FREE(encoding);
                    encoding = NULL;
                    return FALSE;
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
                fprintf(stderr, _("Error: Not a valid version file %s (encoding not 'utf-8') Encoding '%s' found\n"), filename_VERSION_module, encoding);
            }

            FREE(encoding);
            encoding = NULL;
            bOK = TRUE;
        }
        else
        {
            // version.xml does not exist but module exists then we returns scilab version
            *sci_version_major =  SCI_VERSION_MAJOR;
            *sci_version_minor = SCI_VERSION_MINOR;
            *sci_version_maintenance = SCI_VERSION_MAINTENANCE;
            *sci_version_revision = SCI_VERSION_TIMESTAMP;
            wcscpy(_pwstSciVersionString, L"");
            bOK = TRUE;
        }
        FREE(filename_VERSION_module);
        filename_VERSION_module = NULL;
    }
    return bOK;
}
/*--------------------------------------------------------------------------*/
