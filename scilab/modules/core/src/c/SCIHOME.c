/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SCIHOME.h"
#include "MALLOC.h"
#include "version.h"
#include "machine.h"

#include "../../../io/includes/directories.h"
#include "../../../io/includes/getenvc.h"
/*-----------------------------------------------------------------------------------*/
#if (defined _MSC_VER) 
#undef putenv
#define putenv(x) _putenv(x)
#endif
/*-----------------------------------------------------------------------------------*/
static char SCIHOMEPATH[PATH_MAX*2]="empty_SCIHOME";
/*-----------------------------------------------------------------------------------*/
BOOL setSCIHOME(void)
{
	BOOL bOK=FALSE;
	int ierr=0;
	int buflen=PATH_MAX;
	int iflag=0;
	
	char SCIHOME[PATH_MAX];
	char USERPATHSCILAB[PATH_MAX];
	
	C2F(getenvc)(&ierr,"SCIHOME",SCIHOME,&buflen,&iflag);

	if (ierr) /* SCIHOME not define */
	{
		#ifdef _MSC_VER
			#define BASEDIR "Scilab"
		#else
			#define BASEDIR ".Scilab"
		#endif

		char env[PATH_MAX+1+10];
		char USERHOMESYSTEM[PATH_MAX];
		iflag=1;

		#ifdef _MSC_VER
			C2F(getenvc)(&ierr,"USERPROFILE",USERHOMESYSTEM,&buflen,&iflag);
		#else
			C2F(getenvc)(&ierr,"HOME",USERHOMESYSTEM,&buflen,&iflag);
		#endif


		if (ierr) {return bOK; }
		else
		{
			sprintf(USERPATHSCILAB,"%s%s%s",USERHOMESYSTEM,DIR_SEPARATOR,BASEDIR);
			sprintf(SCIHOMEPATH,"%s%s%s",USERPATHSCILAB,DIR_SEPARATOR,SCI_VERSION_STRING);
		}
		sprintf(env,"SCIHOME=%s",SCIHOMEPATH);
		putenv(env);
	}
	else
	{
		strcpy(SCIHOMEPATH,SCIHOME);
	}

	if (!isdir(SCIHOMEPATH))
	{
		if(!isdir(USERPATHSCILAB)) CreateDir(USERPATHSCILAB);
		if (CreateDir(SCIHOMEPATH))
		{
			bOK=TRUE;
			return bOK;
		}
	}
	else bOK=TRUE;
	
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
char *getSCIHOME(void)
{
	char *retSCIHOME=NULL;
	retSCIHOME=(char*)MALLOC(sizeof(char)*(strlen(SCIHOMEPATH)+1));
	strcpy(retSCIHOME,SCIHOMEPATH);
	return retSCIHOME;
}
/*-----------------------------------------------------------------------------------*/
