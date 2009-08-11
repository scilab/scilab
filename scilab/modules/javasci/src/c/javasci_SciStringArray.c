/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ENPC
 * Copyright (C) INRIA
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
#include "javasci_SciStringArray.h"
#include "api_common.h"
#include "api_string.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
JNIEXPORT jstring JNICALL Java_javasci_SciStringArray_GetElement(JNIEnv *env , jobject obj_this,jint indrarg, jint indcarg);
/*--------------------------------------------------------------------------*/
/*! public native String GetElement(int indr, int indc); */
JNIEXPORT jstring JNICALL Java_javasci_SciStringArray_GetElement(JNIEnv *env , jobject obj_this,jint indrarg, jint indcarg)
{
	int *pLength = NULL;
	char **pStrings = NULL;
	int cm = 0, cn = 0;
	int i = 0;
	jstring jstrToReturn;

	jclass class_Mine = (*env)->GetObjectClass(env, obj_this);
	jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name","Ljava/lang/String;");
	jfieldID id_m = (*env)->GetFieldID(env, class_Mine, "m", "I");
	jfieldID id_n = (*env)->GetFieldID(env, class_Mine, "n", "I");

	jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);
	jint jm = (*env)->GetIntField(env, obj_this, id_m);
	jint jn = (*env)->GetIntField(env, obj_this, id_n);
	const char *cname = (*env)->GetStringUTFChars(env, jname, NULL);

	jfieldID id_x;
	jobjectArray jx;

	int dimension[2];

	if (getNamedVarDimension((char*)cname, &dimension[0], &dimension[1]))
	{
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		fprintf(stderr,"Error in Java_javasci_SciStringArray_GetElement (1).\n");
		jstrToReturn = (*env)->NewStringUTF(env, "");
		return jstrToReturn;
	}

	if (dimension[0] != jm)
	{
		fprintf(stderr,"Error in Java_javasci_SciStringArray_GetElement (2).\n");
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		jstrToReturn = (*env)->NewStringUTF(env, "");
		return jstrToReturn;
	}

	if (dimension[1] != jn)
	{
		fprintf(stderr,"Error in Error in Java_javasci_SciStringArray_GetElement (3).\n");
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		jstrToReturn = (*env)->NewStringUTF(env, "");
		return jstrToReturn;
	}

	id_x = (*env)->GetFieldID(env, class_Mine, "x", "[Ljava/lang/String;");
	jx = (*env)->GetObjectField(env, obj_this, id_x);

	pLength = (int*)MALLOC(sizeof(int) * (jm * jn));
	if (pLength == NULL)
	{
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		fprintf(stderr,"Error in Java_javasci_SciStringArray_GetElement (4).\n");
		jstrToReturn = (*env)->NewStringUTF(env, "");
		return jstrToReturn;
	}

	if ( readNamedMatrixOfString((char*)cname, &cm, &cn, pLength, pStrings) )
	{
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		fprintf(stderr,"Error in Java_javasci_SciStringArray_GetElement (5).\n");
		jstrToReturn = (*env)->NewStringUTF(env, "");
		return jstrToReturn;
	}

	pStrings = (char**)MALLOC(sizeof(char*) * (jm * jn));
	if (pStrings == NULL)
	{
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		fprintf(stderr,"Error in Java_javasci_SciStringArray_GetElement (6).\n");
		jstrToReturn = (*env)->NewStringUTF(env, "");
		return jstrToReturn;
	}

	for (i = 0; i < jm * jn; i++)
	{
		pStrings[i] = (char*)MALLOC(sizeof(char)* (pLength[i] + 1));
		if (pStrings[i] == NULL)
		{
			(*env)->ReleaseStringUTFChars(env, jname , cname);
			fprintf(stderr,"Error in Java_javasci_SciStringArray_GetElement (7).\n");
			freeArrayOfString(pStrings, i);
			FREE(pLength);  pLength = NULL;
			jstrToReturn = (*env)->NewStringUTF(env, "");
			return jstrToReturn;
		}
	}

	if ( readNamedMatrixOfString((char*)cname, &cm, &cn, pLength, pStrings) )
	{
		if (pLength) {FREE(pLength); pLength = NULL;}
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		fprintf(stderr,"Error in Java_javasci_SciStringArray_GetElement (8).\n");
		jstrToReturn = (*env)->NewStringUTF(env, "");
		return jstrToReturn;
	}

	if (pLength) {FREE(pLength); pLength = NULL;}

	for (i = 0; i < cm * cn; i++)
	{
		jstring jelement = (*env)->NewStringUTF(env, pStrings[i]);
		(*env)->SetObjectArrayElement(env, jx,  i, jelement);
	}

	jstrToReturn = (*env)->NewStringUTF(env, pStrings[(indcarg - 1) * cm + (indrarg - 1)]);

	freeArrayOfString(pStrings, cm * cn);

	(*env)->ReleaseStringUTFChars(env, jname , cname);

	return jstrToReturn;
}
/*--------------------------------------------------------------------------*/
