/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
#include "javasci_SciBooleanArray.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
JNIEXPORT jboolean JNICALL Java_javasci_SciBooleanArray_GetElement(JNIEnv *env , jobject obj_this,jint indrarg, jint indcarg);
/*--------------------------------------------------------------------------*/
/*! public native boolean GetElement(int indr, int indc); */
JNIEXPORT jboolean JNICALL Java_javasci_SciBooleanArray_GetElement(JNIEnv *env , jobject obj_this,jint indrarg, jint indcarg)
{
	StrErr strErr;
  jboolean Value = JNI_FALSE;
  int *CX = NULL;

  jclass class_Mine = (*env)->GetObjectClass(env, obj_this);
  jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name","Ljava/lang/String;");
  jfieldID id_m = (*env)->GetFieldID(env, class_Mine, "m", "I");
  jfieldID id_n = (*env)->GetFieldID(env, class_Mine, "n", "I");

  jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);
  jint jm = (*env)->GetIntField(env, obj_this, id_m);
  jint jn = (*env)->GetIntField(env, obj_this, id_n);
  const char *cname = (*env)->GetStringUTFChars(env, jname, NULL);

  jfieldID id_x;
  jbooleanArray jx;
  jboolean *cx = NULL;

  int dimension[2];

  int i = 0;
  int cm = 0, cn = 0;

	strErr = getNamedVarDimension(pvApiCtx, (char*)cname, &dimension[0], &dimension[1]);
	if(strErr.iErr)
	{
		fprintf(stderr,"%s", getErrorMessage(strErr));
	  (*env)->ReleaseStringUTFChars(env, jname , cname);
	  fprintf(stderr,"Error in Java_javasci_SciBooleanArray_GetElement (1).\n");
	  return Value;
  }

  if (dimension[0] != jm)
  {
	  fprintf(stderr,"Error in Java_javasci_SciBooleanArray_GetElement (2).\n");
	  (*env)->ReleaseStringUTFChars(env, jname , cname);
	  return Value;
  }

  if (dimension[1] != jn)
  {
	  fprintf(stderr,"Error in Java_javasci_SciBooleanArray_GetElement (3).\n");
	  (*env)->ReleaseStringUTFChars(env, jname , cname);
	  return Value;
  }

  id_x = (*env)->GetFieldID(env, class_Mine, "x", "[Z");
  jx = (*env)->GetObjectField(env, obj_this, id_x);
  cx = (*env)->GetBooleanArrayElements(env, jx, NULL); 

  CX = (int*) MALLOC(sizeof(int)*(jm * jn));

  if (CX == NULL)
  {
	  fprintf(stderr,"Error in Java_javasci_SciBooleanArray_GetElement (4).\n");
	  (*env)->ReleaseBooleanArrayElements(env, jx, cx, 0);
	  return 1;
  }

	strErr = readNamedMatrixOfBoolean(pvApiCtx, (char*)cname, &cm, &cn, CX);
	if(strErr.iErr)
	{
		fprintf(stderr,"%s", getErrorMessage(strErr));
	  fprintf(stderr,"Error in Java_javasci_SciBooleanArray_GetElement (5).\n");
	  (*env)->ReleaseBooleanArrayElements(env, jx, cx, 0);
	  return 1;
  }

  for(i = 0; i < cm*cn; i++)
  {
	  cx[i] = (jboolean)CX[i];
  }

  FREE(CX); CX = NULL;

  if ( (indrarg <= 0) || (indcarg <= 0) )
  {
	  (*env)->ReleaseBooleanArrayElements(env, jx, cx, 0);
	  (*env)->ReleaseStringUTFChars(env, jname , cname);
	  fprintf(stderr,"Error with int indr & int indc must be >0.\n");
	  return Value;
  }

  if ( (indrarg > jm) || (indcarg > jn) )
  {
	  (*env)->ReleaseBooleanArrayElements(env, jx, cx, 0);
	  (*env)->ReleaseStringUTFChars(env, jname , cname);
	  fprintf(stderr,"Error with int indr & int indc.\n");
	  return Value;
  }

  Value = (jboolean)cx[(indcarg - 1) * cm + (indrarg - 1)];
  
  (*env)->ReleaseBooleanArrayElements(env, jx, cx, 0);
  (*env)->ReleaseStringUTFChars(env, jname , cname);

  return Value;

}
/*--------------------------------------------------------------------------*/

