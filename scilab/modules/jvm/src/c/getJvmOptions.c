/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
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
#include <stdlib.h>
#include <libxml/xpath.h>
#include <libxml/xmlreader.h>
#include "getJvmOptions.h"
#include "GetXmlFileEncoding.h"
#include "sci_malloc.h"
#include "localization.h"
#include "machine.h"
#include "FileExist.h"
#include "os_string.h"
#include "strsubst.h"
#include "getos.h"
#include "getshortpathname.h"
#include "BOOL.h"
#include "getScilabPreference.h"

static char * getJavaHeapSize(void);
/*--------------------------------------------------------------------------*/
JavaVMOption * getJvmOptions(char *SCI_PATH, char *filename_xml_conf, int *size_JavaVMOption)
{
    if ( FileExist(filename_xml_conf) )
    {
        JavaVMOption *jvm_options = NULL;
        char *encoding = GetXmlFileEncoding(filename_xml_conf);

        /* Don't care about line return / empty line */
        xmlKeepBlanksDefault(0);
        /* check if the XML file has been encoded with utf8 (unicode) or not */
        if (stricmp("utf-8", encoding) == 0)
        {
            xmlDocPtr doc = NULL;
            xmlXPathContextPtr xpathCtxt = NULL;
            xmlXPathObjectPtr xpathObj = NULL;
            char *jvm_option_string = NULL;
            char *xpath_query = NULL;

            int indice = 0;
            {
                BOOL bConvert = FALSE;
                char *shortfilename_xml_conf = getshortpathname(filename_xml_conf, &bConvert);
                if (shortfilename_xml_conf)
                {
                    doc = xmlParseFile (shortfilename_xml_conf);
                    FREE(shortfilename_xml_conf);
                    shortfilename_xml_conf = NULL;
                }
            }

            if (doc == NULL)
            {
                fprintf(stderr, _("Error: Could not parse file %s.\n"), filename_xml_conf);
                FREE(encoding);
                encoding = NULL;
                *size_JavaVMOption = 0;
                return NULL;
            }

            xpathCtxt = xmlXPathNewContext(doc);
            /* Retrieve all nodes without the os tag + only the one from our operating system */
#define XPATH_QUERY "//jvm_options/option[not(@os)] | //jvm_options/option[@os='%s']"

            xpath_query = (char *)MALLOC(sizeof(char) * ((int)strlen(XPATH_QUERY) + (int)strlen(OSNAME) + 1));
            sprintf(xpath_query, XPATH_QUERY, OSNAME);

            xpathObj = xmlXPathEval((const xmlChar*)xpath_query, xpathCtxt);
            FREE(xpath_query);
            if (xpathObj && xpathObj->nodesetval->nodeMax)
            {
                /* the Xpath has been understood and there are node */
                int i;
                char *heapSize = getJavaHeapSize();

                for (i = 0; i < xpathObj->nodesetval->nodeNr; i++)
                {

                    xmlAttrPtr attrib = xpathObj->nodesetval->nodeTab[i]->properties;
                    /* Get the properties of <option>  */
                    while (attrib != NULL)
                    {
                        /* loop until when have read all the attributes */
                        if (xmlStrEqual (attrib->name, (const xmlChar*) "value"))
                        {
                            /* we found the tag name */
                            const char *str = (const char*)attrib->children->content;
                            if (jvm_option_string)
                            {
                                FREE(jvm_option_string);
                                jvm_option_string = NULL;
                            }
                            if (strstr(str, "-Xmx") == str && heapSize)
                            {
                                jvm_option_string = os_strdup(heapSize);
                            }
                            else
                            {
                                jvm_option_string = os_strdup(str);
                            }
                        }
                        attrib = attrib->next;
                    }

                    if ( (jvm_option_string) && (strlen(jvm_option_string) > 0) )
                    {
                        char *option_string_path_separator = NULL;
                        char *option_string_sci_path = NULL;

                        option_string_path_separator = strsub(jvm_option_string, "$PATH_SEPARATOR", PATH_SEPARATOR);
                        option_string_sci_path = strsub(option_string_path_separator, "$SCILAB", SCI_PATH);
                        if (option_string_path_separator)
                        {
                            FREE(option_string_path_separator);
                        }

                        jvm_options = (JavaVMOption *)REALLOC(jvm_options, sizeof(JavaVMOption) * (indice + 1));
                        jvm_options[indice].optionString = option_string_sci_path;
                        indice++;
                    }
                    if (jvm_option_string)
                    {
                        FREE(jvm_option_string);
                        jvm_option_string = NULL;
                    }
                }

                FREE(heapSize);
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

            /* xmlCleanupParser is called in
             * modules/core/src/c/TerminateCore.c
             * since it needs to be done only once.
             */

            if (getenv("SCI_JAVA_ENABLE_HEADLESS") != NULL)
            {
                /* When Scilab is built from a virtual machine, it needs
                 * an X11 server / input
                 * This is only called by "make doc" by the SCI/Makefile.am
                 */
#define HEADLESS "-Djava.awt.headless=true"
                jvm_options = (JavaVMOption *)REALLOC(jvm_options, sizeof(JavaVMOption) * (indice + 1));
                jvm_options[indice].optionString = MALLOC((strlen(HEADLESS) + 1) * sizeof(char));
                strcpy(jvm_options[indice].optionString, HEADLESS);
                indice++;
#undef HEADLESS
            }

            FREE(encoding);
            encoding = NULL;

            *size_JavaVMOption = indice;
            return jvm_options;
        }
        else
        {
            fprintf(stderr, _("Error: Not a valid configuration file %s (encoding not '%s') Encoding '%s' found.\n"), filename_xml_conf, "utf-8", encoding);
        }
        FREE(encoding);
        encoding = NULL;
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/
char * getJavaHeapSize(void)
{
    const char * value = getScilabPreferences()->heapSize;
    char * rvalue = NULL;
    int ivalue;

    if (value)
    {
        ivalue = (int)atof(value);
        if (ivalue > 0)
        {
            rvalue = (char *)MALLOC(24 * sizeof(char));
            sprintf(rvalue, "-Xmx%dm", ivalue);
        }
    }

    return rvalue;
}
