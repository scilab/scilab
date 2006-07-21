#ifndef _Included_javasci_SciComplexArray
#define _Included_javasci_SciComplexArray
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2006 */
/********************************************************************************************************/
#include "javasci_globals.h"
#ifdef __cplusplus
extern "C" {
#endif

/*! private static native void Initialize(); */
JNIEXPORT void JNICALL Java_javasci_SciComplexArray_Initialize (JNIEnv *env, jclass cl);

/*! public native boolean Job(String job); */
JNIEXPORT jboolean JNICALL Java_javasci_SciComplexArray_Job (JNIEnv *, jobject, jstring);

/*! public native void Get(); */
JNIEXPORT void JNICALL Java_javasci_SciComplexArray_Get(JNIEnv *env , jobject obj_this);

/*! public native void Send(); */
JNIEXPORT void JNICALL Java_javasci_SciComplexArray_Send(JNIEnv *env , jobject obj_this);

/*! public native double GetRealPartElement(int indr,int indc); */
JNIEXPORT jdouble JNICALL Java_javasci_SciComplexArray_GetRealPartElement(JNIEnv *env , jobject obj_this,jint indr, jint indc);

/*! public native double GetImaginaryPartElement(int indr,int indc); */
JNIEXPORT jdouble JNICALL Java_javasci_SciComplexArray_GetImaginaryPartElement(JNIEnv *env , jobject obj_this,jint indr, jint indc);

#ifdef __cplusplus
}
#endif
#endif
/********************************************************************************************************/
