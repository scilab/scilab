#ifndef _Included_javasci_SciReal
#define _Included_javasci_SciReal
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2005 */
/********************************************************************************************************/
#include "javasci_globals.h"
#ifdef __cplusplus
extern "C" {
#endif

/* public native void Job(String job); */
JNIEXPORT void JNICALL Java_javasci_SciReal_Job (JNIEnv *, jobject, jstring);

/* public native void Get(); */
JNIEXPORT void JNICALL Java_javasci_SciReal_Get(JNIEnv *env , jobject obj_this);

/* public native void Send(); */
JNIEXPORT void JNICALL Java_javasci_SciReal_Send(JNIEnv *env , jobject obj_this);

#ifdef __cplusplus
}
#endif
#endif
/********************************************************************************************************/
