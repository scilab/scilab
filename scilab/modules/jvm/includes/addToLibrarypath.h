/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __ADDTOLIBRARYPATH_H__
#define __ADDTOLIBRARYPATH_H__

#include "machine.h" /* BOOL */

/**
* add a path to java.library.path
* @param a new path to add
* @return TRUE or FALSE
*/
BOOL addToLibrarypath(char *librarypathstring);

#endif /* __ADDTOLIBRARYPATH_H__ */
/*-----------------------------------------------------------------------------------*/ 
