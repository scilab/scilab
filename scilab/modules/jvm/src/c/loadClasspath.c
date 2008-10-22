/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) 2008-2008 - INRIA - Sylvestre LEDRU
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
#include <stdio.h>
#include <string.h>
#include "loadClasspath.h"
#include "GetXmlFileEncoding.h"
#include "../../fileio/includes/FileExist.h"
#include "addToClasspath.h"
#include "setgetSCIpath.h"
#include "MALLOC.h"
#include "localization.h"
#include "scilabmode.h"
#include "stricmp.h"
#ifdef _MSC_VER
	#include "strdup_windows.h"
#endif
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
		char *encoding=GetXmlFileEncoding(xmlfilename);

		/* Don't care about line return / empty line */
		xmlKeepBlanksDefault(0);
		/* check if the XML file has been encoded with utf8 (unicode) or not */
		if ( stricmp("utf-8", encoding)==0 )
		{
			xmlXPathContextPtr xpathCtxt = NULL;
			xmlXPathObjectPtr xpathObj = NULL;
			char *classpath=NULL;
			char *load="";
			typeOfLoad eLoad=STARTUP;
			char *currentMode = getScilabModeString();
			/* Xpath Query :
			 * Retrieve all the path which are not disabled in our mode 
			 */
			#define XPATH "//classpaths/path[not(@disableUnderMode='%s')]"
			char * XPath=(char*)MALLOC(sizeof(char)*(strlen(XPATH)+strlen(currentMode)-2+1)); /* -2 = strlen(%s) */
			sprintf(XPath,XPATH,currentMode);

			ClassPathxmlDocPtr = xmlParseFile (xmlfilename);

			if (ClassPathxmlDocPtr == NULL) 
			{
				fprintf(stderr,_("Error: could not parse file %s\n"), xmlfilename);
				if (encoding) {FREE(encoding);encoding=NULL;}
				return bOK;
			}

			xpathCtxt = xmlXPathNewContext(ClassPathxmlDocPtr);
			xpathObj = xmlXPathEval((const xmlChar*)XPath, xpathCtxt);

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
							classpath=(char*)attrib->children->content;
						}
						if (xmlStrEqual (attrib->name, (const xmlChar*) "load"))
						{ 
							/* we found the tag load */
							load = (char*)attrib->children->content;

							/* By default, it is startup */
							if (stricmp(load,"background")==0)
							{
								eLoad=BACKGROUND;
							} 
							else 
							{
								if (stricmp(load,"onuse")==0) 
								{
									eLoad=ONUSE;
								}
							}
						}
						else
						{
							eLoad=STARTUP;
						}
						attrib = attrib->next;
					}

					if ( (classpath) && (strlen(classpath) > 0) && (strncmp(classpath,"@",1) != 0) ) /* If it starts by a @ that means it hasn't been able to find it... which is normal... for example with the documentation */
					{
						#define KEYWORDSCILAB "$SCILAB" 
						char *sciPath = getSCIpath();
						char *FullClasspath = NULL;
						
						if (strncmp(classpath,KEYWORDSCILAB,strlen(KEYWORDSCILAB))==0)
						{
							FullClasspath = (char*)MALLOC(sizeof(char)*(strlen(sciPath)+strlen(classpath)+1));
							if (FullClasspath)
							{
								strcpy(FullClasspath,sciPath);
								strcat(FullClasspath,&classpath[strlen(KEYWORDSCILAB)]);
							}
						}
						else
						{
							FullClasspath = strdup(classpath);
						}
  					    
						if (FullClasspath)
						{
							if (!addToClasspath(FullClasspath, eLoad))
							{
								errorOnLoad=TRUE;
							}
							FREE(FullClasspath);
							FullClasspath = NULL;
						}

						if (sciPath) {FREE(sciPath);sciPath=NULL;}
						classpath = NULL;
					}
				}
				bOK = TRUE;
			}
			else
			{
				fprintf(stderr,_("Wrong format for %s.\n"), xmlfilename);
			}

			if(xpathObj) xmlXPathFreeObject(xpathObj);
			if(xpathCtxt) xmlXPathFreeContext(xpathCtxt);
			/*
			* Cleanup function for the XML library.
			*/
			xmlCleanupParser();
		}
		else
		{
			fprintf(stderr,_("Error : Not a valid classpath file %s (encoding not 'utf-8') Encoding '%s' found\n"), xmlfilename, encoding);
		}
		if (encoding) {FREE(encoding);encoding=NULL;}
	}
	else
	{
		fprintf(stderr,_("Warning: could not find classpath declaration file %s.\n"), xmlfilename);
	}
	if (errorOnLoad)
	{
		fprintf(stderr,_("Some problems during the loading of the Java libraries occured.\nThis could lead to inconsistent behaviours.\nPlease check SCI/etc/classpath.xml.\n"));
	}

	return bOK;
}
/*--------------------------------------------------------------------------*/ 
