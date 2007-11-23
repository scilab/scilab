/*--------------------------------------------------------------------------*/
/* Allan CORNET */
/* INRIA 2007 */
/*--------------------------------------------------------------------------*/
#ifndef __GETLONGPATHNAME_H__
#define __GETLONGPATHNAME_H__

#include "machine.h" /* BOOL */

/**
* Retrieves the long path form of the specified path
* @param[in] short path name
* @param[out] TRUE if conversion is ok
* @return long path name
* on Linux returns same path name
* used for Windows
*/
char *getlongpathname(char *shortpathname,BOOL *convertok);


#endif /* __GETLONGPATHNAME_H__ */
/*--------------------------------------------------------------------------*/
