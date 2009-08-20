/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _Included_javasci_SciIntegerArray
#define _Included_javasci_SciIntegerArray

#include "javasci_globals.h"
#ifdef __cplusplus
extern "C" {
#endif

/*! private static native void Initialize(); */
JNIEXPORT void JNICALL Java_javasci_SciIntegerArray_Initialize (JNIEnv *env, jclass cl);

/*! private native int getRowFromScilab(String name); */
JNIEXPORT jint JNICALL Java_javasci_SciIntegerArray_getRowFromScilab(JNIEnv *, jobject, jstring);

/*! private native int getColFromScilab(String name); */
JNIEXPORT jint JNICALL Java_javasci_SciIntegerArray_getColFromScilab(JNIEnv *, jobject, jstring);

/*! public native boolean Job(String job); */
JNIEXPORT jboolean JNICALL Java_javasci_SciIntegerArray_Job (JNIEnv *, jobject, jstring);

/*! public native void Get(); */
JNIEXPORT void JNICALL Java_javasci_SciIntegerArray_Get(JNIEnv *env , jobject obj_this);

/*! public native void Send(); */
JNIEXPORT void JNICALL Java_javasci_SciIntegerArray_Send(JNIEnv *env , jobject obj_this);

/*! public native int GetElement(int indr,int indc); */
JNIEXPORT jint JNICALL Java_javasci_SciIntegerArray_GetElement(JNIEnv *env , jobject obj_this,jint indr, jint indc);

#ifdef __cplusplus
}
#endif
#endif
/*--------------------------------------------------------------------------*/
