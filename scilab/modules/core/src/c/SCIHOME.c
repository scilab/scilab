/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <stdio.h>
#include "SCIHOME.h"
#include "MALLOC.h"
#include "version.h"
#include "../../../io/includes/createdir.h"
/*-----------------------------------------------------------------------------------*/
#ifndef MAX_PATH
#define MAX_PATH 260
#endif
/*-----------------------------------------------------------------------------------*/
static char SCIHOMEPATH[MAX_PATH*2]="empty_SCIHOME";
/*-----------------------------------------------------------------------------------*/
extern void C2F(getenvc)(int *ierr,char *var,char *buf,int *buflen,int *iflag);
/*-----------------------------------------------------------------------------------*/
BOOL setSCIHOME(void)
{
	BOOL bOK=FALSE;
	int ierr=0;
	int buflen=MAX_PATH;
	int iflag=0;
	
	char SCIHOME[MAX_PATH];
	char USERPATHSCILAB[MAX_PATH];
	
	C2F(getenvc)(&ierr,"SCIHOME",SCIHOME,&buflen,&iflag);

	if (ierr) /* SCIHOME not define */
	{
		#if _MSC_VER
			#define DIRSEPARATOR "\\"
			#define BASEDIR "Scilab"
		#else
			#define DIRSEPARATOR "/"
			#define BASEDIR ".Scilab"
		#endif

		char USERHOMESYSTEM[MAX_PATH];
		iflag=1;

		#if _MSC_VER
			C2F(getenvc)(&ierr,"USERPROFILE",USERHOMESYSTEM,&buflen,&iflag);
		#else
			C2F(getenvc)(&ierr,"HOME",USERHOMESYSTEM,&buflen,&iflag);
		#endif


		if (ierr) {	return bOK; }
		else
		{
			sprintf(USERPATHSCILAB,"%s%s%s",USERHOMESYSTEM,DIRSEPARATOR,BASEDIR);
			sprintf(SCIHOMEPATH,"%s%s%s",USERPATHSCILAB,DIRSEPARATOR,SCI_VERSION_STRING);
		}
	}
	else
	{
		strcpy(SCIHOMEPATH,SCIHOME);
	}

	if (!ExistDir(SCIHOMEPATH))
	{
		if(!ExistDir(USERPATHSCILAB)) CreateDir(USERPATHSCILAB);
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
