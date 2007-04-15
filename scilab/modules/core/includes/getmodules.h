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
 * @param void  
 * @return List of modules
 */
struct MODULESLIST *getmodules(void);

/**
 * <long-description>
 *
 * @param void  
 * @return <ReturnValue>
 */
BOOL DisposeModulesInfo(void);
/*-----------------------------------------------------------------------------------*/ 
