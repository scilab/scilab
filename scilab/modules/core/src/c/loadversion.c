/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "loadversion.h"
#include "with_module.h"
#include "setgetSCIpath.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
#define FORMATFILENAMEVERSION  "%s/modules/%s/VERSION" 
#define SCI_VERSION_MAJOR_keyword "SCI_VERSION_MAJOR"
#define SCI_VERSION_MINOR_keyword "SCI_VERSION_MINOR"
#define SCI_VERSION_MAINTENANCE_keyword "SCI_VERSION_MAINTENANCE"
#define SCI_VERSION_STRING_keyword "SCI_VERSION_STRING"
#define SCI_VERSION_REVISION_keyword "SCI_VERSION_REVISION"
/*-----------------------------------------------------------------------------------*/ 
extern BOOL FileExist(char *filename);
/*-----------------------------------------------------------------------------------*/ 
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
		len=strlen(FORMATFILENAMEVERSION)+strlen(SciPath)+strlen(modulename)+1;
		filename_VERSION_module=(char*)MALLOC(sizeof(char)*len);
		sprintf(filename_VERSION_module,FORMATFILENAMEVERSION,SciPath,modulename);

		if (FileExist(filename_VERSION_module))
		{
			#define LineMaxSize 1024
			char Line[LineMaxSize];
			FILE *pFile;

			pFile=fopen(filename_VERSION_module,"rt");
			while (fgets(Line, LineMaxSize,pFile))
			{
				int retval=0;
				char SCI_VERSION_TYPE[32];

				retval=sscanf(Line,"%s",SCI_VERSION_TYPE);

				if ( strcmp(SCI_VERSION_TYPE,SCI_VERSION_MAJOR_keyword) == 0 )
				{
					retval=sscanf(Line,"%s %d",SCI_VERSION_TYPE,sci_version_major);
				}
				else if ( strcmp(SCI_VERSION_TYPE,SCI_VERSION_MINOR_keyword) == 0 )
				{
					retval=sscanf(Line,"%s %d",SCI_VERSION_TYPE,sci_version_minor);
				}
				else if ( strcmp(SCI_VERSION_TYPE,SCI_VERSION_MAINTENANCE_keyword) == 0 )
				{
					retval=sscanf(Line,"%s %d",SCI_VERSION_TYPE,sci_version_maintenance);
				}
				else if ( strcmp(SCI_VERSION_TYPE,SCI_VERSION_STRING_keyword) == 0 )
				{
					int j=0;
					strncpy(sci_version_string,&Line[strlen(SCI_VERSION_STRING_keyword)],LineMaxSize);
					for (j=strlen(sci_version_string);j>0;j--)
					{
						if (sci_version_string[j] == '\n')
						{
							sci_version_string[j]='\0';
							break;
						}
					}
				}
				else if ( strcmp(SCI_VERSION_TYPE,SCI_VERSION_REVISION_keyword) == 0 )
				{
					retval=sscanf(Line,"%s %d",SCI_VERSION_TYPE,sci_version_revision);
				}
			}
			fclose(pFile);
			bOK=TRUE;
		}
		if (SciPath){FREE(SciPath);SciPath=NULL;}
		if (filename_VERSION_module) {FREE(filename_VERSION_module);filename_VERSION_module=NULL;}
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
