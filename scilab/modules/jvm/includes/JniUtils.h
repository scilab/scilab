/*------------------------------------------------------------------------*/
/* file: JniUtils.h                                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Set of functions to simplify the use of JNI. These              */
/*        functions are used to call Java from C code                     */
/*------------------------------------------------------------------------*/

#ifndef _JNI_UTILS_H_
#define _JNI_UTILS_H_

#include <jni.h>
#include "machine.h"

/**
 * Initialize the environment from an already created jvm.
 */
void jniInitUtils( JavaVM * jvm ) ;

/**
 * To be called when we don't need the jvm any more.
 */
void jniCloseUtils( void ) ;

/**
 * Return the virtual machine used by this object
 */
JavaVM * jniGetJavaVM( void ) ;

/**
 * Retrieve the current Java environment from the JVM.
 * This function should be called each time the environment
 * May have changed.
 */
void jniUpdateCurrentEnv( void ) ;

/**
 * Get the current Java environment
 */
JNIEnv * jniGetCurrentJaveEnv( void ) ;

/**
 * Allocate and set a Java array as a copy of a C array.
 */
jdoubleArray jniCreateDoubleArrayCopy( const jdouble * cArray, int nbElements ) ;
jintArray    jniCreateIntArrayCopy( const jint    * cArray, int nbElements ) ;

/**
 * Delete a Java object (array, objects, ...)
 */
void jniDeleteJavaLocalEntity( jobject entity ) ;

/**
 * Create a new instance of a Java class calling its
 * default constructor
 * @param className[in] string of the class name
 * @param instanceClass[out] Java reference to the class
 * @param instance[out] created object
 */
void jniCreateDefaultInstance(     const char * className, jclass * instanceClass, jobject * instance ) ;
void jniCreateDefaultInstanceSafe( const char * className, jclass * instanceClass, jobject * instance ) ;

/**
 * Call a Java function returning void.
 * @param instance Object caontaining the function to call
 * @param functionName Name of the function
 * @param paramTypes string containing the types of paramaters given as optionals argument.
 *                   This respect the JNI style and looks like "[DID".
 */
void jniCallVoidFunction(     jobject instance, const char * functionName, const char * paramTypes, ... ) ;
void jniCallVoidFunctionSafe( jobject instance, const char * functionName, const char * paramTypes, ... ) ;

/**
 * Tell if the last call to JNI primitive was successful
 * @return true if the call was successful, false otherwise
 * @param dumpStack if true the calling stack is dump in the shell
 * like when exceptions are not caught in Java
 */
BOOL jniCheckLastCall( BOOL dumpStack ) ;

#endif /* _JNI_UTILS_H_ */