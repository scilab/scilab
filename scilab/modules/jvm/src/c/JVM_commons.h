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
#define DEFAULT_SCILAB_CLASSPATH "/bin" /* SCI/bin */
#define DEFAULT_SCILAB_LIBRARYPATH "/bin" /* SCI/bin */
#else
/* not really used , java.library.path is used in fact */
/* @TODO is it used or not ? Please explain or remove it */
#define DEFAULT_SCILAB_CLASSPATH "/.libs/" /* SCI/.libs */
#define DEFAULT_SCILAB_LIBRARYPATH "/.libs/" /* SCI/.libs */
#endif
/*-----------------------------------------------------------------------------------*/ 
#define USER_CLASSPATH "."
#define JVM_CLASSPATH "/modules/jvm/jar/org.scilab.modules.jvm.jar"
/*-----------------------------------------------------------------------------------*/ 
#endif /*  __JVM_COMMONS_H__ */
/*-----------------------------------------------------------------------------------*/ 
