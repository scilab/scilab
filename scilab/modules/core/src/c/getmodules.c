/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/xpath.h>
#include <libxml/xmlreader.h>
#include "getmodules.h"
#include "sci_malloc.h"
#include "sci_path.h"
#include "localization.h"
#include "string.h"
#include "sciprint.h"
#include "GetXmlFileEncoding.h"
#include "scilabDefaults.h"
#include "FileExist.h"
#include "os_string.h"
#include "getshortpathname.h"
/*--------------------------------------------------------------------------*/
static struct MODULESLIST *ScilabModules = NULL;
/*--------------------------------------------------------------------------*/
static BOOL ReadModulesFile(void);
static BOOL AppendModules(char *filename);
static BOOL VerifyModule(char *ModuleName);
/*--------------------------------------------------------------------------*/
struct MODULESLIST *getmodules(void)
{
    if (ScilabModules == NULL)
    {
        ScilabModules = (struct MODULESLIST *)MALLOC(sizeof(struct MODULESLIST));
        ReadModulesFile();
    }
    return ScilabModules;
}
/*--------------------------------------------------------------------------*/
BOOL DisposeModulesInfo(void)
{
    BOOL bOK = FALSE;
    if (ScilabModules)
    {
        int i = 0;
        for (i = 0; i < ScilabModules->numberofModules; i++)
        {
            if (ScilabModules->ModuleList[i])
            {
                FREE(ScilabModules->ModuleList[i]);
                ScilabModules->ModuleList[i] = NULL;
            }
        }
        if (ScilabModules->ModuleList)
        {
            FREE(ScilabModules->ModuleList);
            ScilabModules->ModuleList = NULL;
        }
        ScilabModules->numberofModules = 0;
        FREE(ScilabModules);
        ScilabModules = NULL;
    }

    return bOK;
}
/*--------------------------------------------------------------------------*/
static BOOL ReadModulesFile(void)
{
    BOOL bOK = FALSE;
    char *ModulesFilename = NULL;
    char *SciPath = NULL;

    SciPath = getSCI();
    if (SciPath == NULL)
    {
        sciprint(_("The SCI environment variable is not set.\n"));
        return FALSE;
    }

    ModulesFilename = (char*)MALLOC((strlen(SciPath) + strlen("/") + strlen(basenamemodulesfile) + 1) * sizeof(char));
    sprintf(ModulesFilename, "%s/%s", SciPath, basenamemodulesfile);
    FREE(SciPath);
    SciPath = NULL;

    if (FileExist(ModulesFilename))
    {
        AppendModules(ModulesFilename);
        FREE(ModulesFilename);
        ModulesFilename = NULL;
    }
    else
    {
        sciprint(_("Cannot load the module declaration file: %s.\n"), ModulesFilename);
        FREE(ModulesFilename);
        ModulesFilename = NULL;
        return FALSE;
    }
    return bOK;
}
/*--------------------------------------------------------------------------*/
static BOOL VerifyModule(char *ModuleName)
{
    BOOL bOK = TRUE;
    char *SciPath = NULL;
    char *FullPathModuleName = NULL;


    SciPath = getSCI();
    if (SciPath == NULL)
    {
        sciprint(_("The SCI environment variable is not set.\n"));
        return FALSE;
    }

    FullPathModuleName = (char*)MALLOC((strlen(SciPath) + strlen("%s/modules/%s/etc/%s.start") + (strlen(ModuleName) * 2) + 1) * sizeof(char));
    sprintf(FullPathModuleName, "%s/modules/%s/etc/%s.start", SciPath, ModuleName, ModuleName);
    FREE(SciPath);
    SciPath = NULL;

    /* @TODO add more checks (missing files for example) */

    if (!FileExist(FullPathModuleName))
    {
        fprintf(stderr, _("Warning: Could not find %s\n"), FullPathModuleName);
        bOK = FALSE;
    }
    FREE(FullPathModuleName);
    FullPathModuleName = NULL;

    return bOK;
}
/*--------------------------------------------------------------------------*/
static BOOL AppendModules(char *xmlfilename)
{
    BOOL bOK = FALSE;
    if ( FileExist(xmlfilename) )
    {
        char *encoding = GetXmlFileEncoding(xmlfilename);

        /* Don't care about line return / empty line */
        xmlKeepBlanksDefault(0);
        /* check if the XML file has been encoded with utf8 (unicode) or not */
        if (stricmp("utf-8", encoding) == 0)
        {
            xmlDocPtr doc = NULL;
            xmlXPathContextPtr xpathCtxt = NULL;
            xmlXPathObjectPtr xpathObj = NULL;
            char *name = NULL;
            int activate = 0;

            int indice = 0;
            BOOL bConvert = FALSE;
            char *shortxmlfilename = getshortpathname(xmlfilename, &bConvert);

            if (shortxmlfilename)
            {
                doc = xmlParseFile (shortxmlfilename);
                FREE(shortxmlfilename);
                shortxmlfilename = NULL;
            }

            if (doc == NULL)
            {
                printf(_("Error: Could not parse file %s.\n"), xmlfilename);
                FREE(encoding);
                encoding = NULL;
                return bOK;
            }

            xpathCtxt = xmlXPathNewContext(doc);
            xpathObj = xmlXPathEval((const xmlChar*)"//modules/module", xpathCtxt);

            if (xpathObj && xpathObj->nodesetval->nodeMax)
            {
                /* the Xpath has been understood and there are node */
                int    i;
                for (i = 0; i < xpathObj->nodesetval->nodeNr; i++)
                {

                    xmlAttrPtr attrib = xpathObj->nodesetval->nodeTab[i]->properties;
                    /* Get the properties of <module>  */
                    while (attrib != NULL)
                    {
                        /* loop until when have read all the attributes */
                        if (xmlStrEqual (attrib->name, (const xmlChar*) "name"))
                        {
                            /* we found the tag name */
                            const char *str = (const char*)attrib->children->content;
                            if (name)
                            {
                                FREE(name);
                            }
                            name = os_strdup(str);
                        }
                        else if (xmlStrEqual (attrib->name, (const xmlChar*) "activate"))
                        {
                            /* we found the tag activate */
                            const char *str = (const char*)attrib->children->content;
                            if (stricmp(str, "yes") == 0 || strcmp(str, "1") == 0)
                            {
                                activate = 1;
                            }
                        }
                        attrib = attrib->next;
                    }

                    if ( (name) && (strlen(name) > 0) && (activate) )
                    {
                        if ( VerifyModule(name) )
                        {
                            if (indice == 0)
                            {
                                ScilabModules->ModuleList = (char**)MALLOC(sizeof(char*) * (indice + 1));
                            }
                            else
                            {
                                ScilabModules->ModuleList = (char**)REALLOC(ScilabModules->ModuleList, sizeof(char*) * (indice + 1));
                            }

                            ScilabModules->numberofModules = indice + 1;

                            ScilabModules->ModuleList[indice] = os_strdup(name);
                            indice++;
                        }
                        else
                        {
                            sciprint(_("%s module not found.\n"), name);
                        }
                    }
                    if (name)
                    {
                        FREE(name);
                        name = NULL;
                    }
                    activate = 0;
                }
                bOK = TRUE;
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
            printf(_("Error: Not a valid module file %s (encoding not '%s') Encoding '%s' found.\n"), xmlfilename, "utf-8", encoding);
        }
        FREE(encoding);
        encoding = NULL;
    }
    return bOK;
}
/*--------------------------------------------------------------------------*/
