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

#include <stdio.h>
#include "javasci_Scilab.h"
#include "CallScilab.h"
#include "../../../modules/graphics/includes/WindowList.h"

JNIEXPORT void JNICALL Java_javasci_Scilab_Initialize (JNIEnv *env, jclass cl)
{
	if ( GetInterfState() == 0) 
	{ 
		EnableInterf();
		Initialize();
	} 

}
/*****************************************************************************/
/* public static native void Events(); */
JNIEXPORT void JNICALL Java_javasci_Scilab_Events(JNIEnv *env , jobject obj_this)
/*****************************************************************************/
{
	ScilabDoOneEvent();
}
/*****************************************************************************/
/* public static native boolean HaveAGraph(); */
JNIEXPORT jboolean JNICALL Java_javasci_Scilab_HaveAGraph (JNIEnv *env , jobject obj_this)
/*****************************************************************************/
{
	
    if (sciHasFigures()) return JNI_TRUE;

	return JNI_FALSE;
}
/*****************************************************************************/
/* public static native boolean Exec(String job); */
JNIEXPORT jboolean JNICALL Java_javasci_Scilab_Exec(JNIEnv *env , jclass cl, jstring job)
/*****************************************************************************/
{
  jboolean bOK=JNI_TRUE;
  const char *cjob;

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
		  bOK=JNI_FALSE;
	  }
	  else bOK=JNI_TRUE;
	  fflush(stdout);
  }

  (*env)->ReleaseStringUTFChars(env, job , cjob);

  return bOK;
}
/*****************************************************************************/
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
/*****************************************************************************/
/* public static native boolean ExistVar(String varName); */
JNIEXPORT jboolean JNICALL Java_javasci_Scilab_ExistVar(JNIEnv *env , jclass cl, jstring varName)
{
	jboolean bOK=JNI_FALSE;
  const char *cvarName;

  cvarName = (*env)->GetStringUTFChars(env, varName, NULL);

  if (strlen(cvarName) >= MAX_STR)
  {
	  fprintf(stderr,"Error in Java_javasci_Scilab_ExistVar routine (line too long).\n");
	  bOK=JNI_FALSE;
  }
  else
  {
	  int lw; int fin;
	  
	  if (C2F(objptr)((char*)cvarName,&lw,&fin,(unsigned long)strlen(cvarName)))
	  {
		  bOK=JNI_TRUE;
	  }
	  else
	  {
		  bOK=JNI_FALSE;
	  }
  }

  (*env)->ReleaseStringUTFChars(env, varName , cvarName);

  return bOK;
}
/*****************************************************************************/
/* public static native int TypeVar(String varName); */
JNIEXPORT jint JNICALL Java_javasci_Scilab_TypeVar(JNIEnv *env , jclass cl, jstring varName)
{
  jint type=-1;
  const char *cvarName;

  cvarName = (*env)->GetStringUTFChars(env, varName, NULL);

  if (strlen(cvarName) >= MAX_STR)
  {
	  fprintf(stderr,"Error in Java_javasci_Scilab_ExistVar routine (line too long).\n");
	  type=-1;
  }
  else
  {
	  int lw; int fin;
	  
	  if (C2F(objptr)((char*)cvarName,&lw,&fin,(unsigned long)strlen(cvarName)))
	  {
		  int *header=NULL; 
		  header = (int *)GetDataFromName((char *)cvarName);
		  type = header[0];
	  }
	  else
	  {
		  type =-1;
		  
	  }
  }

  (*env)->ReleaseStringUTFChars(env, varName , cvarName);

  return type;
}
/*****************************************************************************/
/* public static native int GetLastErrorCode(); */
JNIEXPORT jint JNICALL Java_javasci_Scilab_GetLastErrorCode (JNIEnv *env , jobject obj_this)
{
	return GetLastErrorCode();
}
/*****************************************************************************/
