/*-----------------------------------------------------------------------------------*/
/* Scilab */
/*-----------------------------------------------------------------------------------*/
#include <stdio.h>
#include "machine.h"
#include "../../../io/includes/directories.h"
#include <string.h>
#include "MALLOC.h" /* MALLOC */

#ifdef _MSC_VER
#include <windows.h>
#endif
#if defined(__STDC__) || defined(_MSC_VER)
#include <stdlib.h>
#ifndef _MSC_VER
#include <sys/types.h>
#include <unistd.h>
#endif
#else 
extern  char  *getenv();
#endif

#ifdef _MSC_VER
#define putenv(x) _putenv(x)
#endif

#ifdef _MSC_VER
#include <process.h>
#endif
#include "prompt.h"
#include "tmpdir.h"
/*-----------------------------------------------------------------------------------*/
static char tmp_dir[256],buf[256];
/*-----------------------------------------------------------------------------------*/
/**
* creates a tmp dir for a scilab session 
* and fixes the TMPDIR env variable
*/
void C2F(settmpdir)(void)
{
#ifdef _MSC_VER
	char TmpDirDefault[PATH_MAX];
#endif
  static int first =0;
  if ( first == 0 ) 
    {
      first++;
#ifdef _MSC_VER 
	  if (!GetTempPath(PATH_MAX,TmpDirDefault))
	  {
		  MessageBox(NULL,"Don''t find Windows temporary directory","Error",MB_ICONERROR);
		  exit(1);
	  }
	  else
	  {
		  sprintf(tmp_dir,"%sSCI_TMP_%d_",TmpDirDefault,(int) _getpid());
	  }

	  if ( CreateDirectory(tmp_dir,NULL)==FALSE)
      {
		DWORD attribs=GetFileAttributes (tmp_dir); 

		if (attribs & FILE_ATTRIBUTE_DIRECTORY)
		{
		  /* Repertoire existant */
		}
		else
		{
		  #ifdef _DEBUG
		  char MsgErr[1024];
		  wsprintf(MsgErr,"Impossible to create : %s",tmp_dir);
		  MessageBox(NULL,MsgErr,"Error",MB_ICONERROR);
		  exit(1);
		  #else
		  GetTempPath(PATH_MAX,TmpDirDefault);
		  sprintf(tmp_dir,"%s",TmpDirDefault);
		  tmp_dir[strlen(tmp_dir)-1]='\0'; /* Remove last \ */
		  #endif
		}
  }
#else 
  sprintf(tmp_dir,"/tmp/SD_%d_",(int) getpid());
  CreateDir(tmp_dir) ;
#endif 
  sprintf(buf,"TMPDIR=%s",tmp_dir);
  putenv(buf);
    }
}
/*-----------------------------------------------------------------------------------*/
/**
* get a reference to tmp_dir 
* @return the name of the directory
*/
char *get_sci_tmp_dir(void)
{
  /* just in case */
  C2F(settmpdir)();
  return tmp_dir;
}
/*-----------------------------------------------------------------------------------*/
/**
* remove TMPDIR directory
*/
void C2F(tmpdirc)(void)
{
  char *tmp_dir2 = get_sci_tmp_dir(); 
  RemoveDir(tmp_dir2);
}
/*-----------------------------------------------------------------------------------*/
char *getTMPDIR(void)
{
	char *TMPDIR=NULL;
	TMPDIR=(char*)MALLOC(sizeof(char)*(strlen(tmp_dir)+1));
	strcpy(TMPDIR,tmp_dir);

	return TMPDIR;
}
/*-----------------------------------------------------------------------------------*/
