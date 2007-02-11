#ifndef _Included_javasci_SciBooleanArray
#define _Included_javasci_SciBooleanArray
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2007 */
/********************************************************************************************************/
#include "javasci_globals.h"
#ifdef __cplusplus
extern "C" {
#endif

/*! private static native void Initialize(); */
JNIEXPORT void JNICALL Java_javasci_SciBooleanArray_Initialize (JNIEnv *env, jclass cl);

/*! private native int getRowFromScilab(String name); */
JNIEXPORT jint JNICALL Java_javasci_SciBooleanArray_getRowFromScilab(JNIEnv *, jobject, jstring);

/*! private native int getColFromScilab(String name); */
JNIEXPORT jint JNICALL Java_javasci_SciBooleanArray_getColFromScilab(JNIEnv *, jobject, jstring);

/*! public native boolean Job(String job); */
JNIEXPORT jboolean JNICALL Java_javasci_SciBooleanArray_Job (JNIEnv *, jobject, jstring);

/*! public native void Get(); */
JNIEXPORT void JNICALL Java_javasci_SciBooleanArray_Get(JNIEnv *env , jobject obj_this);

/*! public native void Send(); */
JNIEXPORT void JNICALL Java_javasci_SciBooleanArray_Send(JNIEnv *env , jobject obj_this);

/*! public native double GetElement(int indr,int indc); */
JNIEXPORT jboolean JNICALL Java_javasci_SciBooleanArray_GetElement(JNIEnv *env , jobject obj_this,jint indr, jint indc);

#ifdef __cplusplus
}
#endif
#endif
/********************************************************************************************************/
