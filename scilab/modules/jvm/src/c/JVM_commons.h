/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2007 */
/* Allan CORNET*/
/*-----------------------------------------------------------------------------------*/ 
#ifndef __JVM_COMMONS_H__
#define __JVM_COMMONS_H__
/*-----------------------------------------------------------------------------------*/ 
#include <jni.h>
#include "machine.h"
#include "JVM_functions.h"
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
#define JRE_PATH "/java/jre"
#else
/* Always this value when we embedded JRE */
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
/* not really used , java.library.path is used in fact */
#define DEFAULT_SCILAB_CLASSPATH "/.libs/" /* SCI/.libs */
#define DEFAULT_SCILAB_LIBRARYPATH "/.libs/" /* SCI/.libs */
#endif
/*-----------------------------------------------------------------------------------*/ 
#define USER_CLASSPATH "."
#define JVM_CLASSPATH "/modules/jvm/jar/org.scilab.modules.jvm.jar"
/*-----------------------------------------------------------------------------------*/ 
#endif /*  __JVM_COMMONS_H__ */
/*-----------------------------------------------------------------------------------*/ 
