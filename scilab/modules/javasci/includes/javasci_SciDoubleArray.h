#ifndef _Included_javasci_SciDoubleArray
#define _Included_javasci_SciDoubleArray
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2005 */
/********************************************************************************************************/
#include "javasci_globals.h"
#ifdef __cplusplus
extern "C" {
#endif

/*! private static native void Initialize(); */
JNIEXPORT void JNICALL Java_javasci_SciDoubleArray_Initialize (JNIEnv *env, jclass cl);

/*! private native int getRowFromScilab(String name); */
JNIEXPORT jint JNICALL Java_javasci_SciDoubleArray_getRowFromScilab(JNIEnv *, jobject, jstring);

/*! private native int getColFromScilab(String name); */
JNIEXPORT jint JNICALL Java_javasci_SciDoubleArray_getColFromScilab(JNIEnv *, jobject, jstring);

/*! public native boolean Job(String job); */
JNIEXPORT jboolean JNICALL Java_javasci_SciDoubleArray_Job (JNIEnv *, jobject, jstring);

/*! public native void Get(); */
JNIEXPORT void JNICALL Java_javasci_SciDoubleArray_Get(JNIEnv *env , jobject obj_this);

/*! public native void Send(); */
JNIEXPORT void JNICALL Java_javasci_SciDoubleArray_Send(JNIEnv *env , jobject obj_this);

/*! public native double GetElement(int indr,int indc); */
JNIEXPORT jdouble JNICALL Java_javasci_SciDoubleArray_GetElement(JNIEnv *env , jobject obj_this,jint indr, jint indc);

#ifdef __cplusplus
}
#endif
#endif
/********************************************************************************************************/
