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
#include "sciprint.h"

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
void jniInitializeCallMethodCache( jniCallMethodCache * cache, jclass instanceClass, jmethodID methodId )
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
JNIEnv * jniUpdateCurrentEnv( void )
{
  /* tips from sun, use AttachCurrentThread to always get the right environment */ 
  (*sciJVM)->AttachCurrentThread( sciJVM, (void **) &sciJEnv, NULL ) ;
  //(*sciJVM)->GetEnv( sciJVM, (void **) &sciJEnv, JNI_VERSION_1_6 ) ;
  /* clear all previous exceptions pending on the thread */
  (*sciJEnv)->ExceptionClear( sciJEnv ) ;
  return sciJEnv;
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
  JNIEnv * curEnv = jniUpdateCurrentEnv();
  
  localClass = (*curEnv)->FindClass( curEnv, className ) ;
  if ( !jniCheckLastCall(TRUE) )
  {
    Scierror( 999, "Unable to find class %s.\r\n", className ) ;
    *instanceClass = NULL ;
    *instance      = NULL ;
    return FALSE ;
  }

  *instanceClass = (*curEnv)->NewGlobalRef(curEnv, localClass) ;

  /* "()V" for no parameters and return void */
  /* "<init>" for constructor */
  constructObject = (*curEnv)->GetMethodID( curEnv, *instanceClass, "<init>", "()V" ) ;

  localInstance = (*curEnv)->NewObject( curEnv, *instanceClass, constructObject ) ;
  if ( !jniCheckLastCall(TRUE) )
  {
    Scierror( 999, "Unable to create an instance of class %s.\r\n", className ) ;
    (*curEnv)->DeleteGlobalRef(curEnv, *instanceClass) ;
    *instanceClass = NULL ;
    *instance      = NULL ;
    return FALSE ;
  }
  
  *instance = (*curEnv)->NewGlobalRef(curEnv, localInstance) ;

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

  JNIEnv * curEnv  = NULL;//= jniUpdateCurrentEnv();
  jsize nbJvm = 0;

  (*sciJVM)->AttachCurrentThread(sciJVM, (void **)&curEnv, NULL);
  
  jniInitJValue(&res) ;
  
  if ((*curEnv)->EnsureLocalCapacity(sciJEnv, 2) == JNI_OK)
  {
    if ( !jniIsCallMethodCacheInitialized(cache) )
    {
      // Need to intialize cache
      instanceClass = (*curEnv)->GetObjectClass(curEnv, instance) ;
      methodId = (*curEnv)->GetMethodID(curEnv, instanceClass, functionName, descriptor ) ;
      if ( cache != NULL )
      {
        jniInitializeCallMethodCache(cache, instanceClass, methodId) ;
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
      jniCheckLastCall(TRUE);
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
      (*curEnv)->CallVoidMethodV(curEnv, instance, methodId, args) ;
      break;
    case 'Z':
      res.z = (*curEnv)->CallBooleanMethodV(curEnv, instance, methodId, args) ;
      break;
    case 'B':
      res.b = (*curEnv)->CallByteMethodV(curEnv, instance, methodId, args) ;
      break;
    case 'C':
      res.c = (*curEnv)->CallCharMethodV(curEnv, instance, methodId, args) ;
      break;
    case 'S':
      res.s = (*curEnv)->CallShortMethodV(curEnv, instance, methodId, args) ;
      break;
    case 'I':
      res.i = (*curEnv)->CallIntMethodV(curEnv, instance, methodId, args) ;
      break;
    case 'J':
      res.j = (*curEnv)->CallLongMethodV(curEnv, instance, methodId, args) ;
      break;
    case 'F':
      res.f = (*curEnv)->CallFloatMethodV(curEnv, instance, methodId, args) ;
      break;
    case 'D':
      res.d = (*curEnv)->CallDoubleMethodV(curEnv, instance, methodId, args) ;
      break;
    case '[':
    case'L':
      res.l = (*curEnv)->CallObjectMethodV(curEnv, instance, methodId, args) ;
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
  JNIEnv * curEnv = jniUpdateCurrentEnv() ;
  if ( !curEnv ) { return FALSE ; }

  if ( (*curEnv)->ExceptionOccurred(curEnv) )
  {
    if ( dumpStack )
    {
      (*curEnv)->ExceptionDescribe(curEnv) ;
    }
    return FALSE ;
  }
  return TRUE ;
}
/*------------------------------------------------------------------------------------------*/
