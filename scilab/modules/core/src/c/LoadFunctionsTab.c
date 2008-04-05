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
#include "MALLOC.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libxml/xpath.h>
#include <libxml/xmlreader.h>
#include "machine.h"
#include "hashtable_core.h"
#include "getmodules.h"
#include "stricmp.h"
#include "setgetSCIpath.h"
#include "LoadFunctionsTab.h"
#include "GetXmlFileEncoding.h"
#include "scilabDefaults.h"
#include "localization.h"
#include "../../fileio/includes/FileExist.h"
/*--------------------------------------------------------------------------*/  
static int firstentry = 0;
/*--------------------------------------------------------------------------*/  
extern int C2F(cvname)(integer *,char *,integer *, unsigned long int);
/*--------------------------------------------------------------------------*/  
static int Add_a_Scilab_primitive_in_hashtable(char *str, int *dataI, int *data);
static BOOL Load_primitives_from_file(char *filename);
/*--------------------------------------------------------------------------*/  
void LoadFunctionsTab(void)
{
	struct MODULESLIST *Modules=NULL;
	char *SciPath=NULL;
	
	int j=0;

	if ( firstentry != 0 ) return;

	SciPath=getSCIpath();
	Modules=getmodules();
	/* We are not freeing Modules in order to speed up the next call of getmodule */
	/* freed in sciquit.c */

	for (j=0;j<Modules->numberofModules;j++)
	{
		char *filename_primitives_list=NULL;
		int len=(int)strlen(FORMATGATEWAYFILENAME)+(int)strlen(SciPath)+(int)strlen(Modules->ModuleList[j])*2;

		filename_primitives_list=(char*)MALLOC((len+1)*sizeof(char));
		sprintf(filename_primitives_list,FORMATGATEWAYFILENAME,SciPath,Modules->ModuleList[j],Modules->ModuleList[j]);

		Load_primitives_from_file(filename_primitives_list);

		if (filename_primitives_list) { FREE(filename_primitives_list);filename_primitives_list=NULL;}
	}

	if (SciPath){FREE(SciPath);SciPath=NULL;}
	firstentry = 1;

}
/*--------------------------------------------------------------------------*/
static int Add_a_Scilab_primitive_in_hashtable(char *str, int *dataI, int *data)
{
	int ldata;
	int id[nsiz];
	int zero=0;
	C2F(cvname)(id,str,&zero,(unsigned long)strlen(str));
	ldata= (*dataI)*100+*data;
	return( action_hashtable_scilab_functions(id,str,&ldata,SCI_HFUNCTIONS_ENTER));
}
/*--------------------------------------------------------------------------*/
static BOOL Load_primitives_from_file(char *filename)
{
	BOOL bOK=FALSE;

	if (FileExist(filename))
	{
		char *encoding=GetXmlFileEncoding(filename);

		/* Don't care about line return / empty line */
		xmlKeepBlanksDefault(0); 

		/* check if the XML file has been encoded with utf8 (unicode) or not */
		if ( stricmp("utf-8", encoding)==0) {
			xmlDocPtr doc;
			xmlXPathContextPtr xpathCtxt = NULL;
			xmlXPathObjectPtr xpathObj = NULL;


			int GATEWAY_ID=0;
			int PRIMITIVE_ID=0;
			char *PRIMITIVE_NAME=NULL;

			doc = xmlParseFile (filename);

			if (doc == NULL) 
			{
				fprintf(stderr,_("Error: could not parse file %s\n"), filename);
				if (encoding) {FREE(encoding);encoding=NULL;}
				return FALSE;
			}

			xpathCtxt = xmlXPathNewContext(doc);
			xpathObj = xmlXPathEval((const xmlChar*)"//GATEWAY/PRIMITIVE", xpathCtxt);

			if(xpathObj && xpathObj->nodesetval->nodeMax) 
			{
				/* the Xpath has been understood and there are node */
				int	i;
				for(i = 0; i < xpathObj->nodesetval->nodeNr; i++)
				{
					xmlAttrPtr attrib=xpathObj->nodesetval->nodeTab[i]->properties;
					/* Get the properties of <PRIMITIVE>  */
					while (attrib != NULL)
					{
						/* loop until when have read all the attributes */
						if (xmlStrEqual (attrib->name, (const xmlChar*) "gatewayId"))
						{ 
							/* we found the tag gatewayId */
							const char *str=(const char*)attrib->children->content;
							GATEWAY_ID=atoi(str);
						}
						else if (xmlStrEqual (attrib->name, (const xmlChar*)"primitiveId"))
						{ 
							/* we found the tag primitiveId */
							const char *str=(const char*)attrib->children->content;
							PRIMITIVE_ID=atoi(str);
						}
						else if (xmlStrEqual (attrib->name, (const xmlChar*)"primitiveName"))
						{
							/* we found the tag primitiveName */
							const char *str=(const char*)attrib->children->content;
							PRIMITIVE_NAME=(char*)MALLOC(sizeof(char)*(strlen((const char*)str)+1));
							strcpy(PRIMITIVE_NAME,str);
						}
						attrib = attrib->next;
					}

					if ( (GATEWAY_ID != 0) && (PRIMITIVE_ID != 0) && (PRIMITIVE_NAME) )
					{
						if (strlen(PRIMITIVE_NAME) > 0)
						{
							Add_a_Scilab_primitive_in_hashtable(PRIMITIVE_NAME,&GATEWAY_ID,&PRIMITIVE_ID);
						}
					}

					if (PRIMITIVE_NAME) {FREE(PRIMITIVE_NAME); PRIMITIVE_NAME =NULL;}
					GATEWAY_ID = 0;
					PRIMITIVE_ID = 0;
				}
			}
			else
			{
				fprintf(stderr,_("Error : Not a valid gateway file %s (should start with <GATEWAY> and contain <PRIMITIVE gatewayId='' primitiveId='' primitiveName=''>)\n"), filename);
				return FALSE;
			}
			if(xpathObj) xmlXPathFreeObject(xpathObj);
			if(xpathCtxt) xmlXPathFreeContext(xpathCtxt);
			xmlFreeDoc (doc);

			/*
			* Cleanup function for the XML library.
			*/
			xmlCleanupParser();
		}
		else
		{
			fprintf(stderr,_("Error : Not a valid gateway file %s (encoding not 'utf-8') Encoding '%s' found\n"), filename, encoding);
		}

		if (encoding) {FREE(encoding);encoding=NULL;}
	}
	return bOK;
}
/*--------------------------------------------------------------------------*/ 
