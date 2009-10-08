/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "javasci_SciComplexArray.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
JNIEXPORT jdouble JNICALL Java_javasci_SciComplexArray_GetRealPartElement(JNIEnv *env , jobject obj_this,jint indrarg, jint indcarg);
JNIEXPORT jdouble JNICALL Java_javasci_SciComplexArray_GetImaginaryPartElement(JNIEnv *env , jobject obj_this,jint indrarg, jint indcarg);
/*--------------------------------------------------------------------------*/
JNIEXPORT jdouble JNICALL Java_javasci_SciComplexArray_GetRealPartElement(JNIEnv *env , jobject obj_this,jint indrarg, jint indcarg)
{
	StrErr strErr;
	double Value = 0.0;

	jclass class_Mine = (*env)->GetObjectClass(env, obj_this);
	jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name","Ljava/lang/String;");
	jfieldID id_m = (*env)->GetFieldID(env, class_Mine, "m", "I");
	jfieldID id_n = (*env)->GetFieldID(env, class_Mine, "n", "I");

	jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);
	jint jm = (*env)->GetIntField(env, obj_this, id_m);
	jint jn = (*env)->GetIntField(env, obj_this, id_n);
	const char *cname = (*env)->GetStringUTFChars(env, jname, NULL);

	jfieldID id_x;
	jdoubleArray jx;
	double *cx = NULL;

	jfieldID id_y;
	jdoubleArray jy;
	double *cy = NULL;

	int dimension[2];

	int cm = 0, cn = 0;

	strErr = getNamedVarDimension(pvApiCtx, (char*)cname, &dimension[0], &dimension[1]);
	if(strErr.iErr)
	{
		fprintf(stderr,"%s", getErrorMessage(strErr));
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		fprintf(stderr,"Error in Java_javasci_SciComplexArray_GetRealPartElement (1).\n");
		return Value;
	}

	if (dimension[0] != jm)
	{
		fprintf(stderr,"Error in Java_javasci_SciComplexArray_GetRealPartElement (2).\n");
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		return Value;
	}

	if (dimension[1] != jn)
	{
		fprintf(stderr,"Error in Java_javasci_SciComplexArray_GetRealPartElement (3).\n");
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		return Value;
	}

	id_x = (*env)->GetFieldID(env, class_Mine, "x", "[D");
	jx = (*env)->GetObjectField(env, obj_this, id_x);
	cx = (*env)->GetDoubleArrayElements(env, jx, NULL);

	id_y = (*env)->GetFieldID(env, class_Mine, "y", "[D");
	jy = (*env)->GetObjectField(env, obj_this, id_y);
	cy = (*env)->GetDoubleArrayElements(env, jy, NULL);

	strErr = readNamedComplexMatrixOfDouble(pvApiCtx, (char*)cname, &cm, &cn, cx, cy);
	if(strErr.iErr)
	{
		fprintf(stderr,"%s", getErrorMessage(strErr));
		fprintf(stderr,"Error in Java_javasci_SciComplexArray_GetRealPartElement (4).\n");
		(*env)->ReleaseDoubleArrayElements(env, jx, cx, 0);
		(*env)->ReleaseDoubleArrayElements(env, jy, cy, 0);
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		return Value;
	}

	if ( (indrarg <= 0) || (indcarg <= 0) )
	{
		(*env)->ReleaseDoubleArrayElements(env, jx, cx, 0);
		(*env)->ReleaseDoubleArrayElements(env, jy, cy, 0);
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		fprintf(stderr,"Error with int indr & int indc must be >0.\n");
		return Value;
	}

	if ( (indrarg > jm) || (indcarg > jn) )
	{
		(*env)->ReleaseDoubleArrayElements(env, jx, cx, 0);
		(*env)->ReleaseDoubleArrayElements(env, jy, cy, 0);
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		fprintf(stderr,"Error with int indr & int indc.\n");
		return Value;
	}

	Value = cx[(indcarg - 1) * cm + (indrarg - 1)];

	(*env)->ReleaseDoubleArrayElements(env, jx, cx, 0);
	(*env)->ReleaseDoubleArrayElements(env, jy, cy, 0);
	(*env)->ReleaseStringUTFChars(env, jname , cname);

	return Value;
}
/*--------------------------------------------------------------------------*/
/*! public native double GetImaginaryPartElement(int indr,int indc); */
JNIEXPORT jdouble JNICALL Java_javasci_SciComplexArray_GetImaginaryPartElement(JNIEnv *env , jobject obj_this,jint indrarg, jint indcarg)
{
	StrErr strErr;
	double Value = 0.0;

	jclass class_Mine = (*env)->GetObjectClass(env, obj_this);
	jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name","Ljava/lang/String;");
	jfieldID id_m = (*env)->GetFieldID(env, class_Mine, "m", "I");
	jfieldID id_n = (*env)->GetFieldID(env, class_Mine, "n", "I");

	jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);
	jint jm = (*env)->GetIntField(env, obj_this, id_m);
	jint jn = (*env)->GetIntField(env, obj_this, id_n);
	const char *cname = (*env)->GetStringUTFChars(env, jname, NULL);

	jfieldID id_x;
	jdoubleArray jx;
	double *cx = NULL;

	jfieldID id_y;
	jdoubleArray jy;
	double *cy = NULL;

	int dimension[2];

	int cm = 0, cn = 0;

	strErr = getNamedVarDimension(pvApiCtx, (char*)cname, &dimension[0], &dimension[1]);
	if(strErr.iErr)
	{
		fprintf(stderr,"%s", getErrorMessage(strErr));
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		fprintf(stderr,"Error in Java_javasci_SciComplexArray_GetRealPartElement (1).\n");
		return Value;
	}

	if (dimension[0] != jm)
	{
		fprintf(stderr,"Error in Java_javasci_SciComplexArray_GetRealPartElement (2).\n");
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		return Value;
	}

	if (dimension[1] != jn)
	{
		fprintf(stderr,"Error in Java_javasci_SciComplexArray_GetRealPartElement (3).\n");
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		return Value;
	}

	id_x = (*env)->GetFieldID(env, class_Mine, "x", "[D");
	jx = (*env)->GetObjectField(env, obj_this, id_x);
	cx = (*env)->GetDoubleArrayElements(env, jx, NULL);

	id_y = (*env)->GetFieldID(env, class_Mine, "y", "[D");
	jy = (*env)->GetObjectField(env, obj_this, id_y);
	cy = (*env)->GetDoubleArrayElements(env, jy, NULL);

	strErr = readNamedComplexMatrixOfDouble(pvApiCtx, (char*)cname, &cm, &cn, cx, cy);
	if(strErr.iErr)
	{
		fprintf(stderr,"%s", getErrorMessage(strErr));
		fprintf(stderr,"Error in Java_javasci_SciComplexArray_GetRealPartElement (4).\n");
		(*env)->ReleaseDoubleArrayElements(env, jx, cx, 0);
		(*env)->ReleaseDoubleArrayElements(env, jy, cy, 0);
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		return Value;
	}

	if ( (indrarg <= 0) || (indcarg <= 0) )
	{
		(*env)->ReleaseDoubleArrayElements(env, jx, cx, 0);
		(*env)->ReleaseDoubleArrayElements(env, jy, cy, 0);
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		fprintf(stderr,"Error with int indr & int indc must be >0.\n");
		return Value;
	}

	if ( (indrarg > jm) || (indcarg > jn) )
	{
		(*env)->ReleaseDoubleArrayElements(env, jx, cx, 0);
		(*env)->ReleaseDoubleArrayElements(env, jy, cy, 0);
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		fprintf(stderr,"Error with int indr & int indc.\n");
		return Value;
	}

	Value = cy[(indcarg - 1) * cm + (indrarg - 1)];

	(*env)->ReleaseDoubleArrayElements(env, jx, cx, 0);
	(*env)->ReleaseDoubleArrayElements(env, jy, cy, 0);
	(*env)->ReleaseStringUTFChars(env, jname , cname);

	return Value;
}
/*--------------------------------------------------------------------------*/

