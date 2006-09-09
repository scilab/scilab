/***********************************************************************/
/* Copyright (C) 2005 INRIA Allan CORNET */
/***********************************************************************/
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
/***********************************************************************/
typedef int (*MYPROC) (HINSTANCE , HWND , char *, char *); 
typedef BOOL (*MYPROC2) (void);
#define MAXSTR 256
/***********************************************************************/
char *GetScilabDirectory(BOOL UnixStyle);
char *GetScilabDllDirectory(BOOL UnixStyle);
void AddToScilabDllPathEnv();
/***********************************************************************/
int main (int argc, char *argv[])
{
	HINSTANCE hinstLib; 
	
    MYPROC gp_printfile; 
	MYPROC2 ConfigurePrinterDialogBox; 
    BOOL fFreeResult, fRunTimeLinkSuccess = FALSE; 

	AddToScilabDllPathEnv();
	
	hinstLib = LoadLibrary(TEXT("LibScilab")); 	
 
    if (hinstLib != NULL) 
    { 
        gp_printfile = (MYPROC) GetProcAddress(hinstLib, TEXT("gp_printfile")); 
		ConfigurePrinterDialogBox = (MYPROC2) GetProcAddress(hinstLib, TEXT("ConfigurePrinterDialogBox")); 
 
        if ( (NULL != gp_printfile) && (NULL != ConfigurePrinterDialogBox) )
        {
		HWND parent;
		HANDLE x = GetModuleHandleA (0);
						
		fRunTimeLinkSuccess = TRUE;
		parent = GetActiveWindow ();
						
		ConfigurePrinterDialogBox();
		
		if (argc == 2)
			(gp_printfile) (x, parent, argv[1], (char *) 0);
		else
			(gp_printfile) (x, parent, (char *) 0, (char *) 0);

        }
		else
		{
			MessageBox(NULL,"Lpr : Libscilab.dll Incorret Version !","Warning",MB_ICONERROR); 
		}

        fFreeResult = FreeLibrary(hinstLib); 
    } 
 
    if (! fRunTimeLinkSuccess) MessageBox(NULL,"Lpr : Libscilab.dll not found !","Warning",MB_ICONERROR); 
	
    return (0);
}
/***********************************************************************/
char *GetScilabDirectory(BOOL UnixStyle)
{
	LPSTR ScilabModuleName=NULL;
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	char *SciPathName=NULL;
	char *DirTmp=NULL;

	int index = 0;

	ScilabModuleName = (LPSTR) malloc (MAXSTR + 1);

	if (!GetModuleFileName ((HINSTANCE)GetModuleHandle(NULL), (LPSTR) ScilabModuleName, MAXSTR))
	{
		if (ScilabModuleName) {free(ScilabModuleName);ScilabModuleName=NULL;}
		return NULL;
	}

	_splitpath(ScilabModuleName,drive,dir,fname,ext);
	if (ScilabModuleName) {free(ScilabModuleName);ScilabModuleName=NULL;}
	if (dir[strlen(dir)-1] == '\\') dir[strlen(dir)-1] = '\0';

	for (index=0;index<3;index++)
	{
		DirTmp=strrchr (dir, '\\');
		if (DirTmp)
		{
			if (strlen(dir)-strlen(DirTmp)>0)
			{
				dir[strlen(dir)-strlen(DirTmp)] = '\0';
			}
			else return NULL;
		}
	}
	


	SciPathName=(char*)malloc((int)(strlen(drive)+strlen(dir)+5)*sizeof(char));

	_makepath(SciPathName,drive,dir,NULL,NULL);

	if ( UnixStyle )
	{	
		int i=0;
		for (i=0;i<(int)strlen(SciPathName);i++)
		{
			if (SciPathName[i]=='\\') SciPathName[i]='/';
		}
	}
	SciPathName[strlen(SciPathName)-1]='\0';
	return SciPathName;
}
/***********************************************************************/
char *GetScilabDllDirectory(BOOL UnixStyle)
{
	char *LibScilabPath=NULL;
	char *PathTmp=NULL;
	PathTmp=GetScilabDirectory(UnixStyle);

	LibScilabPath=(char*)malloc(sizeof(char)*(strlen(PathTmp)+strlen("%s\\bin")+1));
	wsprintf(LibScilabPath,"%s\\bin",PathTmp);
	if (PathTmp) {free(PathTmp);PathTmp=NULL;}
	return LibScilabPath;
}
/***********************************************************************/
void AddToScilabDllPathEnv(void)
{
	char *LibScilabDirectory=NULL;
	char *env=NULL;

	LibScilabDirectory=GetScilabDllDirectory(FALSE);
	env=(char*)malloc(sizeof(char)*(strlen(LibScilabDirectory)+strlen("PATH=%s;%PATH%")+1));
	sprintf(env,"PATH=%s;%PATH%",LibScilabDirectory);
	if (LibScilabDirectory) {free(LibScilabDirectory);LibScilabDirectory=NULL;}
	_putenv(env);
	if (env) {free(env);env=NULL;}
}
/***********************************************************************/