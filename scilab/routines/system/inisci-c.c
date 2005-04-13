#ifdef WIN32
#include <windows.h>
#else
#include <stdio.h>
#include <sys/utsname.h>
#endif

#include <string.h>
#include <stdlib.h> /* getenv ! */
#include "../machine.h" 
#include "../sun/Sun.h" 

#ifdef WIN32
	BOOL FileExist(char *filename);
	BOOL ExistModelicac(void);
	BOOL ExistJavaSciWin(void);
#endif


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
	
	fullpathModelicac=(char*)malloc((strlen(SCIPATH)+strlen(ModelicacName)+1)*sizeof(char));
	wsprintf(fullpathModelicac,"%s%s",SCIPATH,ModelicacName);
	bOK=FileExist(fullpathModelicac);
	if (fullpathModelicac) free(fullpathModelicac);
	return bOK;
}
/*************************************************************************************************/
BOOL ExistJavaSciWin(void)
{
	#define JavaSCIName "/bin/javasci.dll"

	BOOL bOK=FALSE;
	char *SCIPATH = (char*)getenv ("SCI");
	char *fullpathJavaSci=NULL;
	
	fullpathJavaSci=(char*)malloc((strlen(SCIPATH)+strlen(JavaSCIName)+1)*sizeof(char));
	wsprintf(fullpathJavaSci,"%s%s",SCIPATH,JavaSCIName);
	bOK=FileExist(fullpathJavaSci);
	if (fullpathJavaSci) free(fullpathJavaSci);
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
	fullpathJavaSci=(char*)malloc((strlen(SCIPATH)+strlen("/bin/")+strlen(JavaSciName)+strlen(extension)+1)*sizeof(char));
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
	if (fullpathJavaSci) free(fullpathJavaSci);
#endif
	return bOK;
}
/*************************************************************************************************/
