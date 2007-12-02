/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "with_module.h"
#include "getmodules.h"
/*--------------------------------------------------------------------------*/
BOOL with_module(char *modulename)
{
	if (modulename)
	{
		struct MODULESLIST *Modules=getmodules();
		int nrow = Modules->numberofModules;
		int i = 0;

		for(i=0;i<nrow;i++)
		{
			if ( strcmp(Modules->ModuleList[i],modulename) == 0)
			{
				return TRUE;
			}
		}
	}
	return FALSE;

}
/*--------------------------------------------------------------------------*/
