/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _Included_javasci_SciBooleanArray
#define _Included_javasci_SciBooleanArray

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
/*--------------------------------------------------------------------------*/

