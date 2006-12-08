/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "MALLOC.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "machine.h"
#include "hashtable_core.h"
#include "getmodules.h"
#include "setgetSCIpath.h"
/*-----------------------------------------------------------------------------------*/  
static int firstentry = 0;
/*-----------------------------------------------------------------------------------*/  
extern int C2F(cvname) __PARAMS((integer *,char *,integer *, unsigned long int));
extern BOOL FileExist(char *filename);
/*-----------------------------------------------------------------------------------*/  
static int Add_a_Scilab_primitive_in_hashtable(char *str, int *dataI, int *data);
static BOOL Load_primitives_from_file(char *filename);
/*-----------------------------------------------------------------------------------*/  
void LoadFunctionsTab(void)
{
	static int firstentry = 0;
	struct MODULESLIST *Modules=NULL;
	char *SciPath=NULL;
	
	int j=0;

	if ( firstentry != 0 ) return;

	SciPath=getSCIpath();
	Modules=getmodules();
	/* je ne libere pas Modules pour accelerer le prochain appel à getmodule */
	/* liberé dans sciquit.c */

	for (j=0;j<Modules->numberofModules;j++)
	{
		#define FORMATFILENAME "%s/modules/%s/sci_gateway/%s"
		char *filename_primitives_list=NULL;
		int len=strlen(FORMATFILENAME)+strlen(SciPath)+strlen(Modules->ModuleList[j])*2;

		filename_primitives_list=(char*)MALLOC((len+1)*sizeof(char));
		sprintf(filename_primitives_list,FORMATFILENAME,SciPath,Modules->ModuleList[j],Modules->ModuleList[j]);

		Load_primitives_from_file(filename_primitives_list);

		if (filename_primitives_list) { FREE(filename_primitives_list);filename_primitives_list=NULL;}
	}

	firstentry = 1;

}
/*-----------------------------------------------------------------------------------*/
static int Add_a_Scilab_primitive_in_hashtable(char *str, int *dataI, int *data)
{
	int ldata;
	int id[NAMECODE];
	int zero=0;
	C2F(cvname)(id,str,&zero,strlen(str));
	ldata= (*dataI)*100+*data;
	return( action_hashtable_scilab_functions(id,str,&ldata,SCI_HFUNCTIONS_ENTER));
}
/*-----------------------------------------------------------------------------------*/
static int Load_primitives_from_file(char *filename)
{
	BOOL bOK=FALSE;
	if (FileExist(filename))
	{
		#define LineMaxSize 1024
		char Line[LineMaxSize];
		FILE *pFile;
		int dataI=0;
		int data=0;
		char namefunction[MAXLENGHTFUNCTIONNAME];

		pFile=fopen(filename,"rt");
		while (fgets(Line, LineMaxSize,pFile))
		{
			if (Line[0]!=';')
			{
				int retval=0;
				retval=sscanf(Line,"%d %d %s",&dataI,&data,namefunction);
				if (retval == 3)
				{
					Add_a_Scilab_primitive_in_hashtable(namefunction,&dataI,&data);
				}
			}
		}
		fclose(pFile);
		bOK=TRUE;
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
