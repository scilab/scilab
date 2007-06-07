/*------------------------------------------------------------------------*/
/* file: JniUtils.cpp                                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Set of functions to simplify the use of JNI. These              */
/*        functions are used to call Java from C code                     */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "JniUtils.h"
#include "MALLOC.h"
#include "Scierror.h"

/*------------------------------------------------------------------------------------------*/
/** Static variable containing the jvm. */
static JavaVM * sciJVM  = NULL ;
/** Static variable containing the current java environment. */
static JNIEnv * sciJEnv = NULL ;
/*------------------------------------------------------------------------------------------*/
void jniInitUtils( JavaVM * jvm )
{
  sciJVM = jvm ;
  jniUpdateCurrentEnv() ;
}
/*------------------------------------------------------------------------------------------*/
void jniCloseUtils( void )
{
  sciJEnv = NULL ;
  sciJVM  = NULL ;
}
/*------------------------------------------------------------------------------------------*/
void jniUpdateCurrentEnv( void )
{
  /* tips from sun, use AttachCurrentThread to always get the right environment */ 
  (*sciJVM)->AttachCurrentThread( sciJVM, (void **) &sciJEnv, NULL ) ;

  /* clear all previous exceptions pending on the thread */
  (*sciJEnv)->ExceptionClear( sciJEnv ) ;
}
/*------------------------------------------------------------------------------------------*/
JNIEnv * jniGetCurrentJavaEnv( void )
{
  return sciJEnv ;
}
/*------------------------------------------------------------------------------------------*/
jdoubleArray jniCreateDoubleArrayCopy( const jdouble * cArray, int nbElements )
{
  jdoubleArray res = (*sciJEnv)->NewDoubleArray( sciJEnv, nbElements ) ;
  (*sciJEnv)->SetDoubleArrayRegion( sciJEnv, res, 0, nbElements, cArray ) ;
  if ( !jniCheckLastCall(FALSE) )
  {
    return NULL ;
  }
  return res ;
}
/*------------------------------------------------------------------------------------------*/
jintArray jniCreateIntArrayCopy( const jint * cArray, int nbElements )
{
  jintArray res = (*sciJEnv)->NewIntArray( sciJEnv, nbElements ) ;
  (*sciJEnv)->SetIntArrayRegion( sciJEnv, res, 0, nbElements, cArray ) ;
  if ( !jniCheckLastCall(FALSE) )
  {
    return NULL ;
  }
  return res ;
}
/*------------------------------------------------------------------------------------------*/
void jniCopyJavaDoubleArray( const jdoubleArray javaArray, jdouble * cArray )
{
  int arrayLength = (*sciJEnv)->GetArrayLength( sciJEnv, javaArray ) ;
  (*sciJEnv)->GetDoubleArrayRegion( sciJEnv, javaArray, 0, arrayLength, cArray ) ;
}
/*------------------------------------------------------------------------------------------*/
void jniCopyJavaIntArray( const jintArray javaArray, jint * cArray )
{
  int arrayLength = (*sciJEnv)->GetArrayLength( sciJEnv, javaArray ) ;
  (*sciJEnv)->GetIntArrayRegion( sciJEnv, javaArray, 0, arrayLength, cArray ) ;
}
/*------------------------------------------------------------------------------------------*/
jstring jniCreateStringCopy( const char * cString )
{
  jstring res = (*sciJEnv)->NewStringUTF( sciJEnv, cString ) ;
  if ( !jniCheckLastCall(FALSE) )
  {
    return NULL ;
  }
  return res ;
}
/*------------------------------------------------------------------------------------------*/
void jniDeleteLocalEntity( jobject entity )
{
  if ( entity != NULL )
  {
    (*sciJEnv)->DeleteLocalRef( sciJEnv, entity ) ;
  }
}
/*------------------------------------------------------------------------------------------*/
BOOL jniCreateDefaultInstance( const char * className, jclass * instanceClass, jobject * instance )
{
  jmethodID constructObject = NULL ;
  
  *instanceClass = (*sciJEnv)->FindClass( sciJEnv, className ) ;
  if ( !jniCheckLastCall(TRUE) )
  {
    Scierror( 999, "Unable to find class %s.\r\n", className ) ;
    *instanceClass = NULL ;
    *instance      = NULL ;
    return FALSE ;
  }

  /* "()V" for no parameters and return void */
  /* "<init>" for constructor */
  constructObject = (*sciJEnv)->GetMethodID( sciJEnv, *instanceClass, "<init>", "()V" ) ;

  *instance = (*sciJEnv)->NewObject( sciJEnv, *instanceClass, constructObject ) ;
  if ( !jniCheckLastCall(TRUE) )
  {
    Scierror( 999, "Unable to create an instance of class %s.\r\n", className ) ;
    *instanceClass = NULL ;
    *instance      = NULL ;
    return FALSE ;
  }
  return TRUE ;
}
/*------------------------------------------------------------------------------------------*/
BOOL jniCreateDefaultInstanceSafe( const char * className, jclass * instanceClass, jobject * instance )
{
  if ( instanceClass == NULL || instance == NULL ) { return FALSE ; }
  jniUpdateCurrentEnv() ;
  return jniCreateDefaultInstance( className, instanceClass, instance ) ;
}
/*------------------------------------------------------------------------------------------*/
BOOL jniCallVoidFunctionV( jobject instance, const char * functionName, const char * paramTypes, va_list args )
{
  jmethodID   voidMethod = NULL ;
  jclass      instanceClass = (*sciJEnv)->GetObjectClass( sciJEnv, instance ) ; /* retrieve the class of the object */
  char      * callingSequence = NULL ;

  /* Add (...)V around the paramList */
  callingSequence = MALLOC( ( strlen(paramTypes) + 4 ) * sizeof(char) ) ; /* 3 for ()V and 1 for 0 terminating character */
  if ( callingSequence == NULL ) { return FALSE ; }

  sprintf( callingSequence, "(%s)V", paramTypes ) ;

  /* Find the method in the class */
  voidMethod = (*sciJEnv)->GetMethodID( sciJEnv, instanceClass, functionName, callingSequence ) ;
  if ( voidMethod == NULL || !jniCheckLastCall(TRUE) )
  {
    Scierror( 999, "Unable to find function %s.\r\n", functionName ) ;
    FREE( callingSequence ) ;
    return FALSE ;
  }

  /* Call the function with the optionals parameters */
  (*sciJEnv)->CallVoidMethodV( sciJEnv, instance, voidMethod, args ) ;
  if ( !jniCheckLastCall(TRUE) )
  {
    Scierror( 999, "Error when calling function %s.\r\n", functionName ) ;
    FREE( callingSequence ) ;
    return FALSE ;
  }

  FREE( callingSequence ) ;
  return TRUE ;
}
/*------------------------------------------------------------------------------------------*/
BOOL jniCallVoidFunction( jobject instance, const char * functionName, const char * paramTypes, ... )
{
  BOOL status = FALSE ;
  va_list args ;
  va_start( args, paramTypes ) ;
  status = jniCallVoidFunctionV( instance, functionName, paramTypes, args ) ;
  va_end(args);
  
  return status ;
}
/*------------------------------------------------------------------------------------------*/
BOOL jniCallVoidFunctionSafe( jobject instance, const char * functionName, const char * paramTypes, ... )
{
  va_list args ;
  BOOL status = FALSE ;

  if ( instance == NULL ) { return FALSE ; }

  jniUpdateCurrentEnv() ;

  /* Call the function with the optionals parameters */
  va_start( args, paramTypes ) ;
  status = jniCallVoidFunctionV( instance, functionName, paramTypes, args ) ;
  va_end(args);
  return status ;

}
/*------------------------------------------------------------------------------------------*/
BOOL jniCheckLastCall( BOOL dumpStack )
{
  if ( !sciJEnv ) { return FALSE ; }

  if ( (*sciJEnv)->ExceptionOccurred(sciJEnv) )
  {
    if ( dumpStack )
    {
      (*sciJEnv)->ExceptionDescribe(sciJEnv) ;
    }
    return FALSE ;
  }
  return TRUE ;
}
/*------------------------------------------------------------------------------------------*/
