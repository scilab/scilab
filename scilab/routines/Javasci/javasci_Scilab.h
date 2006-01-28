#ifndef _Included_javasci_Scilab
#define _Included_javasci_Scilab
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2005 */
/********************************************************************************************************/
#include "javasci_globals.h"
#ifdef __cplusplus
extern "C" {
#endif

/* public static native void Events(); */
JNIEXPORT void JNICALL Java_javasci_Scilab_Events(JNIEnv *env , jobject obj_this);
  
/* public static native boolean HaveAGraph(); */
JNIEXPORT jboolean JNICALL Java_javasci_Scilab_HaveAGraph (JNIEnv *env , jobject obj_this);

  
/* public static native boolean Exec(String job); */
JNIEXPORT jboolean JNICALL Java_javasci_Scilab_Exec(JNIEnv *env , jclass cl, jstring job);

#ifdef __cplusplus
}
#endif
#endif
/********************************************************************************************************/
