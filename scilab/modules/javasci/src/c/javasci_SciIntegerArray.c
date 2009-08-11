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
/*--------------------------------------------------------------------------*/
#include "javasci_SciIntegerArray.h"
#include "api_common.h"
#include "api_int.h"
/*--------------------------------------------------------------------------*/
JNIEXPORT jint JNICALL Java_javasci_SciIntegerArray_GetElement(JNIEnv *env , jobject obj_this,jint indrarg, jint indcarg);
/*--------------------------------------------------------------------------*/
/*! public native int GetElement(int indr, int indc); */
JNIEXPORT jint JNICALL Java_javasci_SciIntegerArray_GetElement(JNIEnv *env , jobject obj_this,jint indrarg, jint indcarg)
{
	int Value = 0;
	int j = 0;

	jclass class_Mine = (*env)->GetObjectClass(env, obj_this);
	jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name","Ljava/lang/String;");
	jfieldID id_m = (*env)->GetFieldID(env, class_Mine, "m", "I");
	jfieldID id_n = (*env)->GetFieldID(env, class_Mine, "n", "I");

	jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);
	jint jm = (*env)->GetIntField(env, obj_this, id_m);
	jint jn = (*env)->GetIntField(env, obj_this, id_n);
	const char *cname = (*env)->GetStringUTFChars(env, jname, NULL);

	jfieldID id_x;
	jintArray jx;
	jint *cx = NULL;
	int *icx = NULL;

	int dimension[2];

	int cm = 0, cn = 0;

	if (getNamedVarDimension((char*)cname, &dimension[0], &dimension[1]))
	{
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		fprintf(stderr,"Error in Java_javasci_SciIntegerArray_GetElement (1).\n");
		return Value;
	}

	if (dimension[0] != jm)
	{
		fprintf(stderr,"Error in Java_javasci_SciIntegerArray_GetElement (2).\n");
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		return Value;
	}

	if (dimension[1] != jn)
	{
		fprintf(stderr,"Error in Java_javasci_SciIntegerArray_GetElement (3).\n");
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		return Value;
	}

	id_x = (*env)->GetFieldID(env, class_Mine, "x", "[D");
	jx = (*env)->GetObjectField(env, obj_this, id_x);
	cx = (*env)->GetIntArrayElements(env, jx, NULL);

	icx = (int*)MALLOC(sizeof(int) * (cm * cn));
	if (icx == NULL)
	{
		fprintf(stderr,"Error in Java_javasci_SciIntegerArray_GetElement (4).\n");
		(*env)->ReleaseIntArrayElements(env, jx, cx, 0);
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		return Value;
	}

	for (j = 0; j < cm * cn; j++) icx[j] = (int)cx[j];

	if (readNamedMatrixOfInteger32((char*)cname, &cm, &cn, icx))
	{
		FREE(icx); icx = NULL;
		fprintf(stderr,"Error in Java_javasci_SciIntegerArray_GetElement (5).\n");
		(*env)->ReleaseIntArrayElements(env, jx, cx, 0);
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		return Value;
	}

	FREE(icx); icx = NULL;

	if ( (indrarg <= 0) || (indcarg <= 0) )
	{
		(*env)->ReleaseIntArrayElements(env, jx, cx, 0);
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		fprintf(stderr,"Error with int indr & int indc must be >0.\n");
		return Value;
	}

	if ( (indrarg > jm) || (indcarg > jn) )
	{
		(*env)->ReleaseIntArrayElements(env, jx, cx, 0);
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		fprintf(stderr,"Error with int indr & int indc.\n");
		return Value;
	}

	Value = cx[(indcarg - 1) * cm + (indrarg - 1)];

	(*env)->ReleaseIntArrayElements(env, jx, cx, 0);
	(*env)->ReleaseStringUTFChars(env, jname , cname);

	return Value;
}
/*--------------------------------------------------------------------------*/
