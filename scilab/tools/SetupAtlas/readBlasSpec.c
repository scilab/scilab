/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
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
#include <windows.h>
#include <libxml/xpath.h>
#include <libxml/xmlreader.h>
#include "readBlasSpec.h"
#include "xmlEncoding.h"
/*--------------------------------------------------------------------------*/
static char *getshortpathname(wchar_t *wclongpathname, BOOL *convertok);
static wchar_t *GetXmlFileEncoding(const wchar_t *wcfilename);
/*--------------------------------------------------------------------------*/
struct cpu_struct ** readBlasSpec(wchar_t *blasspec_filename, int *sizeArrayReturned)
{
    struct cpu_struct **CPUS_BLAS_SPEC = NULL;

    if (blasspec_filename)
    {
        wchar_t *FileEncoding = GetXmlFileEncoding(blasspec_filename);
        if (FileEncoding)
        {
            if (_wcsicmp(FileEncoding, L"UTF-8") == 0)
            {
#define XPATH "//blas_specification/cpu"
                BOOL bConvert = FALSE;
                char *shortfilename = getshortpathname((wchar_t*)blasspec_filename, &bConvert);
                if (shortfilename)
                {
                    xmlXPathContextPtr xpathCtxt = NULL;
                    xmlXPathObjectPtr xpathObj = NULL;
                    xmlDocPtr BlasSpecxmlDocPtr = xmlParseFile (shortfilename);

                    free(shortfilename);
                    shortfilename = NULL;

                    xmlKeepBlanksDefault(0);

                    if (BlasSpecxmlDocPtr)
                    {
                        xpathCtxt = xmlXPathNewContext(BlasSpecxmlDocPtr);
                        xpathObj = xmlXPathEval((const xmlChar*)XPATH, xpathCtxt);

                        if (xpathObj && xpathObj->nodesetval->nodeMax)
                        {
                            /* the Xpath has been understood and there are node */
                            CPUS_BLAS_SPEC = (struct cpu_struct **) malloc(sizeof(struct cpu_struct *) * xpathObj->nodesetval->nodeNr);
                            if (CPUS_BLAS_SPEC)
                            {
                                int	i = 0;
                                for (i = 0; i < xpathObj->nodesetval->nodeNr; i++)
                                {
                                    xmlAttrPtr attrib = xpathObj->nodesetval->nodeTab[i]->properties;
                                    CPUS_BLAS_SPEC[i] = (struct cpu_struct *) malloc(sizeof(struct cpu_struct));
                                    if (CPUS_BLAS_SPEC[i])
                                    {
                                        while (attrib != NULL)
                                        {
                                            if (xmlStrEqual (attrib->name, (const xmlChar*) "cpu_manufacturer"))
                                            {
                                                /* we found the tag cpu_manufacturer */
                                                CPUS_BLAS_SPEC[i]->cpu_vendor = toWideString((char*)attrib->children->content);
                                            }

                                            if (xmlStrEqual (attrib->name, (const xmlChar*) "cpu_family"))
                                            {
                                                /* we found the tag cpu_family */
                                                if (strcmp((char*)attrib->children->content, "*") == 0)
                                                {
                                                    CPUS_BLAS_SPEC[i]->cpu_family = 0;
                                                }
                                                else
                                                {
                                                    CPUS_BLAS_SPEC[i]->cpu_family = atoi((char*)attrib->children->content);
                                                }
                                            }

                                            if (xmlStrEqual (attrib->name, (const xmlChar*) "cpu_model"))
                                            {
                                                /* we found the tag cpu_model */
                                                if (strcmp((char*)attrib->children->content, "*") == 0)
                                                {
                                                    CPUS_BLAS_SPEC[i]->cpu_model = 0;
                                                }
                                                else
                                                {
                                                    CPUS_BLAS_SPEC[i]->cpu_model = atoi((char*)attrib->children->content);
                                                }
                                            }

                                            if (xmlStrEqual (attrib->name, (const xmlChar*) "dll_filename"))
                                            {
                                                /* we found the tag dll_filename */
                                                CPUS_BLAS_SPEC[i]->dll_filename = toWideString((char*)attrib->children->content);
                                            }

                                            if (xmlStrEqual (attrib->name, (const xmlChar*) "comments"))
                                            {
                                                /* we found the tag comments */
                                                CPUS_BLAS_SPEC[i]->comments = toWideString((char*)attrib->children->content);
                                            }
                                            attrib = attrib->next;
                                        }
                                    }
                                    *sizeArrayReturned = i;
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
                        xmlFreeDoc (BlasSpecxmlDocPtr);
                    }
                }
            }
        }
        free(FileEncoding);
        FileEncoding = NULL;
    }

    return CPUS_BLAS_SPEC;
}
/*--------------------------------------------------------------------------*/
wchar_t *GetXmlFileEncoding(const wchar_t *wcfilename)
{
    wchar_t *encoding = NULL;

    if (wcfilename)
    {
        xmlDocPtr doc = NULL;
        BOOL bConvert = FALSE;
        char *shortfilename = getshortpathname((wchar_t*)wcfilename, &bConvert);
        if (shortfilename)
        {
            doc = xmlParseFile(shortfilename);

            free(shortfilename);
            shortfilename = NULL;
            if (doc)
            {
                if (doc->encoding)
                {
                    encoding = toWideString((char*)doc->encoding);
                }
            }
            xmlFreeDoc (doc);
        }
    }
    return encoding;
}
/*--------------------------------------------------------------------------*/
char *getshortpathname(wchar_t *wclongpathname, BOOL *convertok)
{
    char *ShortName = NULL;

    if (wclongpathname)
    {
        /* first we try to call to know path length */
        wchar_t *ptwShortName = NULL;
        int length = GetShortPathNameW(wclongpathname, NULL, 0);

        if (length <= 0 )
        {
            length = MAX_PATH;
        }

        ptwShortName = (wchar_t*)calloc((length + 1), sizeof(wchar_t));

        if (ptwShortName)
        {
            /* second converts path */
            if ( GetShortPathNameW(wclongpathname, ptwShortName, length) )
            {
                ShortName = toUTF(ptwShortName);
                *convertok = TRUE;
            }
            else
            {
                /* FAILED */
                ShortName = toUTF(wclongpathname);
                *convertok = FALSE;
            }
            if (ptwShortName)
            {
                free(ptwShortName);
                ptwShortName = NULL;
            }
        }
        else
        {
            /* FAILED */
            ShortName = toUTF(wclongpathname);
            *convertok = FALSE;
        }
    }
    else
    {
        /* FAILED */
        *convertok = FALSE;
    }
    return ShortName;
}
/*--------------------------------------------------------------------------*/
