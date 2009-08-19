/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Arnaud MANGIN
 * Copyright (C) 2007 - INRIA - Fabien VIALE
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
#include <string.h>
#include "javasci_Scilab2.h"
#include "CallScilab.h"
#include "api_common.h"
#include "api_double.h"
#include "api_string.h"
#include "freeArrayOfString.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
static jobject getDoubleMatrix(JNIEnv *env,  jclass cl, jstring name, jint nbRow, jint nbCol);
static jobject getComplexMatrix(JNIEnv *env,  jclass cl, jstring name, jint nbRow, jint nbCol);
static jobject getStringMatrix(JNIEnv *env,  jclass cl, jstring name, jint nbRow, jint nbCol);
/*--------------------------------------------------------------------------*/
/**
* Add on  : Javasci for Pro-Active 
*/
/*--------------------------------------------------------------------------*/
JNIEXPORT void JNICALL Java_javasci_Scilab_initialize(JNIEnv *env, jclass cl)
{
	if ( GetInterfState() == 0) { EnableInterf(); Initialize();} 
}
/*--------------------------------------------------------------------------*/
JNIEXPORT void JNICALL Java_javasci_Scilab_sendDoubleMatrix (JNIEnv *env, jclass cl, jobject objMatrix)
{
	const char *cname = NULL;
	double *matrix = NULL;
	int nbRow = 0, nbCol = 0;

	jclass clMatrix = (*env)->GetObjectClass(env, objMatrix);

	jfieldID id_matrix =  (*env)->GetFieldID(env, clMatrix, "matrix", "[D" );
	jfieldID id_name =  (*env)->GetFieldID(env, clMatrix, "name", "Ljava/lang/String;");
	jfieldID id_nbRow =  (*env)->GetFieldID(env, clMatrix, "nbRow", "I" );
	jfieldID id_nbCol =  (*env)->GetFieldID(env, clMatrix, "nbCol", "I" );

	jdoubleArray jmatrix = (*env)->GetObjectField(env, objMatrix, id_matrix);
	jstring jname = (jstring) (*env)->GetObjectField(env, objMatrix, id_name);
	jint jnbRow = (*env)->GetIntField(env, objMatrix, id_nbRow);
	jint jnbCol = (*env)->GetIntField(env, objMatrix, id_nbCol);

	nbRow = jnbRow;
	nbCol = jnbCol;

	cname = (*env)->GetStringUTFChars(env, jname, NULL); 
	matrix = (*env)->GetDoubleArrayElements(env, jmatrix, NULL);

	if (createNamedMatrixOfDouble((char*)cname, nbRow, nbCol, matrix))
	{
		fprintf(stderr,"Error in Java_javasci_Scilab_sendDoubleMatrix.\n");
	}

	(*env)->ReleaseStringUTFChars(env, jname , cname);
	(*env)->ReleaseDoubleArrayElements(env, jmatrix, matrix, 0);
}
/*--------------------------------------------------------------------------*/
JNIEXPORT void JNICALL Java_javasci_Scilab_receiveDoubleMatrix (JNIEnv *env, jclass cl, jobject objMatrix)
{
	const char *cname = NULL;
	double *matrix = NULL;
	int nbRow = 0, nbCol = 0;
	int rows = 0, cols  = 0; 

	jclass clMatrix = (*env)->GetObjectClass(env, objMatrix);

	jfieldID id_matrix =  (*env)->GetFieldID(env, clMatrix, "matrix", "[D" );
	jfieldID id_name =  (*env)->GetFieldID(env, clMatrix, "name", "Ljava/lang/String;");
	jfieldID id_nbRow =  (*env)->GetFieldID(env, clMatrix, "nbRow", "I" );
	jfieldID id_nbCol =  (*env)->GetFieldID(env, clMatrix, "nbCol", "I" );

	jdoubleArray jmatrix = (*env)->GetObjectField(env, objMatrix, id_matrix);
	jstring jname = (jstring) (*env)->GetObjectField(env, objMatrix, id_name);
	jint jnbRow = (*env)->GetIntField(env, objMatrix, id_nbRow);
	jint jnbCol = (*env)->GetIntField(env, objMatrix, id_nbCol);

	nbRow = jnbRow;
	nbCol = jnbCol;

	cname = (*env)->GetStringUTFChars(env, jname, NULL); 

	if (readNamedMatrixOfDouble((char*)cname, &rows, &cols, NULL))
	{
		fprintf(stderr,"Error in Java_javasci_Scilab_receiveDoubleMatrix (1).\n");
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		return;
	}

	if (nbRow != rows)
	{
		fprintf(stderr,"Error in Java_javasci_Scilab_receiveDoubleMatrix (2).\n");
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		return;
	}

	if (nbCol != cols)
	{
		fprintf(stderr,"Error in Java_javasci_Scilab_receiveDoubleMatrix (3).\n");
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		return;
	}

	matrix = (*env)->GetDoubleArrayElements(env, jmatrix, NULL);

	readNamedMatrixOfDouble((char*)cname, &rows, &cols, matrix);

	(*env)->ReleaseStringUTFChars(env, jname , cname);
	(*env)->ReleaseDoubleArrayElements(env, jmatrix, matrix, 0);
}
/*--------------------------------------------------------------------------*/
JNIEXPORT void JNICALL Java_javasci_Scilab_sendComplexMatrix (JNIEnv *env, jclass cl, jobject objMatrix)
{
	const char *cname = NULL;
	double *cx = NULL, *cy = NULL;
	int nbRow = 0, nbCol = 0;

	jclass clMatrix = (*env)->GetObjectClass(env, objMatrix);

	jfieldID id_x =  (*env)->GetFieldID(env, clMatrix, "x", "[D" );
	jfieldID id_y =  (*env)->GetFieldID(env, clMatrix, "y", "[D" );
	jfieldID id_name =  (*env)->GetFieldID(env, clMatrix, "name", "Ljava/lang/String;");
	jfieldID id_nbRow =  (*env)->GetFieldID(env, clMatrix, "nbRow", "I" );
	jfieldID id_nbCol =  (*env)->GetFieldID(env, clMatrix, "nbCol", "I" );

	jdoubleArray jx = (*env)->GetObjectField(env, objMatrix, id_x);
	jdoubleArray jy = (*env)->GetObjectField(env, objMatrix, id_y);
	jstring jname = (jstring) (*env)->GetObjectField(env, objMatrix, id_name);
	jint jnbRow = (*env)->GetIntField(env, objMatrix, id_nbRow);
	jint jnbCol = (*env)->GetIntField(env, objMatrix, id_nbCol);

	nbRow = jnbRow;
	nbCol = jnbCol;

	cname = (*env)->GetStringUTFChars(env, jname, NULL); 
	cx = (*env)->GetDoubleArrayElements(env, jx, NULL);
	cy = (*env)->GetDoubleArrayElements(env, jy, NULL);

	if ( createNamedComplexMatrixOfDouble((char*)cname, nbRow, nbCol, cx, cy) )
	{
		fprintf(stderr,"Error in Java_javasci_Scilab_sendComplexMatrix.\n");
	}

	(*env)->ReleaseStringUTFChars(env, jname , cname);
	(*env)->ReleaseDoubleArrayElements(env,jx,cx,0);
	(*env)->ReleaseDoubleArrayElements(env,jy,cy,0);
}
/*--------------------------------------------------------------------------*/
JNIEXPORT void JNICALL Java_javasci_Scilab_receiveComplexMatrix (JNIEnv *env, jclass cl, jobject objMatrix)
{
	const char *cname = NULL;
	double *cx = NULL, *cy = NULL;
	int nbRow = 0, nbCol = 0;
	int rows = 0, cols = 0;

	jclass clMatrix = (*env)->GetObjectClass(env, objMatrix);

	jfieldID id_x =  (*env)->GetFieldID(env, clMatrix, "x", "[D" );
	jfieldID id_y =  (*env)->GetFieldID(env, clMatrix, "y", "[D" );
	jfieldID id_name =  (*env)->GetFieldID(env, clMatrix, "name", "Ljava/lang/String;");
	jfieldID id_nbRow =  (*env)->GetFieldID(env, clMatrix, "nbRow", "I" );
	jfieldID id_nbCol =  (*env)->GetFieldID(env, clMatrix, "nbCol", "I" );

	jdoubleArray jx = (*env)->GetObjectField(env, objMatrix, id_x);
	jdoubleArray jy = (*env)->GetObjectField(env, objMatrix, id_y);
	jstring jname = (jstring) (*env)->GetObjectField(env, objMatrix, id_name);
	jint jnbRow = (*env)->GetIntField(env, objMatrix, id_nbRow);
	jint jnbCol = (*env)->GetIntField(env, objMatrix, id_nbCol);

	nbRow = jnbRow;
	nbCol = jnbCol;

	cname = (*env)->GetStringUTFChars(env, jname, NULL); 

	if ( readNamedComplexMatrixOfDouble((char*)cname, &rows, &cols, NULL, NULL) )
	{
		fprintf(stderr,"Error in Java_javasci_Scilab_receiveComplexMatrix (1).\n");
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		return;
	}

	if (nbRow != rows)
	{
		fprintf(stderr,"Error in Java_javasci_Scilab_receiveComplexMatrix (2).\n");
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		return;
	}

	if (nbCol != cols)
	{
		fprintf(stderr,"Error in Java_javasci_Scilab_receiveComplexMatrix (3).\n");
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		return;
	}

	cx = (*env)->GetDoubleArrayElements(env, jx, NULL);
	cy = (*env)->GetDoubleArrayElements(env, jy, NULL);

	if ( readNamedComplexMatrixOfDouble((char*)cname, &rows, &cols, cx, cy) )
	{
		fprintf(stderr,"Error in Java_javasci_Scilab_receiveComplexMatrix (4).\n");
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		(*env)->ReleaseDoubleArrayElements(env,jx,cx,0);
		(*env)->ReleaseDoubleArrayElements(env,jy,cy,0);
		return;
	}

	(*env)->ReleaseStringUTFChars(env, jname , cname);
	(*env)->ReleaseDoubleArrayElements(env,jx,cx,0);
	(*env)->ReleaseDoubleArrayElements(env,jy,cy,0);
}
/*--------------------------------------------------------------------------*/
JNIEXPORT void JNICALL Java_javasci_Scilab_sendStringMatrix (JNIEnv *env, jclass cl, jobject objMatrix)
{
	const char *cname = NULL;
	int nbRow = 0, nbCol = 0;
	int i = 0;

	char **pStrings = NULL;

	jclass clMatrix = (*env)->GetObjectClass(env, objMatrix);

	jfieldID id_matrix = (*env)->GetFieldID(env, clMatrix, "matrix", "[Ljava/lang/String;" );
	jfieldID id_name =  (*env)->GetFieldID(env, clMatrix, "name", "Ljava/lang/String;");
	jfieldID id_nbRow = (*env)->GetFieldID(env, clMatrix, "nbRow", "I" );
	jfieldID id_nbCol = (*env)->GetFieldID(env, clMatrix, "nbCol", "I" );

	jobjectArray jmatrix = (*env)->GetObjectField(env, objMatrix, id_matrix);
	jstring jname = (jstring) (*env)->GetObjectField(env, objMatrix, id_name);
	jint jnbRow = (*env)->GetIntField(env, objMatrix, id_nbRow);
	jint jnbCol = (*env)->GetIntField(env, objMatrix, id_nbCol);

	nbRow = jnbRow;
	nbCol = jnbCol;

	cname = (*env)->GetStringUTFChars(env, jname, NULL);

	pStrings = (char **)MALLOC(sizeof(char*)*(nbRow * nbCol));
	if (pStrings == NULL)
	{
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		return;
	}

	for (i = 0; i < (nbRow * nbCol); i++)
	{
		jstring jelement = (jstring)(*env)->GetObjectArrayElement(env, jmatrix, i);
		const char *element = (*env)->GetStringUTFChars(env, jelement, NULL);
		pStrings[i] = strdup(element);
		(*env)->ReleaseStringUTFChars(env, jelement,  element);
	}

	createNamedMatrixOfString((char*)cname, nbRow, nbCol, pStrings);

	(*env)->ReleaseStringUTFChars(env, jname , cname);
	freeArrayOfString(pStrings, nbRow * nbCol);
}
/*--------------------------------------------------------------------------*/
JNIEXPORT void JNICALL Java_javasci_Scilab_receiveStringMatrix (JNIEnv *env, jclass cl, jobject objMatrix)
{
	const char *cname = NULL;
	int nbRow = 0, nbCol = 0;
	int i = 0;
	int rows = 0;
	int cols = 0;
	int *pLength = NULL;
	char **pStrings = NULL;

	jclass clMatrix = (*env)->GetObjectClass(env, objMatrix);

	jfieldID id_matrix =  (*env)->GetFieldID(env, clMatrix, "matrix", "[Ljava/lang/String;" );
	jfieldID id_name =  (*env)->GetFieldID(env, clMatrix, "name", "Ljava/lang/String;");
	jfieldID id_nbRow =  (*env)->GetFieldID(env, clMatrix, "nbRow", "I" );
	jfieldID id_nbCol =  (*env)->GetFieldID(env, clMatrix, "nbCol", "I" );

	jobjectArray jmatrix = (*env)->GetObjectField(env, objMatrix, id_matrix);
	jstring jname = (jstring) (*env)->GetObjectField(env, objMatrix, id_name);
	jint jnbRow = (*env)->GetIntField(env, objMatrix, id_nbRow);
	jint jnbCol = (*env)->GetIntField(env, objMatrix, id_nbCol);

	nbRow = jnbRow;
	nbCol = jnbCol;

	cname = (*env)->GetStringUTFChars(env, jname, NULL);
	if ( readNamedMatrixOfString((char*)cname, &rows, &cols, pLength, pStrings) )
	{
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		return;
	}

	if (nbCol != cols)
	{
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		return;
	}

	if (nbRow != rows)
	{
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		return;
	}

	pLength = (int*)MALLOC(sizeof(int)*(rows * cols));
	if (pLength == NULL)
	{
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		return;
	}

	if ( readNamedMatrixOfString((char*)cname, &rows, &cols, pLength, pStrings) )
	{
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		return;
	}

	pStrings = (char**)MALLOC(sizeof(char*)*(rows * cols));
	for (i = 0; i < rows * cols; i++)
	{
		pStrings[i] = (char*)MALLOC(sizeof(char)* (pLength[i] + 1));
		if (pStrings[i] == NULL)
		{
			freeArrayOfString(pStrings, i);
			FREE(pLength);
			(*env)->ReleaseStringUTFChars(env, jname , cname);
			return;
		}
	}

	if ( readNamedMatrixOfString((char*)cname, &rows, &cols, pLength, pStrings) )
	{
		if (pLength) {FREE(pLength); pLength = NULL;}
		(*env)->ReleaseStringUTFChars(env, jname , cname);
		return;
	}

	if (pLength) {FREE(pLength); pLength = NULL;}

	for (i = 0; i < rows * cols; i++)
	{
		jstring jelement = (*env)->NewStringUTF(env, pStrings[i]);
		(*env)->SetObjectArrayElement(env, jmatrix,  i, jelement);
	}

	freeArrayOfString(pStrings, rows * cols);

	(*env)->ReleaseStringUTFChars(env, jname , cname);
}
/*--------------------------------------------------------------------------*/
static jobject getDoubleMatrix(JNIEnv *env,  jclass cl, jstring name, jint nbRow, jint nbCol)
{
	jclass clMatrix = (*env)->FindClass(env, "javasci/SciDoubleMatrix");
	jmethodID consID = (*env)->GetMethodID(env, clMatrix, "<init>", "(Ljava/lang/String;II)V"); 
	jobject objMatrix =  (*env)->NewObject(env, clMatrix, consID, name, nbRow, nbCol);

	Java_javasci_Scilab_receiveDoubleMatrix(env, cl, objMatrix);
	return objMatrix;
}
/*--------------------------------------------------------------------------*/
static jobject getComplexMatrix(JNIEnv *env,  jclass cl, jstring name, jint nbRow, jint nbCol)
{
	jclass clMatrix = (*env)->FindClass(env, "javasci/SciComplexMatrix");
	jmethodID consID = (*env)->GetMethodID(env, clMatrix, "<init>", "(Ljava/lang/String;II)V"); 
	jobject objMatrix =  (*env)->NewObject(env, clMatrix, consID, name, nbRow, nbCol);

	Java_javasci_Scilab_receiveComplexMatrix(env, cl, objMatrix);
	return objMatrix;
}
/*--------------------------------------------------------------------------*/
static jobject getStringMatrix(JNIEnv *env,  jclass cl, jstring name, jint nbRow, jint nbCol)
{
	jclass clMatrix = (*env)->FindClass(env, "javasci/SciStringMatrix");
	jmethodID consID = (*env)->GetMethodID(env, clMatrix, "<init>", "(Ljava/lang/String;II)V"); 
	jobject objMatrix =  (*env)->NewObject(env, clMatrix, consID, name, nbRow, nbCol);

	Java_javasci_Scilab_receiveStringMatrix(env, cl, objMatrix);
	return objMatrix;
}
/*--------------------------------------------------------------------------*/
JNIEXPORT jobject JNICALL Java_javasci_Scilab_receiveDataByName (JNIEnv *env, jclass cl, jstring name)
{
	const char *cname = NULL;
	jobject obj  =NULL;
	int type = 0;
	int Dimensions[2];

	cname = (*env)->GetStringUTFChars(env, name, NULL); 
	(*env)->ReleaseStringUTFChars(env, name , cname);

	type = getNamedVarType((char*)cname); /* give type */
	if (getNamedVarDimension((char*)cname, &Dimensions[0], &Dimensions[1]));

	switch(type)
	{
	case sci_matrix : 

		if (isNamedVarComplex((char*)cname)) 
		{
			return getComplexMatrix(env, cl, name, Dimensions[0], Dimensions[1]);
		}
		else 
		{
			return getDoubleMatrix(env, cl, name, Dimensions[0], Dimensions[1]);
		}
	case sci_poly :
		break;
	case sci_boolean :
		break;
	case sci_ints :
		break; 
	case sci_strings :
		return getStringMatrix(env, cl, name, Dimensions[0], Dimensions[1]);
		break;
	case sci_list :
		break;
	default :
		break;
	}
	return obj;
}
/*--------------------------------------------------------------------------*/

