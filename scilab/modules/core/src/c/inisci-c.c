#ifdef _MSC_VER
#include <windows.h>
#else
#include <stdio.h>
#include <sys/utsname.h>
#endif

#include <string.h>
#include <stdlib.h> /* getenv ! */
#include "machine.h" 
#include "Os_specific.h"
#include "core_math.h" 
#include "setgetSCIpath.h"
#include "MALLOC.h"

/*************************************************************************************************/
#ifdef _MSC_VER
	extern BOOL FileExist(char *filename);
	BOOL ExistScicos(void);
	BOOL ExistModelicac(void);
	BOOL ExistJavaSciWin(void);
#endif

static BOOL WITH_GUI=TRUE;
/*************************************************************************************************/
void SetWITH_GUI(int ON);
BOOL  GetWITH_GUI(void);
/*************************************************************************************************/
extern int C2F(withtk)(int *rep);
/*************************************************************************************************/
/*-------------------------------------------
 *  get configure options used for compilation 
 *  used in inisci.f
 *  Copyright ENPC/ Jean-Philipe Chancelier 
 *-------------------------------------------*/
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
  *rep =0; 
  return 0;
}
/*************************************************************************************************/
int C2F(withscicos)(int *rep)
{ 
	#ifdef _MSC_VER
		*rep = ExistScicos(); 
	#else
		#ifdef WITHOUT_SCICOS
			*rep =0; 
		#else 
			*rep =1; 
		#endif 
	#endif
	return 0;
}

/*************************************************************************************************/
int C2F(withocaml)(int *rep)
{
#ifdef _MSC_VER
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
#ifdef _MSC_VER
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
int C2F(withmsdos)(int *rep)
{
#ifdef _MSC_VER
	*rep =1;
#else
	*rep =0; 
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

/**
 * Set the SCI path and initialize the scilab environement path
 *
 */
int SetSci()
{
#ifdef _MSC_VER
#define PATH_MAX 1024
#endif

	int ierr,iflag=0;
	int lbuf=PATH_MAX;
	char *buf=MALLOC(PATH_MAX*sizeof(char));
	C2F(getenvc)(&ierr,"SCI",buf,&lbuf,&iflag);

	if ( ierr== 1) 
	{
		#if  _MSC_VER
		MessageBox(NULL,"SCI not defined.","Warning",MB_ICONWARNING);
		#else
		printf("SCI not defined.\n");
		#endif
		exit(1);
	}
	setSCIpath(buf);
	if (buf) {FREE(buf);buf=NULL;}
	return 0;
}

/*************************************************************************************************/

/**
 * Get the SCI path and initialize the scilab environement path
 *
 */
int C2F(getsci)(char *buf,int *nbuf,long int lbuf)
{
	SetSci();
	strcpy(buf,getSCIpath());

	*nbuf = strlen(buf);
	return 0;
}
/*************************************************************************************************/
int C2F(gettmpdir)(char *buf,int *nbuf,long int lbuf)
{
	int ierr,iflag=0,l1buf=lbuf;
	C2F(getenvc)(&ierr,"TMPDIR",buf,&l1buf,&iflag);
	if ( ierr== 1) 
	{
#if  _MSC_VER
		MessageBox(NULL,"TMPDIR not defined.","Warning",MB_ICONWARNING);
#else
		printf("TMPDIR not defined.\n");
#endif
		exit(1);
	}
	*nbuf = strlen(buf);
	return 0;
}
/*************************************************************************************************/
#ifdef _MSC_VER
BOOL ExistModelicac(void)
{
	#define ModelicacName "/modules/scicos/bin/modelicac.exe"

	BOOL bOK=FALSE;
	char *SCIPATH = (char*)getSCIpath();
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
	char *SCIPATH = (char*)getSCIpath();
	char *fullpathJavaSci=NULL;
	
	fullpathJavaSci=(char*)MALLOC((strlen(SCIPATH)+strlen(JavaSCIName)+1)*sizeof(char));
	wsprintf(fullpathJavaSci,"%s%s",SCIPATH,JavaSCIName);
	bOK=FileExist(fullpathJavaSci);
	if (fullpathJavaSci) FREE(fullpathJavaSci);
	return bOK;
}
/*************************************************************************************************/
BOOL ExistScicos(void)
{
	#define ScicosMacrosDirectory "/modules/scicos/macros/scicos"

	BOOL bOK=FALSE;
	char *SCIPATH = (char*)getSCIpath();
	char *fullpathScicos=NULL;
	
	fullpathScicos=(char*)MALLOC((strlen(SCIPATH)+strlen(ScicosMacrosDirectory)+1)*sizeof(char));
	wsprintf(fullpathScicos,"%s%s",SCIPATH,ScicosMacrosDirectory);
	bOK=FileExist(fullpathScicos);
	if (fullpathScicos) FREE(fullpathScicos);
	return bOK;
}
#endif
/*************************************************************************************************/
int ExistJavaSciUnix(void)
{
	int bOK=0;
	char *SCIPATH = (char*)getSCIpath();
	char *fullpathJavaSci=NULL;

#ifndef _MSC_VER
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
