/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "modules.h"
#include "getmodules.h"
#include "machine.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
static struct MODULESLIST *ScilabModules=NULL;
/*-----------------------------------------------------------------------------------*/ 
static BOOL AddModules(void);
/*-----------------------------------------------------------------------------------*/ 
struct MODULESLIST *getmodules(void)
{
	if (ScilabModules==NULL)
	{
		ScilabModules=(struct MODULESLIST *)MALLOC(sizeof(struct MODULESLIST));
		AddModules();
	}
	return ScilabModules;

}
/*-----------------------------------------------------------------------------------*/ 
static BOOL AddModules(void)
{
	int i=0;

	/*premiere version sans test sur les modules*/

	ScilabModules->ModuleList=(char**)MALLOC(sizeof(char*)*AllScilabModulesNumber);
	ScilabModules->numberofModules=AllScilabModulesNumber;

	for (i=0;i<AllScilabModulesNumber;i++)
	{
		ScilabModules->ModuleList[i]=(char*)MALLOC(sizeof(char)*(strlen(AllScilabModules[i])+1));
		sprintf(ScilabModules->ModuleList[i],"%s",AllScilabModules[i]);
	}

	return TRUE;
}
/*-----------------------------------------------------------------------------------*/ 