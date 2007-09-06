#ifndef _Included_javasci_Scilab2
#define _Included_javasci_Scilab2
/********************************************************************************************************/
/* Arnaud Mangin - INRIA 2006 */
/* Fabien Viale - INRIA 2007 */
/********************************************************************************************************/
/* Add on  : Javasci for Pro-Active */
/********************************************************************************************************/
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
/********************************************************************************************************/
