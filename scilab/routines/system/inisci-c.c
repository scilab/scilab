#include <string.h>
#include "../machine.h" 
#include "../sun/Sun.h" 

#ifdef WIN32
#include <windows.h>
BOOL FileExist(char *filename);
BOOL ExistModelicac(void);
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
	free(fullpathModelicac);
	return bOK;
}
#endif
/*************************************************************************************************/