
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/ 
#include <libxml/xpath.h>
#include <libxml/xmlreader.h>
#include "loadClasspath.h"
#include "GetXmlFileEncoding.h"
#include "../../fileio/includes/FileExist.h"
#include "addToClasspath.h"
#include "setgetSCIpath.h"
#include "MALLOC.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/ 
BOOL LoadClasspath(char *xmlfilename)
{
	BOOL bOK = FALSE;
	if ( FileExist(xmlfilename) )
	{
		char *encoding=GetXmlFileEncoding(xmlfilename);

		/* Don't care about line return / empty line */
		xmlKeepBlanksDefault(0);
		/* check if the XML file has been encoded with utf8 (unicode) or not */
		if ( (strcmp("utf-8", encoding)!=0) || (strcmp("UTF-8", encoding)==0) )
		{
			xmlDocPtr doc;
			xmlXPathContextPtr xpathCtxt = NULL;
			xmlXPathObjectPtr xpathObj = NULL;
			char *classpath=NULL;

			doc = xmlParseFile (xmlfilename);

			if (doc == NULL) 
			{
				printf(_("Error: could not parse file %s\n"), xmlfilename);
				if (encoding) {FREE(encoding);encoding=NULL;}
				return bOK;
			}

			xpathCtxt = xmlXPathNewContext(doc);
			xpathObj = xmlXPathEval((const xmlChar*)"//classpaths/path", xpathCtxt);

			if(xpathObj && xpathObj->nodesetval->nodeMax) 
			{
				/* the Xpath has been understood and there are node */
				int	i;
				for(i = 0; i < xpathObj->nodesetval->nodeNr; i++)
				{

					xmlAttrPtr attrib=xpathObj->nodesetval->nodeTab[i]->properties;
					/* Get the properties of <path>  */
					while (attrib != NULL)
					{
						/* loop until when have read all the attributes */
						if (xmlStrEqual (attrib->name, (const xmlChar*) "value"))
						{ 
							/* we found the tag value */
							const char *str=(const char*)attrib->children->content;
							classpath=(char*)MALLOC(sizeof(char)*(strlen((const char*)str)+1));
							strcpy(classpath,str);
						}
						attrib = attrib->next;
					}

					if ( (classpath) && (strlen(classpath) > 0) )
					{
						#define KEYWORDSCILAB "$SCILAB" 
						char *SCIPATH=NULL;
						SCIPATH=getSCIpath();
						
						if (strncmp(classpath,KEYWORDSCILAB,strlen(KEYWORDSCILAB))==0)
						{
							char *modifypath = (char*)MALLOC(sizeof(char)*(strlen(SCIPATH)+strlen(classpath)+1));
							strcpy(modifypath,SCIPATH);
							strcat(modifypath,&classpath[strlen(KEYWORDSCILAB)]);
							FREE(classpath);
							classpath = modifypath;
						}

  					    if (SCIPATH) {FREE(SCIPATH);SCIPATH=NULL;}
						addToClasspath(classpath);
						FREE(classpath);
						classpath = NULL;
					}
				}
				bOK = TRUE;
			}
			else
			{
			printf(_("Wrong format for %s.\n"), xmlfilename);
			}
		}
		else
		{
			printf(_("Error : Not a valid classpath file %s (encoding not 'utf-8') Encoding '%s' found\n"), xmlfilename, encoding);
		}
		if (encoding) {FREE(encoding);encoding=NULL;}
	}else{
		printf(_("Warning: could not find classpath declaration file %s.\n"), xmlfilename);
	}
	return bOK;
}
/*--------------------------------------------------------------------------*/ 
