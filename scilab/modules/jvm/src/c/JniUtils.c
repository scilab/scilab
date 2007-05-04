/*------------------------------------------------------------------------*/
/* file: JniUtils.cpp                                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Set of functions to simplify the use of JNI. These              */
/*        functions are used to call Java from C code                     */
/*------------------------------------------------------------------------*/

#include <stdarg.h>
#include <string.h>

#include "JniUtils.h"
#include "MALLOC.h"

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
void jniDeleteLocalEntity( jobject entity )
{
  (*sciJEnv)->DeleteLocalRef( sciJEnv, entity ) ;
}
/*------------------------------------------------------------------------------------------*/
void jniCreateDefaultInstance( const char * className, jclass * instanceClass, jobject * instance )
{
  jmethodID constructObject = NULL ;
  
  *instanceClass = (*sciJEnv)->FindClass( sciJEnv, className ) ;
  if ( !jniCheckLastCall(TRUE) )
  {
    *instanceClass = NULL ;
    *instance      = NULL ;
    return ;
  }

  /* "()V" for no parameters and return void */
  /* "<init>" for constructor */
  constructObject = (*sciJEnv)->GetMethodID( sciJEnv, *instanceClass, "<init>", "()V" ) ;

  *instance = (*sciJEnv)->NewObject( sciJEnv, *instanceClass, constructObject ) ;
  if ( !jniCheckLastCall(TRUE) )
  {
    *instanceClass = NULL ;
    *instance      = NULL ;
    return ;
  }
}
/*------------------------------------------------------------------------------------------*/
void jniCreateDefaultInstanceSafe( const char * className, jclass * instanceClass, jobject * instance )
{
  jniUpdateCurrentEnv() ;
  jniCreateDefaultInstance( className, instanceClass, instance ) ;
}
/*------------------------------------------------------------------------------------------*/
void jniCallVoidFunction( jobject instance, const char * functionName, const char * paramTypes, ... )
{
  jmethodID   voidMethod = NULL ;
  jclass      instanceClass = (*sciJEnv)->GetObjectClass( sciJEnv, instance ) ; /* retrieve the class of the object */
  char      * callingSequence = NULL ;
  va_list     args ;

  /* Add (...)V around the paramList */
  callingSequence = MALLOC( ( strlen(paramTypes) + 4 ) * sizeof(char) ) ; /* 3 for ()V and 1 for 0 terminating character */
  if ( callingSequence == NULL ) { return ; }

  sprintf( callingSequence, "(%s)V", paramTypes ) ;

  /* Find the method in the class */
  voidMethod = (*sciJEnv)->GetMethodID( sciJEnv, instanceClass, functionName, callingSequence ) ;

  /* Call the function with the optionals parameters */
  va_start( args, paramTypes ) ;
  (*sciJEnv)->CallVoidMethod( sciJEnv, instance, voidMethod, args ) ;
  va_end(args);

  FREE( callingSequence ) ;

}
/*------------------------------------------------------------------------------------------*/
void jniCallVoidFunctionSafe( jobject instance, const char * functionName, const char * paramTypes, ... )
{
  va_list args;

  jniUpdateCurrentEnv() ;

  /* Call the function with the optionals parameters */
  va_start( args, paramTypes ) ;
  jniCallVoidFunction( instance, functionName, paramTypes, args ) ;
  va_end(args);

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
