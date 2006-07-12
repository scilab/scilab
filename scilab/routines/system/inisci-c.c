#ifdef WIN32
#include <windows.h>
#else
#include <stdio.h>
#include <sys/utsname.h>
#endif

#include <string.h>
#include <stdlib.h> /* getenv ! */
#include "../machine.h" 
#include "../os_specific/Os_specific.h"
#include "../graphics/Math.h" 

#ifdef WIN32
#include "../os_specific/win_mem_alloc.h"  /* malloc */
#else
#include "../os_specific/sci_mem_alloc.h"  /* malloc */
#endif


/*************************************************************************************************/
#ifndef BOOL
typedef int BOOL;
#endif

#ifndef TRUE
 #define TRUE 1
#endif

#ifndef FALSE
 #define FALSE 0
#endif
/*************************************************************************************************/
#ifdef WIN32
	BOOL FileExist(char *filename);
	BOOL ExistModelicac(void);
	BOOL ExistJavaSciWin(void);
#endif

static BOOL WITH_GUI=TRUE;
/*************************************************************************************************/
void SetWITH_GUI(int ON);
BOOL  GetWITH_GUI(void);
/*************************************************************************************************/
/*-------------------------------------------
 *  get configure options used for compilation 
 *  used in inisci.f
 *  Copyright ENPC/ Jean-Philipe Chancelier 
 *-------------------------------------------*/
/*************************************************************************************************/

int C2F(withtk)(int *rep)
{ 
#ifdef WITH_TK
  *rep =1; 
#else 
  *rep =0; 
#endif 
  return 0;
}
/*************************************************************************************************/
int C2F(withpvm)(int *rep)
{ 
#ifdef WITH_PVM
  *rep =1; 
#else 
  *rep =0; 
#endif 
  return 0;
}
/*************************************************************************************************/
int C2F(withgtk)(int *rep)
{ 
#ifdef WITH_GTK
  *rep =1; 
#else 
  *rep =0; 
#endif 
  return 0;
}
/*************************************************************************************************/
int C2F(withocaml)(int *rep)
{
#ifdef WIN32
	if (ExistModelicac())
	{
		*rep =1;
	}
	else
	{
		*rep =0; 
	}
#else
	#ifdef WITH_OCAML
		*rep =1; 
	#else 
		*rep =0; 
	#endif 
#endif
  return 0;
}
/*************************************************************************************************/
int C2F(withjavasci)(int *rep)
{
#ifdef WIN32
	if (ExistJavaSciWin())
	{
		*rep =1;
	}
	else
	{
		*rep =0; 
	}
#else
	if (ExistJavaSciUnix())
	{
		*rep =1;
	}
	else
	{
		*rep =0; 
	}
#endif
  return 0;
}
/*************************************************************************************************/
int C2F(getcomp)(char *buf,int *nbuf,long int lbuf)
{
  int ierr,iflag=0,l1buf=lbuf;
  C2F(getenvc)(&ierr,"COMPILER",buf,&l1buf,&iflag);
  if ( ierr== 1) strncpy(buf,"NO",lbuf);
  *nbuf = strlen(buf);
  return 0;
}
/*************************************************************************************************/
#ifdef WIN32
BOOL FileExist(char *filename)
{
     BOOL retour=FALSE;	
	
     WIN32_FIND_DATA FindFileData;
     HANDLE handle = FindFirstFile (filename, &FindFileData);
     if (handle != INVALID_HANDLE_VALUE)
     {
         FindClose (handle);
         retour=TRUE;
     }
     else retour=FALSE;

     return retour;
}
/*************************************************************************************************/
BOOL ExistModelicac(void)
{
	#define ModelicacName "/bin/modelicac.exe"

	BOOL bOK=FALSE;
	char *SCIPATH = (char*)getenv ("SCI");
	char *fullpathModelicac=NULL;
	
	fullpathModelicac=(char*)MALLOC((strlen(SCIPATH)+strlen(ModelicacName)+1)*sizeof(char));
	wsprintf(fullpathModelicac,"%s%s",SCIPATH,ModelicacName);
	bOK=FileExist(fullpathModelicac);
	if (fullpathModelicac) FREE(fullpathModelicac);
	return bOK;
}
/*************************************************************************************************/
BOOL ExistJavaSciWin(void)
{
	#define JavaSCIName "/bin/javasci.dll"

	BOOL bOK=FALSE;
	char *SCIPATH = (char*)getenv ("SCI");
	char *fullpathJavaSci=NULL;
	
	fullpathJavaSci=(char*)MALLOC((strlen(SCIPATH)+strlen(JavaSCIName)+1)*sizeof(char));
	wsprintf(fullpathJavaSci,"%s%s",SCIPATH,JavaSCIName);
	bOK=FileExist(fullpathJavaSci);
	if (fullpathJavaSci) FREE(fullpathJavaSci);
	return bOK;
}
#endif
/*************************************************************************************************/
int ExistJavaSciUnix(void)
{
	int bOK=0;
	char *SCIPATH = (char*)getenv ("SCI");
	char *fullpathJavaSci=NULL;

#ifndef WIN32
	#define JavaSciName "libjavasci"

	struct utsname uname_pointer;
	FILE *fp;

	char OperatinSystem[256];
	char Release[256];
	char extension[5];

	uname(&uname_pointer);
	sprintf(OperatinSystem,"%s",uname_pointer.sysname);
	sprintf(Release,"%s",uname_pointer.release);

	if ( strcmp(OperatinSystem,"HP-UX") == 0 )
	{
		strcpy(extension,".sl");
	}
	else
	{
		strcpy(extension,".so");
	}
	fullpathJavaSci=(char*)MALLOC((strlen(SCIPATH)+strlen("/bin/")+strlen(JavaSciName)+strlen(extension)+1)*sizeof(char));
	sprintf(fullpathJavaSci,"%s/bin/%s%s",SCIPATH,JavaSciName,extension);
	fp=fopen(fullpathJavaSci,"r");
	if (fp)
	{
		fclose(fp);
		bOK=1;
	}
	else
	{
		bOK=0;
	}
	if (fullpathJavaSci) FREE(fullpathJavaSci);
#endif
	return bOK;
}
/*************************************************************************************************/
int C2F(withgui)(int *rep)
{ 
	*rep = WITH_GUI; 
	return 0;
}
/*************************************************************************************************/
void SetWITH_GUI(BOOL ON)
{
	if ( (ON == FALSE) || (ON == TRUE) ) WITH_GUI=ON;
}
/*************************************************************************************************/
BOOL  GetWITH_GUI(void)
{
	return WITH_GUI;
}
/*************************************************************************************************/
