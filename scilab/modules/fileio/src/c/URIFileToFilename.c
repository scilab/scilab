/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "URIFileToFilename.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
#define strnicmp _strnicmp
#else
#define strnicmp strncasecmp
#endif
/*-----------------------------------------------------------------------------------*/
#define URI_BEGIN "file://"
/*-----------------------------------------------------------------------------------*/
char *URIFileToFilename(char *uri)
{
	char *filename = NULL;

	if (uri)
	{
		if (isURIFile(uri))
		{
			int pos = (int) strlen(URI_BEGIN);
			filename = (char*)MALLOC(sizeof(char)*(strlen(uri)+1));
			if (filename) strcpy(filename,&uri[pos]);
		}
		else
		{
			filename = (char*)MALLOC(sizeof(char)*(strlen(uri)+1));
			if (filename) strcpy(filename,uri);
		}
	}
	return filename;
}
/*-----------------------------------------------------------------------------------*/
BOOL isURIFile(char *uri)
{
	BOOL bOK = FALSE;
	if (uri)
	{
		if (strlen(uri) > strlen(URI_BEGIN))
		{
			if ( strnicmp(uri,URI_BEGIN,strlen(URI_BEGIN)) == 0)
			{
				bOK = TRUE;
			}
		}
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
