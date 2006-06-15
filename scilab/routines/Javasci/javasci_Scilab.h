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

/**
* private static native void Initialize();
*/
JNIEXPORT void JNICALL Java_javasci_Scilab_Initialize (JNIEnv *env, jclass cl);

/**
* public static native void Events(); 
*/
JNIEXPORT void JNICALL Java_javasci_Scilab_Events(JNIEnv *env , jobject obj_this);
  
/** 
* public static native boolean HaveAGraph(); 
*/
JNIEXPORT jboolean JNICALL Java_javasci_Scilab_HaveAGraph (JNIEnv *env , jobject obj_this);

/** 
* public static native boolean Exec(String job); 
*/
JNIEXPORT jboolean JNICALL Java_javasci_Scilab_Exec(JNIEnv *env , jclass cl, jstring job);

/** 
public static native boolean ExistVar(String VarName); 
*/
JNIEXPORT jboolean JNICALL Java_javasci_Scilab_ExistVar(JNIEnv *env , jclass cl, jstring job);

/**
* public static native int TypeVar(String VarName); 
*/
JNIEXPORT jint JNICALL Java_javasci_Scilab_TypeVar(JNIEnv *env , jclass cl, jstring job);

/** 
* public static native int GetLastErrorCode(); 
*/
JNIEXPORT jint JNICALL Java_javasci_Scilab_GetLastErrorCode (JNIEnv *env , jobject obj_this);

/** 
* public static native boolean Finish(); 
*/
JNIEXPORT jboolean JNICALL Java_javasci_Scilab_Finish (JNIEnv *env , jobject obj_this);

#ifdef __cplusplus
}
#endif
#endif
/********************************************************************************************************/
