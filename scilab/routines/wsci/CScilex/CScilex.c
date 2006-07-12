/***********************************************************************/
/* Copyright (C) 2005 INRIA Allan CORNET */
/***********************************************************************/
#include <Windows.h>
#include "stdio.h"
#include "../../os_specific/win_mem_alloc.h" /* MALLOC */
/***********************************************************************/
typedef int (*MYPROC) (int , char **);
/***********************************************************************/
int main (int argc, char **argv)
{
	#define MAXCMDTOKENS 128

	HINSTANCE hinstLib; 
	MYPROC Console_Main; 

	BOOL fFreeResult, fRunTimeLinkSuccess = FALSE; 
	
	int argcbis=-1;
	LPSTR argvbis[MAXCMDTOKENS];
	int i=0;
	int FindNW=0;

	for (i=0;i<argc;i++)
	{
		if ( (strcmp(argv[i],"-nw")==0) || (strcmp(argv[i],"-NW")==0) ) FindNW=1;
		if ( (strcmp(argv[i],"-nwni")==0) || (strcmp(argv[i],"-NWNI")==0) ) FindNW=1;
		if ( (strcmp(argv[i],"-nogui")==0) || (strcmp(argv[i],"-NOGUI")==0) ) FindNW=1;
	}

	if (FindNW==0)
	{
		char *nwparam=NULL;
		nwparam=(char*)MALLOC((strlen("-nw")+1)*sizeof(char));
		strcpy(nwparam,"-nw");
		for (i=0;i<argc;i++)
		{
			argvbis[i]=argv[i];
		}
		argvbis[argc]=nwparam;
		argcbis=argc+1;
	}
	else
	{
		for (i=0;i<argc;i++)
		{
			argvbis[i]=argv[i];
		}
		argcbis=argc;
	}
    	
	hinstLib = LoadLibrary(TEXT("Libscilab")); 	
    
	if (hinstLib != NULL) 
	{ 
		Console_Main = (MYPROC) GetProcAddress(hinstLib, TEXT("Console_Main")); 

		if (NULL != Console_Main) 
		{
			fRunTimeLinkSuccess = TRUE;
			(Console_Main)(argcbis,argvbis);
		}
		fFreeResult = FreeLibrary(hinstLib); 
	} 

	if (! fRunTimeLinkSuccess) 
	{
		MessageBox(NULL,"scilex.exe : Libscilab.dll not found !","Warning",MB_ICONERROR); 
		exit(1);
	}
	else exit(0);

    return 0;

}
/***********************************************************************/
