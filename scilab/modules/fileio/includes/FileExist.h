/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __FILEEXIST_H__
#define __FILEEXIST_H__

#include "machine.h" /* BOOL */

/**
* verify if filename is a valid file
* @param filename a string 
* @return TRUE or FALSE
*/
BOOL FileExist(char *filename);
/*-----------------------------------------------------------------------------------*/ 
#endif /* __FILEEXIST_H__ */
