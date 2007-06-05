/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2007 */
/* Allan CORNET*/
/*-----------------------------------------------------------------------------------*/ 
#ifndef __JVM_COMMONS_H__
#define __JVM_COMMONS_H__
/*-----------------------------------------------------------------------------------*/ 
#include <jni.h>
#include "machine.h"
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
/* not really used , LD_LIBRARY_PATH is used in fact */
#define DEFAULT_SCILAB_CLASSPATH "/.libs/" /* SCI/.libs */
#define DEFAULT_SCILAB_LIBRARYPATH "/.libs/" /* SCI/.libs */
#endif
/*-----------------------------------------------------------------------------------*/ 
#define USER_CLASSPATH "."
#define JVM_CLASSPATH "/modules/jvm/jar/org.scilab.modules.jvm.jar"
/*-----------------------------------------------------------------------------------*/ 

/**
 * @TODO : add comments...
 *
 * @param args  
 * @return <ReturnValue>
 */
jint SciJNI_GetDefaultJavaVMInitArgs(void *args);

/**
 * @TODO : add comments...
 *
 * @param pvm   
 * @param penv  
 * @param args  
 * @return <ReturnValue>
 */
jint SciJNI_CreateJavaVM(JavaVM **pvm, void **penv, void *args);

/**
 * @TODO : add comments...
 *
 * @param vmBuf 
 * @param BufLen    
 * @param nVMs  
 * @return <ReturnValue>
 */
jint SciJNI_GetCreatedJavaVMs(JavaVM **vmBuf, jsize BufLen, jsize *nVMs);

/**
 * @TODO : add comments...
 *
 * @param filedynlib    
 * @return <ReturnValue>
 */
BOOL LoadFuntionsJVM(char *filedynlib);


/**
 * @TODO : add comments...
 *
 * @return <ReturnValue>
 */
BOOL FreeDynLibJVM(void);

#endif /*  __JVM_COMMONS_H__ */
/*-----------------------------------------------------------------------------------*/ 
