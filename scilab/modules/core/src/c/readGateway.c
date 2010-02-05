/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libxml/xpath.h>
#include <libxml/xmlreader.h>
#include "readGateway.h"
#include "setgetSCIpath.h"
#include "scilabDefaults.h"
#include "FileExist.h"
#include "with_module.h"
#include "MALLOC.h"
#include "GetXmlFileEncoding.h"
#include "localization.h"
#if _MSC_VER
#include "strdup_Windows.h"
#endif
#include "getshortpathname.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
static char *getModuleXmlFilename(char *modulename);
static struct gateway_struct *readGatewayXmlFile(char *filenameXml);
/*--------------------------------------------------------------------------*/
struct gateway_struct *readGateway(char *modulename)
{
	struct gateway_struct *content = NULL;

	if (modulename)
	{
		char *XmlFile = getModuleXmlFilename(modulename);
		if (XmlFile)
		{
			content = readGatewayXmlFile(XmlFile);
			FREE(XmlFile);
			XmlFile = NULL;
		}
		else
		{
			/* module exists but without gateway */
			if ( with_module(modulename) ) 
			{
				content = (struct gateway_struct *)MALLOC(sizeof(struct gateway_struct));
				if (content)
				{
					content->dimLists = 0;
					content->gatewayIdList = NULL;
					content->primitivesList = NULL;
					content->primiviteIdList = NULL;
				}
			}
			else /* not exist */
			{
				content = NULL;
			}
		}
	}

	return content;
}
/*--------------------------------------------------------------------------*/
static struct gateway_struct *readGatewayXmlFile(char *filenameXml)
{
	struct gateway_struct *gateway = NULL;

	char *encoding = GetXmlFileEncoding(filenameXml);

	/* Don't care about line return / empty line */
	xmlKeepBlanksDefault(0); 

