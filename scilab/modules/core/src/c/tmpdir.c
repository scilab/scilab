/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdio.h>
#include <string.h>
#include "../../../fileio/includes/removedir.h"
#include "../../../fileio/includes/createdirectory.h"
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
#include "strdup_windows.h"
#endif
#include "PATH_MAX.h"
#include "prompt.h"
#include "tmpdir.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
static char tmp_dir[256],buf[256];
/*--------------------------------------------------------------------------*/
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
		  MessageBox(NULL,gettext("Cannot find Windows temporary directory."),gettext("Error"),MB_ICONERROR);
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
		  wsprintf(MsgErr,gettext("Impossible to create : %s"),tmp_dir);
		  MessageBox(NULL,MsgErr,gettext("Error"),MB_ICONERROR);
		  exit(1);
		  #else
		  GetTempPath(PATH_MAX,TmpDirDefault);
		  strcpy(tmp_dir,TmpDirDefault);
		  tmp_dir[strlen(tmp_dir)-1]='\0'; /* Remove last \ */
		  #endif
		}
  }
#else 
  sprintf(tmp_dir,"/tmp/SD_%d_",(int) getpid());
  createdirectory(tmp_dir) ;
#endif 
  sprintf(buf,"TMPDIR=%s",tmp_dir);
  putenv(buf);
    }
}
/*--------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------*/
/**
* remove TMPDIR directory
*/
void C2F(tmpdirc)(void)
{
  char *tmp_dir2 = get_sci_tmp_dir(); 
  removedir(tmp_dir2);
}
/*--------------------------------------------------------------------------*/
char *getTMPDIR(void)
{
	return strdup(tmp_dir);
}
/*--------------------------------------------------------------------------*/
