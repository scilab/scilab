/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __ISDIR_H__
#define __ISDIR_H__

#include "machine.h" /* BOOL */

/**
* Check if a directory exists or not
* @param path the directory
* @return if the directory exists (TRUE) or not (FALSE)
*/
BOOL isdir(const char *path);

#endif /* __ISDIR_H__ */
/*-----------------------------------------------------------------------------------*/ 
