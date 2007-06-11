/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __ADDTOCLASSPATH_H__
#define __ADDTOCLASSPATH_H__

#include "machine.h" /* BOOL */

/**
* add a path to CLASSPATH
* @param a new path to add
* @return TRUE or FALSE
*/
BOOL addToClasspath(char *classpathstring);

#endif /* __ADDTOCLASSPATH_H__ */
/*-----------------------------------------------------------------------------------*/ 
