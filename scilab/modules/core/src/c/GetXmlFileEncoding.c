/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "GetXmlFileEncoding.h"
#include "libxml/xmlreader.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/ 
char *GetXmlFileEncoding(const char *filename)
{
#define DEFAULT_ENCODING "UTF-8"
	char *encoding=NULL;
	xmlDocPtr doc = NULL;

	/* default */
	encoding=(char *)MALLOC(sizeof(char)*(strlen(DEFAULT_ENCODING)+1));
	strcpy(encoding,DEFAULT_ENCODING);

	doc = xmlParseFile (filename);
	if (doc) 
	{
		if (doc->encoding)
		{
			if (encoding) {FREE(encoding);encoding=NULL;}
			encoding=(char *)MALLOC(sizeof(char)*(strlen(doc->encoding)+1));
			strcpy(encoding,doc->encoding);
		}
	}

	xmlFreeDoc (doc);
	return encoding;
}
/*--------------------------------------------------------------------------*/ 
