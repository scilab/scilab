/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "machine.h"
/*-----------------------------------------------------------------------------------*/ 
struct MODULESLIST
{
	char **ModuleList;
	int numberofModules;
};

/*-----------------------------------------------------------------------------------*/ 
/**
 * Get the list of modules
 *
 * @return List of modules
 */
struct MODULESLIST *getmodules(void);

/**
 * <long-description>
 *
 * @return the result of the operation
 */
BOOL DisposeModulesInfo(void);
/*-----------------------------------------------------------------------------------*/ 
