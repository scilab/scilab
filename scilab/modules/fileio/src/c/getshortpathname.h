/*-----------------------------------------------------------------------------------*/
/* Allan CORNET */
/* INRIA 2007 */
/*-----------------------------------------------------------------------------------*/
#ifndef __GETSHORTPATHNAME_H__
#define __GETSHORTPATHNAME_H__

#include "machine.h" /* BOOL */

/**
* Retrieves the short path form of the specified path
* @param[in] long path name
* @param[out] TRUE if conversion is ok
* @return short path name
* on Linux returns same path name
* used for Windows
*/
char *getshortpathname(char *longpathname, BOOL *convertok);


#endif /* __GETSHORTPATHNAME_H__ */
/*-----------------------------------------------------------------------------------*/
