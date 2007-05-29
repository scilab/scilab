/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2007 */
/* Allan CORNET*/
/*-----------------------------------------------------------------------------------*/ 
#ifndef __JVM_COMMONS_H__
#define __JVM_COMMONS_H__
/*-----------------------------------------------------------------------------------*/ 
#include "machine.h"
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
#define JRE_PATH "/java/jre"
#else
/* 
TODO : 
Change this part
The Path of the JRE should be hardcoded for Linux/Unix. 
*/
#define JRE_PATH "/java/jre"
#endif
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
#define PATH_SEPARATOR ";"
#else /* UNIX */
#define PATH_SEPARATOR ":"
#endif
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
#define DEFAULT_SCILAB_CLASSPATH "/bin" /* SCI/bin */
#define DEFAULT_SCILAB_LIBRARYPATH "/bin" /* SCI/bin */
#else
/* 
TODO :
Change this part 
Won't work in production... 
1. this should not be hardcoded (this part should be delegated to the system)
2. the path is not the same in the dev env & the installed env 
*/
#define DEFAULT_SCILAB_CLASSPATH "/.libs/" /* SCI/.libs */
#define DEFAULT_SCILAB_LIBRARYPATH "/.libs/" /* SCI/.libs */
#endif
/*-----------------------------------------------------------------------------------*/ 
#define USER_CLASSPATH "."
#define JVM_CLASSPATH "/modules/jvm/jar/org.scilab.modules.jvm.jar"
/*-----------------------------------------------------------------------------------*/ 
#endif /*  __JVM_COMMONS_H__ */
/*-----------------------------------------------------------------------------------*/ 
