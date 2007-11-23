/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
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
			char *CLASSPATH=NULL;

			doc = xmlParseFile (xmlfilename);

			if (doc == NULL) 
			{
				printf(_("Error: could not parse file %s\n"), xmlfilename);
				if (encoding) {FREE(encoding);encoding=NULL;}
				return bOK;
			}

			xpathCtxt = xmlXPathNewContext(doc);
			xpathObj = xmlXPathEval((const xmlChar*)"//classpaths/classpath", xpathCtxt);

			if(xpathObj && xpathObj->nodesetval->nodeMax) 
			{
				/* the Xpath has been understood and there are node */
				int	i;
				for(i = 0; i < xpathObj->nodesetval->nodeNr; i++)
				{

					xmlAttrPtr attrib=xpathObj->nodesetval->nodeTab[i]->properties;
					/* Get the properties of <classpath>  */
					while (attrib != NULL)
					{
						/* loop until when have read all the attributes */
						if (xmlStrEqual (attrib->name, (const xmlChar*) "path"))
						{ 
							/* we found the tag primitiveName */
							const char *str=(const char*)attrib->children->content;
							CLASSPATH=(char*)MALLOC(sizeof(char)*(strlen((const char*)str)+1));
							strcpy(CLASSPATH,str);
						}
						attrib = attrib->next;
					}

					if ( (CLASSPATH) && (strlen(CLASSPATH) > 0) )
					{
						#define KEYWORDSCILAB "$SCILAB" 
						char firstchars[8];
						char *SCIPATH=NULL;
						SCIPATH=getSCIpath();
						
						strncpy(firstchars,CLASSPATH,strlen(KEYWORDSCILAB));
						firstchars[strlen(KEYWORDSCILAB)]='\0';

						if (strcmp(firstchars,KEYWORDSCILAB)==0)
						{
							char *modifypath = (char*)MALLOC(sizeof(char)*(strlen(SCIPATH)+strlen(CLASSPATH)+1));
							strcpy(modifypath,SCIPATH);
							strcat(modifypath,&CLASSPATH[strlen(KEYWORDSCILAB)]);
							FREE(CLASSPATH);
							CLASSPATH = modifypath;
						}

  					    if (SCIPATH) {FREE(SCIPATH);SCIPATH=NULL;}
						addToClasspath(CLASSPATH);
						FREE(CLASSPATH);
						CLASSPATH = NULL;
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
		printf(_("Warning: could not find classpath declaration file %s\n"), xmlfilename);
	}
	return bOK;
}
/*--------------------------------------------------------------------------*/ 