	/* check if the XML file has been encoded with utf8 (unicode) or not */
	if ( (strcmp("utf-8", encoding)!=0) || (strcmp("UTF-8", encoding)==0) )
	{
		xmlDocPtr doc = NULL;
		xmlXPathContextPtr xpathCtxt = NULL;
		xmlXPathObjectPtr xpathObj = NULL;

		int GATEWAY_ID=0;
		int PRIMITIVE_ID=0;
		char *PRIMITIVE_NAME=NULL;

		{
			BOOL bConvert = FALSE;
			char *shortfilenameXml = getshortpathname(filenameXml,&bConvert);
			if (shortfilenameXml)
			{
				doc = xmlParseFile (shortfilenameXml);
				FREE(shortfilenameXml);
				shortfilenameXml = NULL;
			}
		}

		if (doc == NULL) 
		{
			fprintf(stderr,_("Error: could not parse file %s\n"), filenameXml);
			if (encoding) {FREE(encoding);encoding=NULL;}
			return NULL;
		}

		gateway = (struct gateway_struct *)MALLOC(sizeof(struct gateway_struct));
		if (gateway == NULL)
		{
			fprintf(stderr,_("Error: Memory allocation.\n"));
			if (encoding) {FREE(encoding);encoding=NULL;}
			return NULL;
		}
		else 
		{
			gateway->dimLists = 0;
			gateway->gatewayIdList = NULL;
			gateway->primitivesList = NULL;
			gateway->primiviteIdList = NULL;
		}

		xpathCtxt = xmlXPathNewContext(doc);
		xpathObj = xmlXPathEval((const xmlChar*)"//GATEWAY/PRIMITIVE", xpathCtxt);

		if(xpathObj && xpathObj->nodesetval->nodeMax) 
		{
			/* the Xpath has been understood and there are node */
			int	i;
			for(i = 0; i < xpathObj->nodesetval->nodeNr; i++)
			{
				xmlAttrPtr attrib = xpathObj->nodesetval->nodeTab[i]->properties;
				/* Get the properties of <PRIMITIVE>  */
				while (attrib != NULL)
				{
					/* loop until when have read all the attributes */
					if (xmlStrEqual (attrib->name, (const xmlChar*) "gatewayId"))
					{ 
						/* we found the tag gatewayId */
						const char *str = (const char*)attrib->children->content;
						GATEWAY_ID = atoi(str);
					}
					else if (xmlStrEqual (attrib->name, (const xmlChar*)"primitiveId"))
					{ 
						/* we found the tag primitiveId */
						const char *str = (const char*)attrib->children->content;
						PRIMITIVE_ID = atoi(str);
					}
					else if (xmlStrEqual (attrib->name, (const xmlChar*)"primitiveName"))
					{
						/* we found the tag primitiveName */
						const char *str = (const char*)attrib->children->content;
						PRIMITIVE_NAME = strdup(str);
					}
					attrib = attrib->next;
				}

				if ( (GATEWAY_ID != 0) && (PRIMITIVE_ID != 0) && (PRIMITIVE_NAME) )
				{
					if (strlen(PRIMITIVE_NAME) > 0)
					{
						gateway->dimLists++;
						if (gateway->gatewayIdList)
						{
							gateway->gatewayIdList = (int*)REALLOC(gateway->gatewayIdList,
							                      sizeof(int*)*gateway->dimLists);
						}
						else
						{
							gateway->gatewayIdList = (int*)MALLOC(sizeof(int)*gateway->dimLists);
						}

						if (gateway->primitivesList)
						{
							gateway->primitivesList = (char **)REALLOC(gateway->primitivesList,
							                      sizeof(char**)*gateway->dimLists);
						}
						else
						{
							gateway->primitivesList = (char **)MALLOC(sizeof(char*)*gateway->dimLists);
						}

						if (gateway->primiviteIdList)
						{
							gateway->primiviteIdList = (int*)REALLOC(gateway->primiviteIdList,
							                      sizeof(int*)*gateway->dimLists);
						}
						else
						{
							gateway->primiviteIdList = (int*)MALLOC(sizeof(int)*gateway->dimLists);
						}

						if (gateway->gatewayIdList) 
							gateway->gatewayIdList[gateway->dimLists - 1] = GATEWAY_ID;

						if (gateway->primitivesList) 
							gateway->primitivesList[gateway->dimLists - 1] = strdup(PRIMITIVE_NAME);

						if (gateway->primiviteIdList)
							gateway->primiviteIdList[gateway->dimLists - 1] = PRIMITIVE_ID;
					}
				}
				if (PRIMITIVE_NAME) {FREE(PRIMITIVE_NAME); PRIMITIVE_NAME =NULL;}
				GATEWAY_ID = 0;
				PRIMITIVE_ID = 0;
			}
		}
		else
		{
			fprintf(stderr,_("Error: Not a valid gateway file %s (should start with <GATEWAY> and contain <PRIMITIVE gatewayId='' primitiveId='' primitiveName=''>)\n"), filenameXml);
		}

		if(xpathObj) xmlXPathFreeObject(xpathObj);
		if(xpathCtxt) xmlXPathFreeContext(xpathCtxt);
		xmlFreeDoc (doc);
	}
	else
	{
		fprintf(stderr,_("Error: Not a valid gateway file %s (encoding not 'utf-8') Encoding '%s' found\n"), filenameXml, encoding);
	}

	if (encoding) {FREE(encoding);encoding=NULL;}

	return gateway;
}
/*--------------------------------------------------------------------------*/
static char *getModuleXmlFilename(char *modulename)
{
	char *filename_module = NULL;
	
	if (modulename)
	{
		char *SciPath = NULL;
		SciPath = getSCIpath();
		if (SciPath)
		{
			int length_filename_module = 0;

			length_filename_module = (int)strlen(FORMATGATEWAYFILENAME) + 
			                   (int)strlen(SciPath)+((int)strlen(modulename)*2)+3;

			filename_module = (char*)MALLOC((length_filename_module+1)* 
							   sizeof(char));

			if (filename_module)
			{
				sprintf(filename_module,FORMATGATEWAYFILENAME,
					    SciPath,modulename,modulename);

				/* file doesn't exist */
				if ( !FileExist(filename_module) )
				{
					FREE(filename_module);
					filename_module = NULL;
				}
			}
			FREE(SciPath); SciPath = NULL;
		}
	}
	return filename_module;
}
/*--------------------------------------------------------------------------*/
