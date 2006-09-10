#ifndef _Included_javasci_SciStringArray
#define _Included_javasci_SciStringArray
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2006 */
/********************************************************************************************************/
#include "javasci_globals.h"
#ifdef __cplusplus
extern "C" {
#endif

/* private static native void Initialize(); */
JNIEXPORT void JNICALL Java_javasci_SciStringArray_Initialize (JNIEnv *env, jclass cl);

/* public native void Job(String job); */
JNIEXPORT void JNICALL Java_javasci_SciStringArray_Job (JNIEnv *, jobject, jstring);

/* public native String GetString(int indx, int indy); */
JNIEXPORT jstring JNICALL Java_javasci_SciStringArray_GetString(JNIEnv *env , jobject obj_this,jint indxarg, jint indyarg);

/* private native void SendString(String str,int indx, int indy); */
JNIEXPORT void JNICALL Java_javasci_SciStringArray_SendString(JNIEnv *env , jobject obj_this,jstring strarg,jint indxarg, jint indyarg);

#ifdef __cplusplus
}
#endif
#endif
/********************************************************************************************************/
