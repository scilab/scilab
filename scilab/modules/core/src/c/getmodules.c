/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <stdio.h>
#include "getmodules.h"
#include "machine.h"
#include "MALLOC.h"
#include "setgetSCIpath.h"
/*-----------------------------------------------------------------------------------*/ 
#define basenamemodulesfile "modules/modules" 
/*-----------------------------------------------------------------------------------*/ 
extern BOOL FileExist(char *filename);
extern void sciprint();
/*-----------------------------------------------------------------------------------*/ 
static struct MODULESLIST *ScilabModules=NULL;
/*-----------------------------------------------------------------------------------*/ 
static BOOL AddModules(void);
static BOOL ReadModulesFile(void);
static int GetNumberOfLinesInFile(char *filename);
static BOOL AppendModules(char *filename);
static void cleaningLine(char *source);
static BOOL VerifyModule(char *ModuleName);
/*-----------------------------------------------------------------------------------*/ 
struct MODULESLIST *getmodules(void)
{
	if (ScilabModules==NULL)
	{
		ScilabModules=(struct MODULESLIST *)MALLOC(sizeof(struct MODULESLIST));
		ReadModulesFile();
	}
	return ScilabModules;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL DisposeModulesInfo(void)
{
	BOOL bOK=FALSE;
	if (ScilabModules)
	{
		int i=0;
		for (i=0;i<ScilabModules->numberofModules;i++)
		{
			if (ScilabModules->ModuleList[i])
			{
				FREE(ScilabModules->ModuleList[i]);
				ScilabModules->ModuleList[i]=NULL;
			}
		}
		if (ScilabModules->ModuleList)
		{
			FREE(ScilabModules->ModuleList);
			ScilabModules->ModuleList=NULL;
		}
		ScilabModules->numberofModules=0;
		FREE(ScilabModules);
		ScilabModules=NULL;

	}

	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
static BOOL ReadModulesFile(void)
{
	BOOL bOK=FALSE;
	char *ModulesFilename=NULL;
	char *SciPath=NULL;

	SciPath=getSCIpath();
	if (SciPath==NULL)
	{
		sciprint("The SCI environment variable is not set\n");
		return FALSE;
	}

	ModulesFilename=(char*)MALLOC((strlen(SciPath)+strlen("/")+strlen(basenamemodulesfile)+1)*sizeof(char));
	sprintf(ModulesFilename,"%s/%s",SciPath,basenamemodulesfile);
	FREE(SciPath);
	SciPath=NULL;

	if (FileExist(ModulesFilename))
	{
		int NumberofLines=GetNumberOfLinesInFile(ModulesFilename);
		ScilabModules->ModuleList=(char**)MALLOC(sizeof(char*)*NumberofLines);
		ScilabModules->numberofModules=NumberofLines;
		AppendModules(ModulesFilename);
	}
	else
	{
		sciprint("Error with %s file.\n",ModulesFilename);
		FREE(ModulesFilename);
		ModulesFilename=NULL;
		return FALSE;
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
static int GetNumberOfLinesInFile(char *filename)
{
	#define LineMaxSize 1024
	int ret=0;
	FILE *pFile;

	if (FileExist(filename))
	{
		char Line[LineMaxSize];
		pFile=fopen(filename,"rt");
		while (fgets(Line, LineMaxSize,pFile))
		{
			if ( VerifyModule(Line) )
			{
				ret++;
			}
		}
		fclose(pFile);
	}

	return ret;
}
/*-----------------------------------------------------------------------------------*/ 
static BOOL AppendModules(char *filename)
{
	#define LineMaxSize 1024
	BOOL bOK=FALSE;
	char Line[LineMaxSize];
	int i=0;
	FILE *pFile;

	pFile=fopen(filename,"rt");
	while (fgets(Line, LineMaxSize,pFile))
	{
		if ( VerifyModule(Line) )
		{
			ScilabModules->ModuleList[i]=(char*)MALLOC(sizeof(char)*(strlen(Line)+1));
			sprintf(ScilabModules->ModuleList[i],"%s",Line);
			i++;
		}
		else
		{
			if (Line[0]!=';')
			{
				sciprint("%s module not found.\n",Line);
			}
		}
	}
	fclose(pFile);
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
static void cleaningLine(char *source)
{
	int i=0;
	for (i=0;i<(int)strlen(source);i++)
	{
		if ( (source[i]==' ') || (source[i]=='\n') || (source[i]=='\r') )
		{
			source[i]='\0';
			break;
		}
	}
}
/*-----------------------------------------------------------------------------------*/ 
static BOOL VerifyModule(char *ModuleName)
{
	BOOL bOK=FALSE;
	char *SciPath=NULL;
	char *FullPathModuleName=NULL;

	cleaningLine(ModuleName);

	/* ';' est defini pour mettre un commentaire dans le fichier */
	if (ModuleName[0]==';') return FALSE;

	SciPath=getSCIpath();
	if (SciPath==NULL)
	{
		sciprint("The SCI environment variable is not set\n");
		return FALSE;
	}

	FullPathModuleName=(char*)MALLOC((strlen(SciPath)+strlen("%s/modules/%s/etc/%s.start")+(strlen(ModuleName)*2)+1)*sizeof(char));
	sprintf(FullPathModuleName,"%s/modules/%s/etc/%s.start",SciPath,ModuleName,ModuleName);
	FREE(SciPath);
	SciPath=NULL;

	/* ajouter d'autres tests d'existences */

	if (FileExist(FullPathModuleName))
	{
		bOK=TRUE;
	}
	FREE(FullPathModuleName);
	FullPathModuleName=NULL;

	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
