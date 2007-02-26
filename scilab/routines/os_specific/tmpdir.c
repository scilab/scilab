/* Copyright INRIA/ENPC */

#include "../machine.h"
#include <stdio.h>

#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

#if WIN32
#include <windows.h>
#endif
#if defined(__STDC__) || defined(__MSC__) || defined(__CYGWIN__)
#include <stdlib.h>
#ifndef WIN32
#include <sys/types.h>
#include <unistd.h>
#endif
#else 
extern  char  *getenv();
#endif

#if (defined __MSC__) || defined(__MINGW32__) 
#define putenv(x) _putenv(x)
#endif

#if defined(__MSC__)
#include <process.h>
#endif

extern void C2F(setprlev) __PARAMS((int*));


static char tmp_dir[256],buf[256];


/****************************
 * creates a tmp dir for a scilab session 
 * and fixes the TMPDIR env variable
 ****************************/

void C2F(settmpdir)(void)
{
#if WIN32
	#define PATH_MAX 1024
	char TmpDirDefault[PATH_MAX];
#endif
  static int first =0;
  if ( first == 0 ) 
    {
      first++;
#ifdef WIN32 
	  if (!GetTempPath(PATH_MAX,TmpDirDefault))
	  {
		  MessageBox(NULL,"Don''t find Windows temporary directory","Error",MB_ICONERROR);
		  exit(1);
	  }
	  else
	  {
		  sprintf(tmp_dir,"%sSCI_TMP_%d_",TmpDirDefault,(int) getpid());
	  }

	  if ( CreateDirectory(tmp_dir,NULL)==FALSE)
      {
		DWORD attribs=GetFileAttributes (tmp_dir); 

		if (attribs & FILE_ATTRIBUTE_DIRECTORY)
		{
		  // Repertoire existant
		}
		else
		{
		  #ifdef _DEBUG
		  char MsgErr[1024];
		  wsprintf(MsgErr,"Impossible to create : %s",tmp_dir);
		  MessageBox(NULL,MsgErr,"Error",MB_ICONERROR);
		  exit(1);
		  #endif

		  GetTempPath(PATH_MAX,TmpDirDefault);
		  sprintf(tmp_dir,"%s",TmpDirDefault);
		  tmp_dir[strlen(tmp_dir)-1]='\0'; /* Remove last \ */

		}
  }
#else 
  sprintf(tmp_dir,"/tmp/SD_%d_",(int) getpid());
  sprintf(buf,"umask 000;if test ! -d %s; then mkdir %s; fi ",tmp_dir,tmp_dir);
  system(buf);
#endif 
  sprintf(buf,"TMPDIR=%s",tmp_dir);
  putenv(buf);
    }
}

/****************************
 * get a reference to tmp_dir 
 ****************************/

char *get_sci_tmp_dir(void)
{
  /* just in case */
  C2F(settmpdir)();
  return tmp_dir;
}


#if WIN32
/* Remove directory and subdirectories */
/* A.C INRIA 2005 */
int DeleteDirectory(char *refcstrRootDirectory)
{
	BOOL bDeleteSubdirectories=TRUE;
	BOOL bSubdirectory = FALSE;
	HANDLE hFile;
	char *strFilePath=NULL;
	char *strPattern=NULL;
	WIN32_FIND_DATA FileInformation;
	DWORD dwError;

	strPattern = MALLOC(sizeof(char)*(strlen(refcstrRootDirectory)+5));
	sprintf(strPattern,"%s\\*.*",refcstrRootDirectory);

	hFile = FindFirstFile(strPattern, &FileInformation);
	if (strPattern) { FREE(strPattern);strPattern=NULL;}

	if(hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(FileInformation.cFileName[0] != '.')
			{
				if (strFilePath) {FREE(strFilePath);strFilePath=NULL;}
				strFilePath = MALLOC(sizeof(char)*(strlen(refcstrRootDirectory)+5+strlen(FileInformation.cFileName)));
				sprintf(strFilePath,"%s\\%s",refcstrRootDirectory,FileInformation.cFileName);

				if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if(bDeleteSubdirectories)
					{
						int iRC = DeleteDirectory(strFilePath);
						if(iRC) return iRC;
					}
					else bSubdirectory = TRUE;
				}
				else
				{
					if(SetFileAttributes(strFilePath,FILE_ATTRIBUTE_NORMAL) == FALSE) return GetLastError();
					if(DeleteFile(strFilePath) == FALSE) return GetLastError();
				}
			}
		} while(FindNextFile(hFile, &FileInformation) == TRUE);

		FindClose(hFile);

		dwError = GetLastError();
		if(dwError != ERROR_NO_MORE_FILES) return dwError;
		else
		{
			if(!bSubdirectory)
			{
				if(SetFileAttributes(refcstrRootDirectory,FILE_ATTRIBUTE_NORMAL) == FALSE) return GetLastError();
				if(RemoveDirectory(refcstrRootDirectory) == FALSE)	return GetLastError();
			}
		}
	}

	if (strFilePath) {FREE(strFilePath);strFilePath=NULL;}

	return 0;
}
#endif

/*************************************************
 * remove TMPDIR and dynamic link temporary files 
 *************************************************/

#if (defined(hppa))
  extern void hppa_sci_unlink_shared();
#endif

void C2F(tmpdirc)(void)
{
  char *tmp_dir = get_sci_tmp_dir(); 
#ifdef WIN32 
  DeleteDirectory(tmp_dir);
#else 
#if (defined(hppa))
  hppa_sci_unlink_shared();
#endif
  sprintf(buf,"rm -f -r %s >/dev/null  2>/dev/null",tmp_dir);
  system(buf);
  sprintf(buf,"rm -f -r /tmp/%d.metanet.* > /dev/null  2>/dev/null",
	  (int) getpid());
  system(buf);
#endif 
}
