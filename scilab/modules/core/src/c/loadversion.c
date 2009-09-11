/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libxml/xpath.h>
#include <libxml/xmlreader.h>
#include "loadversion.h"
#include "with_module.h"
#include "setgetSCIpath.h"
#include "MALLOC.h"
#include "GetXmlFileEncoding.h"
#include "scilabDefaults.h"
#include "localization.h"
#include "stricmp.h"
#include "../../../fileio/includes/FileExist.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "getshortpathname.h"
/*--------------------------------------------------------------------------*/ 
BOOL getversionmodule(char *modulename,
					  int *sci_version_major,
					  int *sci_version_minor,
					  int *sci_version_maintenance,
					  char *sci_version_string,
					  int *sci_version_revision)
{
	BOOL bOK=FALSE;

	if (with_module(modulename))
	{
		char *filename_VERSION_module=NULL;
		char *SciPath=NULL;
		int len=0;

		SciPath=getSCIpath();
		len=(int)strlen(FORMATVERSIONFILENAME)+(int)strlen(SciPath)+(int)strlen(modulename)+1;
		filename_VERSION_module=(char*)MALLOC(sizeof(char)*len);
		sprintf(filename_VERSION_module,FORMATVERSIONFILENAME,SciPath,modulename);
		if (SciPath){FREE(SciPath);SciPath=NULL;}

		if (FileExist(filename_VERSION_module))
		{
			char *encoding=GetXmlFileEncoding(filename_VERSION_module);

			/* Don't care about line return / empty line */
			xmlKeepBlanksDefault(0);

			/* check if the XML file has been encoded with utf8 (unicode) or not */
			if ( stricmp("utf-8", encoding)==0) {
				xmlDocPtr doc = NULL;
				xmlXPathContextPtr xpathCtxt = NULL;
				xmlXPathObjectPtr xpathObj = NULL;

				int version_major=0;
				int version_minor=0;
				int version_maintenance=0;
				int version_revision=0;
				char *version_string=0;

				{
					BOOL bConvert = FALSE;
					char *shortfilename_VERSION_module = getshortpathname(filename_VERSION_module,&bConvert);
					if (shortfilename_VERSION_module)
					{
						doc = xmlParseFile (shortfilename_VERSION_module);
						FREE(shortfilename_VERSION_module);
						shortfilename_VERSION_module = NULL;
					}
				}

				if (doc == NULL) 
				{
					fprintf(stderr,_("Error: Could not parse file %s\n"), filename_VERSION_module);
					return FALSE;
				}

			xpathCtxt = xmlXPathNewContext(doc);
			xpathObj = xmlXPathEval((const xmlChar*)"//MODULE_VERSION/VERSION", xpathCtxt);

			if(xpathObj && xpathObj->nodesetval->nodeMax) 
				{
					
					xmlAttrPtr attrib=xpathObj->nodesetval->nodeTab[0]->properties;
					while (attrib != NULL)
						{	
							if (xmlStrEqual (attrib->name, (const xmlChar*) "major"))
								{ 
									/* we found <major> */
									const char *str=(const char*)attrib->children->content;
									version_major=atoi(str);
								}
							else if (xmlStrEqual (attrib->name, (const xmlChar*)"minor"))
								{ 
									/* we found <minor> */
									const char *str=(const char*)attrib->children->content;
									version_minor=atoi(str);
								}
							else if (xmlStrEqual (attrib->name, (const xmlChar*)"maintenance"))
								{ 
									/* we found <maintenance> */
									const char *str=(const char*)attrib->children->content;
									version_maintenance=atoi(str);
								}
							else if (xmlStrEqual (attrib->name, (const xmlChar*)"revision"))
								{ 
									/* we found <revision> */
									const char *str=(const char*)attrib->children->content;
									version_revision=atoi(str);
								}
							else if (xmlStrEqual (attrib->name, (const xmlChar*)"string"))
								{
									/* we found <string> */
									const char *str = (const char*)attrib->children->content;
									version_string= strdup(str);
								}

							attrib = attrib->next;
						}

					*sci_version_major=version_major;
					*sci_version_minor=version_minor;
					*sci_version_maintenance=version_maintenance;
					*sci_version_revision=version_revision;
					strncpy(sci_version_string,version_string,1024);
					if (version_string) {FREE(version_string);version_string=NULL;}
				}
			else
				{
					fprintf(stderr,_("Error: Not a valid version file %s (should start with <MODULE_VERSION> and contains <VERSION major='' minor='' maintenance='' revision='' string=''>)\n"), filename_VERSION_module);
					return FALSE;
				}
			if(xpathObj) xmlXPathFreeObject(xpathObj);
			if(xpathCtxt) xmlXPathFreeContext(xpathCtxt);
			xmlFreeDoc (doc);
			}
			else
			{
				fprintf(stderr,_("Error: Not a valid version file %s (encoding not 'utf-8') Encoding '%s' found\n"), filename_VERSION_module, encoding);
			}

			if (encoding) {FREE(encoding);encoding=NULL;}

			bOK=TRUE;
		}
		
		if (filename_VERSION_module) {FREE(filename_VERSION_module);filename_VERSION_module=NULL;}
	}
	return bOK;
}
