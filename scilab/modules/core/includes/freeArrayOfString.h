/*-----------------------------------------------------------------------------------*/ 
/* Allan CORNET */
/* INRIA 2007 */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __freeArrayOfString_H__
#define __freeArrayOfString_H__

#include "BOOL.h"

/**
* Free Array of String (char **)
* @param[in] ptr on char **
* @param[in] dimension of ptr on char **
* @return TRUE if free
*/
BOOL freeArrayOfString(char **Str,int dim);

#endif
/*-----------------------------------------------------------------------------------*/ 
