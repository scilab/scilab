/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include "javasci_Scilab.h"
#include "CallScilab.h"
#include "api_common.h"
#include "../../../modules/graphics/includes/WindowList.h"
/*--------------------------------------------------------------------------*/
extern int GetLastErrorCode(void);
/*--------------------------------------------------------------------------*/
JNIEXPORT void JNICALL Java_javasci_Scilab_Initialize (JNIEnv *env, jclass cl)
{
	if ( GetInterfState() == 0) 
	{ 
		EnableInterf();
		Initialize();
	} 
}
/*--------------------------------------------------------------------------*/
/* public static native void Events(); */
JNIEXPORT void JNICALL Java_javasci_Scilab_Events(JNIEnv *env , jobject obj_this)
/*--------------------------------------------------------------------------*/
{
	ScilabDoOneEvent();
}
/*--------------------------------------------------------------------------*/
/* public static native boolean HaveAGraph(); */
JNIEXPORT jboolean JNICALL Java_javasci_Scilab_HaveAGraph (JNIEnv *env , jobject obj_this)
/*--------------------------------------------------------------------------*/
{
	if (sciHasFigures()) return JNI_TRUE;
	return JNI_FALSE;
}
/*--------------------------------------------------------------------------*/
/* public static native boolean Exec(String job); */
JNIEXPORT jboolean JNICALL Java_javasci_Scilab_Exec(JNIEnv *env , jclass cl, jstring job)
{
	jboolean bOK = JNI_TRUE;
	const char *cjob = NULL;

	cjob = (*env)->GetStringUTFChars(env, job, NULL);

	if (strlen(cjob) >= MAX_STR)
	{
		fprintf(stderr,"Error in Java_javasci_Scilab_Exec routine (line too long).\n");
		bOK=JNI_FALSE;
	}
	else
	{
		if ( SendScilabJob((char *)cjob) != 0) 
		{
			fprintf(stderr,"Error in Java_javasci_Scilab_Exec routine.\n");
			bOK = JNI_FALSE;
		}
		else bOK = JNI_TRUE;
		fflush(stdout);
	}

	(*env)->ReleaseStringUTFChars(env, job , cjob);

	return bOK;
}
/*--------------------------------------------------------------------------*/
/* public static native boolean Finish(); */
JNIEXPORT jboolean JNICALL Java_javasci_Scilab_Finish (JNIEnv *env , jobject obj_this)
{
	if (GetInterfState() == 0)
	{
		return JNI_FALSE;
	}
	else
	{
		ExitScilab();
		return JNI_TRUE;
	}
}
/*--------------------------------------------------------------------------*/
/* public static native boolean ExistVar(String varName); */
JNIEXPORT jboolean JNICALL Java_javasci_Scilab_ExistVar(JNIEnv *env , jclass cl, jstring varName)
{
	jboolean bOK = JNI_FALSE;
	const char *cvarName = NULL;
	int sciType = 0;

	cvarName = (*env)->GetStringUTFChars(env, varName, NULL);

	if (strlen(cvarName) >= nlgh)
	{
		fprintf(stderr,"Error in Java_javasci_Scilab_ExistVar routine (line too long > %d).\n", nlgh);
		(*env)->ReleaseStringUTFChars(env, varName , cvarName);
		return JNI_FALSE;
	}

	sciType = getNamedVarType((char*)cvarName);

	switch(sciType)
	{
	case sci_matrix:
	case sci_poly:
	case sci_boolean:
	case sci_sparse:
	case sci_boolean_sparse:
	case sci_matlab_sparse:
	case sci_ints:
	case sci_handles:
	case sci_strings:
	case sci_u_function:
	case sci_c_function:
	case sci_lib:
	case sci_list:
	case sci_tlist:
	case sci_mlist:
	case sci_lufact_pointer:
	case sci_implicit_poly:
	case sci_intrinsic_function:
		bOK = JNI_TRUE;
		break;
	default:
		bOK = JNI_FALSE;
		break;
	}

	(*env)->ReleaseStringUTFChars(env, varName , cvarName);
	return bOK;
}
/*--------------------------------------------------------------------------*/
/* public static native int TypeVar(String varName); */
JNIEXPORT jint JNICALL Java_javasci_Scilab_TypeVar(JNIEnv *env , jclass cl, jstring varName)
{
	jint type = -1;
	const char *cvarName = (*env)->GetStringUTFChars(env, varName, NULL);

	if (strlen(cvarName) >= nlgh)
	{
		fprintf(stderr,"Error in Java_javasci_Scilab_TypeVar routine (line too long > %d).\n", nlgh);
		(*env)->ReleaseStringUTFChars(env, varName , cvarName);
		return type;
	}

	type = (jint)getNamedVarType((char *)cvarName);
	(*env)->ReleaseStringUTFChars(env, varName , cvarName);

	return type;
}
/*--------------------------------------------------------------------------*/
/* public static native int GetLastErrorCode(); */
JNIEXPORT jint JNICALL Java_javasci_Scilab_GetLastErrorCode (JNIEnv *env , jobject obj_this)
{
	return GetLastErrorCode();
}
/*--------------------------------------------------------------------------*/
