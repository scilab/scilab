/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __LOADCLASSPATH_H__
#define __LOADCLASSPATH_H__

#include "machine.h" /* BOOL */

/**
* add paths to CLASSPATH
* @param a filename SCI/etc/classpath.xml
* @return TRUE or FALSE
*/
BOOL LoadClasspath(char *xmlfilename);

#endif /* __LOADCLASSPATH_H__ */
/*-----------------------------------------------------------------------------------*/ 
