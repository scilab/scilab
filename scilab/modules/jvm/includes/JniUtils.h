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
#include <stdarg.h>
#include "machine.h"

/**
 * Cache some data that does not change between functions calls
 */
typedef struct 
{
  jclass    instanceClass;
  jmethodID methodId;
} jniCallMethodCache;

/**
 * Create a new cache
 */
jniCallMethodCache * jniCreateCallMethodCache( void ) ;

/**
 * Destroy a cache
 */
void jniDestroyCallMethodCache( jniCallMethodCache * cache ) ;

/**
 * Initialize a cache with its values.
 */
void jniInitializeCallMethodCache( jniCallMethodCache * cache, jclass instanceClass, jmethodID methodId ) ;

/**
 * To know if a cache has alredy been initialized
 */
BOOL jniIsCallMethodCacheInitialized( jniCallMethodCache * cache ) ;

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
 * Manually specify the current java environment.
 * Maught be used when calling C function from Java
 */
void jniSetCurrentEnv( JNIEnv * env ) ;

/**
 * Retrieve the current Java environment from the JVM.
 * This function should be called each time the environment
 * May have changed.
 */
JNIEnv * jniUpdateCurrentEnv( void ) ;

/**
 * Get the current Java environment
 */
JNIEnv * jniGetCurrentJavaEnv( void ) ;

/**
 * Allocate and set a Java array as a copy of a C array.
 */
jdoubleArray jniCreateDoubleArrayCopy( const jdouble * cArray, int nbElements ) ;
jintArray    jniCreateIntArrayCopy(    const jint    * cArray, int nbElements ) ;

/**
 * Copy a java array into a preallocated C array
 */
void jniCopyJavaDoubleArray( const jdoubleArray javaArray, jdouble * cArray ) ;
void jniCopyJavaIntArray(    const jintArray    javaArray, jint    * cArray ) ;

/**
 * Create a copy of a C string to a Java string
 */
jstring jniCreateStringCopy( const char * cString ) ;

/**
 * Delete a Java object (array, objects, ...)
 */
void jniDeleteLocalEntity( jobject entity ) ;

/**
* Delete a Java object (array, objects, ...)
*/
void jniDeleteGlobalEntity( jobject entity ) ;

/**
 * Create a new instance of a Java class calling its
 * default constructor
 * @param className[in] string of the class name
 * @param instanceClass[out] Java reference to the class
 * @param instance[out] created object
 * @return TRUE if the call was succesful, FALSE otherwise.
 */
BOOL jniCreateDefaultInstance(     const char * className, jclass * instanceClass, jobject * instance ) ;
BOOL jniCreateDefaultInstanceSafe( const char * className, jclass * instanceClass, jobject * instance ) ;

/**
 * Call a java member function from C code.
 * @param instance object containing the function to call
 * @param cache cache relative to this function used to speed up call to the function.
 *              if cache is NULL, then it is not used. If cache is not initialized then the function will initialize it.
 * @param functionName Name of the function
 * @param descriptor string containing the kinds of parameters the function takes and the return type.
 *                   This respect the JNI style and looks like "([DID)V".
 * The parameters are then given has optionals arguments.
 */
jvalue jniCallMemberFunction(     jobject instance, jniCallMethodCache * cache, const char * functionName, const char * descriptor, ... ) ;
jvalue jniCallMemberFunctionSafe( jobject instance, jniCallMethodCache * cache, const char * functionName, const char * descriptor, ... ) ;
jvalue jniCallMemberFunctionV(    jobject instance, jniCallMethodCache * cache, const char * functionName, const char * descriptor, va_list args ) ;


/**
 * Set a jvalue to default values
 */
void jniInitJValue( jvalue * value ) ;

/**
 * Get the double value of a jvalue;
 */
double jniGetDoubleValue( jvalue value ) ;
int    jniGetIntValue(    jvalue value ) ;

/**
 * Tell if the last call to JNI primitive was successful
 * @return true if the call was successful, false otherwise
 * @param dumpStack if true the calling stack is dump in the shell
 * like when exceptions are not caught in Java
 */
BOOL jniCheckLastCall( BOOL dumpStack ) ;

#endif /* _JNI_UTILS_H_ */

