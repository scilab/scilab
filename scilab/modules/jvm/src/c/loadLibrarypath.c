/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include <libxml/xpath.h>
#include <libxml/xmlreader.h>
#include "loadLibrarypath.h"
#include "GetXmlFileEncoding.h"
#include "../../fileio/includes/FileExist.h"
#include "addToLibrarypath.h"
#include "setgetSCIpath.h"
#include "MALLOC.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/ 
BOOL LoadLibrarypath(char *xmlfilename)
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
			char *libraryPath=NULL;

			doc = xmlParseFile (xmlfilename);

			if (doc == NULL) 
			{
				printf(_("Error: could not parse file %s\n"), xmlfilename);
				if (encoding) {FREE(encoding);encoding=NULL;}
				return bOK;
			}

			xpathCtxt = xmlXPathNewContext(doc);
			xpathObj = xmlXPathEval((const xmlChar*)"//librarypaths/librarypath", xpathCtxt);

			if(xpathObj && xpathObj->nodesetval->nodeMax) 
			{
				/* the Xpath has been understood and there are node */
				int	i;
				for(i = 0; i < xpathObj->nodesetval->nodeNr; i++)
				{

					xmlAttrPtr attrib=xpathObj->nodesetval->nodeTab[i]->properties;
					/* Get the properties of <librarypath>  */
					while (attrib != NULL)
					{
						/* loop until when have read all the attributes */
						if (xmlStrEqual (attrib->name, (const xmlChar*) "path"))
						{ 
							/* we found the tag primitiveName */
							const char *str=(const char*)attrib->children->content;
							libraryPath=(char*)MALLOC(sizeof(char)*(strlen((const char*)str)+1));
							strcpy(libraryPath,str);
						}
						attrib = attrib->next;
					}

					if ( (libraryPath) && (strlen(libraryPath) > 0) )
					{
						#define KEYWORDSCILAB "$SCILAB" 
						char firstchars[8];
						char *sciPath=getSCIpath();
						
						strncpy(firstchars,libraryPath,strlen(KEYWORDSCILAB));
						firstchars[strlen(KEYWORDSCILAB)]='\0';

						if (strcmp(firstchars,KEYWORDSCILAB)==0)
						{
							char *modifypath = (char*)MALLOC(sizeof(char)*(strlen(sciPath)+strlen(libraryPath)+1));
							strcpy(modifypath,sciPath);
							strcat(modifypath,&libraryPath[strlen(KEYWORDSCILAB)]);
							FREE(libraryPath);
							libraryPath = modifypath;
						}

  					    if (sciPath) {FREE(sciPath);sciPath=NULL;}
                        addToLibrarypath(libraryPath);
						FREE(libraryPath);
						libraryPath = NULL;
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
			printf(_("Error : Not a valid path file %s (encoding not 'utf-8') Encoding '%s' found\n"), xmlfilename, encoding);
		}
		if (encoding) {FREE(encoding);encoding=NULL;}
	}
	return bOK;
}
/*--------------------------------------------------------------------------*/ 
