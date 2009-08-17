/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _Included_javasci_SciComplexArray
#define _Included_javasci_SciComplexArray

#include "javasci_globals.h"
#ifdef __cplusplus
extern "C" {
#endif

/*! private static native void Initialize(); */
JNIEXPORT void JNICALL Java_javasci_SciComplexArray_Initialize (JNIEnv *env, jclass cl);

/*! private native int getRowFromScilab(String name); */
JNIEXPORT jint JNICALL Java_javasci_SciComplexArray_getRowFromScilab(JNIEnv *, jobject, jstring);

/*! private native int getColFromScilab(String name); */
JNIEXPORT jint JNICALL Java_javasci_SciComplexArray_getColFromScilab(JNIEnv *, jobject, jstring);

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
/*--------------------------------------------------------------------------*/
