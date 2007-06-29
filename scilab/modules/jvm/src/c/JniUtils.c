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
jniCallMethodCache * jniCreateCallMethodCache( void )
{
  jniCallMethodCache * newCache = MALLOC(sizeof(jniCallMethodCache)) ;
  if ( newCache == NULL ) { return NULL ; }

  newCache->methodId      = NULL ;
  newCache->instanceClass = NULL ;

  return newCache;
}
/*------------------------------------------------------------------------------------------*/
void jniDestroyCallMethodCache( jniCallMethodCache * cache )
{
  if ( cache != NULL )
  {
     cache->methodId = NULL ;
     if ( cache->instanceClass != NULL )
     {
       (*sciJEnv)->DeleteGlobalRef(sciJEnv, cache->instanceClass) ;
     }
     FREE(cache) ;
  }
}
/*------------------------------------------------------------------------------------------*/
void jniIntializeCallMethodCache( jniCallMethodCache * cache, jclass instanceClass, jmethodID methodId )
{
  cache->methodId = methodId ;

  cache->instanceClass = (*sciJEnv)->NewGlobalRef(sciJEnv, instanceClass) ;
}
/*------------------------------------------------------------------------------------------*/
BOOL jniIsCallMethodCacheInitialized( jniCallMethodCache * cache )
{
  return ( cache != NULL && cache->instanceClass != NULL ) ;
}
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
void jniSetCurrentEnv( JNIEnv * env )
{
  sciJEnv = env ;
}
/*------------------------------------------------------------------------------------------*/
void jniUpdateCurrentEnv( void )
{
  /* tips from sun, use AttachCurrentThread to always get the right environment */ 
  (*sciJVM)->AttachCurrentThread( sciJVM, (void **) &sciJEnv, NULL ) ;
  //(*sciJVM)->GetEnv( sciJVM, (void **) &sciJEnv, JNI_VERSION_1_6 ) ;
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
void jniDeleteGlobalEntity( jobject entity )
{
  if ( entity != NULL )
  {
    (*sciJEnv)->DeleteGlobalRef( sciJEnv, entity ) ;
  }
}
/*------------------------------------------------------------------------------------------*/
BOOL jniCreateDefaultInstance( const char * className, jclass * instanceClass, jobject * instance )
{
  jmethodID constructObject = NULL ;
  jobject localInstance ;
  jclass  localClass ;
  
  localClass = (*sciJEnv)->FindClass( sciJEnv, className ) ;
  if ( !jniCheckLastCall(TRUE) )
  {
    Scierror( 999, "Unable to find class %s.\r\n", className ) ;
    *instanceClass = NULL ;
    *instance      = NULL ;
    return FALSE ;
  }

  *instanceClass = (*sciJEnv)->NewGlobalRef(sciJEnv, localClass) ;

  (*sciJEnv)->DeleteLocalRef(sciJEnv, localClass) ;
  localClass = NULL ;

  /* "()V" for no parameters and return void */
  /* "<init>" for constructor */
  constructObject = (*sciJEnv)->GetMethodID( sciJEnv, *instanceClass, "<init>", "()V" ) ;

  localInstance = (*sciJEnv)->NewObject( sciJEnv, *instanceClass, constructObject ) ;
  if ( !jniCheckLastCall(TRUE) )
  {
    Scierror( 999, "Unable to create an instance of class %s.\r\n", className ) ;
    (*sciJEnv)->DeleteGlobalRef(sciJEnv, *instanceClass) ;
    *instanceClass = NULL ;
    *instance      = NULL ;
    return FALSE ;
  }
  
  *instance = (*sciJEnv)->NewGlobalRef(sciJEnv, localInstance) ;
  (*sciJEnv)->DeleteLocalRef(sciJEnv, localInstance) ;
  localInstance = NULL;

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
BOOL jniCallVoidFunctionV( jobject instance, jclass instanceClass, const char * functionName, const char * paramTypes, va_list args )
{
  jmethodID   voidMethod = NULL ;
  jclass      instanceClasse = (*sciJEnv)->GetObjectClass( sciJEnv, instance ) ; /* retrieve the class of the object */
  char      * callingSequence = NULL ;

  /* Add (...)V around the paramList */
  callingSequence = MALLOC( ( strlen(paramTypes) + 4 ) * sizeof(char) ) ; /* 3 for ()V and 1 for 0 terminating character */
  if ( callingSequence == NULL ) { return FALSE ; }

  sprintf( callingSequence, "(%s)V", paramTypes ) ;

  /* Find the method in the class */
  voidMethod = (*sciJEnv)->GetMethodID( sciJEnv, instanceClass, functionName, callingSequence ) ;
  if ( !jniCheckLastCall(TRUE) || voidMethod == NULL )
  {
    Scierror( 999, "Unable to find method %s.\r\n", functionName ) ;
    FREE( callingSequence ) ;
    return FALSE ;
  }

  /* Call the function with the optionals parameters */
  (*sciJEnv)->CallVoidMethodV( sciJEnv, instance, voidMethod, args ) ;
  if ( !jniCheckLastCall(TRUE) )
  {
    Scierror( 999, "Error when calling method %s.\r\n", functionName ) ;
    FREE( callingSequence ) ;
    return FALSE ;
  }

  //(*sciJEnv)->DeleteLocalRef(sciJEnv, instanceClass) ;
  FREE( callingSequence ) ;
  return TRUE ;
}
/*------------------------------------------------------------------------------------------*/
BOOL jniCallVoidFunction( jobject instance, jclass instanceClass, const char * functionName, const char * paramTypes, ... )
{
  BOOL status = FALSE ;
  va_list args ;
  va_start( args, paramTypes ) ;
  status = jniCallVoidFunctionV( instance, instanceClass, functionName, paramTypes, args ) ;
  va_end(args);
  
  return status ;
}
/*------------------------------------------------------------------------------------------*/
BOOL jniCallVoidFunctionSafe( jobject instance, jclass instanceClass, const char * functionName, const char * paramTypes, ... )
{
  va_list args ;
  BOOL status = FALSE ;

  if ( instance == NULL ) { return FALSE ; }

  jniUpdateCurrentEnv() ;

  /* Call the function with the optionals parameters */
  va_start( args, paramTypes ) ;
  status = jniCallVoidFunctionV( instance, instanceClass, functionName, paramTypes, args ) ;
  va_end(args);
  return status ;

}
/*------------------------------------------------------------------------------------------*/
jvalue jniCallMemberFunction( jobject instance, jniCallMethodCache * cache, const char * functionName, const char * descriptor, ... )
{
  va_list args ;
  jvalue res;

  /* Call the function with the optionals parameters */
  va_start( args, descriptor ) ;
  res = jniCallMemberFunctionV( instance, cache, functionName, descriptor, args ) ;
  va_end(args);
  return res ;
}
/*------------------------------------------------------------------------------------------*/
jvalue jniCallMemberFunctionSafe( jobject instance, jniCallMethodCache * cache, const char * functionName, const char * descriptor, ... )
{
  va_list args ;
  jvalue res;

  jniInitJValue(&res) ;

  if ( instance == NULL ) { return res ; }

  jniUpdateCurrentEnv() ;

  /* Call the function with the optionals parameters */
  va_start( args, descriptor ) ;
  res = jniCallMemberFunctionV( instance, cache, functionName, descriptor, args ) ;
  va_end(args);
  return res ;
}
/*------------------------------------------------------------------------------------------*/
jvalue jniCallMemberFunctionV( jobject instance, jniCallMethodCache * cache, const char * functionName, const char * descriptor, va_list args )
{
  jclass instanceClass = NULL;
  jmethodID methodId = NULL;
  jvalue res;
  int returnTypeIndex = 0 ;
  
  jniInitJValue(&res) ;
  
  if ((*sciJEnv)->EnsureLocalCapacity(sciJEnv, 2) == JNI_OK)
  {
    if ( !jniIsCallMethodCacheInitialized(cache) )
    {
      // Need to intialize cache
      instanceClass = (*sciJEnv)->GetObjectClass(sciJEnv, instance) ;
      methodId = (*sciJEnv)->GetMethodID(sciJEnv, instanceClass, functionName, descriptor ) ;
      if ( cache != NULL )
      {
        jniIntializeCallMethodCache(cache, instanceClass, methodId) ;
      }
    }
    else
    {
      // cache already initialized
      instanceClass = cache->instanceClass ;
      methodId = cache->methodId ;
    }

    if ( methodId == NULL )
    {
      Scierror( 999, "Error when calling function %s.\r\n", functionName ) ;
      return res;
    }

    /* parse the descriptor to find return type */
    while( descriptor[returnTypeIndex] != ')' )
    {
      returnTypeIndex++ ;
    }
    returnTypeIndex++;

    switch( descriptor[returnTypeIndex] )
    {
    case 'V':
      (*sciJEnv)->CallVoidMethodV(sciJEnv, instance, methodId, args) ;
      break;
    case 'Z':
      res.z = (*sciJEnv)->CallBooleanMethodV(sciJEnv, instance, methodId, args) ;
      break;
    case 'B':
      res.b = (*sciJEnv)->CallByteMethodV(sciJEnv, instance, methodId, args) ;
      break;
    case 'C':
      res.c = (*sciJEnv)->CallCharMethodV(sciJEnv, instance, methodId, args) ;
      break;
    case 'S':
      res.s = (*sciJEnv)->CallShortMethodV(sciJEnv, instance, methodId, args) ;
      break;
    case 'I':
      res.i = (*sciJEnv)->CallIntMethodV(sciJEnv, instance, methodId, args) ;
      break;
    case 'J':
      res.j = (*sciJEnv)->CallLongMethodV(sciJEnv, instance, methodId, args) ;
      break;
    case 'F':
      res.f = (*sciJEnv)->CallFloatMethodV(sciJEnv, instance, methodId, args) ;
      break;
    case 'D':
      res.d = (*sciJEnv)->CallDoubleMethodV(sciJEnv, instance, methodId, args) ;
      break;
    case '[':
    case'L':
      res.l = (*sciJEnv)->CallObjectMethodV(sciJEnv, instance, methodId, args) ;
      break;
    default:
      Scierror( 999, "Error when calling function %s.\r\n", functionName ) ;
      break;
    }

    if ( !jniCheckLastCall(TRUE) )
    {
      Scierror( 999, "Error when calling function %s.\r\n", functionName ) ;
      return res ;
    }

  }
  return res;

}
/*------------------------------------------------------------------------------------------*/
void jniInitJValue( jvalue * value )
{
  value->b = 0 ;
  value->c = 0 ;
  value->d = 0.0 ;
  value->f = 0.0 ;
  value->i = 0 ;
  value->j = 0 ;
  value->l = NULL ;
  value->s = 0 ;
  value->z = 0 ;
}
/*------------------------------------------------------------------------------------------*/
double jniGetDoubleValue( jvalue value )
{
  return value.d ;
}
/*------------------------------------------------------------------------------------------*/
int jniGetIntValue( jvalue value )
{
  return value.i ;
}
/*------------------------------------------------------------------------------------------*/
BOOL jniCheckLastCall( BOOL dumpStack )
{
  jniUpdateCurrentEnv() ;
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
