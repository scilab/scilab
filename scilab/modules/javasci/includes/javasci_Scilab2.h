/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Arnaud MANGIN
 * Copyright (C) 2007 - INRIA - Fabien VIALE
 * ...
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _Included_javasci_Scilab2
#define _Included_javasci_Scilab2
/*--------------------------------------------------------------------------*/
/* Add on  : Javasci for Pro-Active */
/*--------------------------------------------------------------------------*/
#include "javasci_globals.h"
#ifdef __cplusplus
extern "C" 
{
#endif

	/**
	* private static native void initialize(); 
	*/
	JNIEXPORT void JNICALL Java_javasci_Scilab_initialize(JNIEnv *env, jclass cl);

	/**
	* public static native SciData receiveDataByName(String id); */
	JNIEXPORT jobject JNICALL Java_javasci_Scilab_receiveDataByName (JNIEnv *env, jclass cl, jstring name);

	/**
	public static native void receiveStringMatrix(SciStringMatrix matrix); 
	*/
	JNIEXPORT void JNICALL Java_javasci_Scilab_receiveStringMatrix (JNIEnv *env, jclass cl, jobject objMatrix);

	/**
	* public static native void sendStringMatrix(SciStringMatrix matrix); 
	*/
	JNIEXPORT void JNICALL Java_javasci_Scilab_sendStringMatrix (JNIEnv *env, jclass cl, jobject objMatrix);

	/**
	* public static native void receiveDoubleMatrix(SciDoubleMatrix matrix); 
	*/
	JNIEXPORT void JNICALL Java_javasci_Scilab_receiveDoubleMatrix (JNIEnv *env, jclass cl, jobject objMatrix);

	/**
	* public static native void sendDoubleMatrix(SciDoubleMatrix matrix); 
	*/
	JNIEXPORT void JNICALL Java_javasci_Scilab_sendDoubleMatrix (JNIEnv *env, jclass cl, jobject objMatrix);

	/**
	* public static native void sendComplexMatrix (SciComplexMatrix matrix); 
	*/
	JNIEXPORT void JNICALL Java_javasci_Scilab_sendComplexMatrix (JNIEnv *env, jclass cl, jobject objMatrix);

	/**
	* public static native void receiveComplexMatrix (SciComplexMatrix matrix); 
	*/
	JNIEXPORT void JNICALL Java_javasci_Scilab_receiveComplexMatrix (JNIEnv *env, jclass cl, jobject objMatrix);

#ifdef __cplusplus
}
#endif
#endif /* _Included_javasci_Scilab2 */
/*--------------------------------------------------------------------------*/
