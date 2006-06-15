#include "javasci_Scilab.h"
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA */
/********************************************************************************************************/
JNIEXPORT void JNICALL Java_javasci_Scilab_Initialize (JNIEnv *env, jclass cl)
{
	if ( GetInterfState() == 0) { EnableInterf(); Initialize();} 
}
/********************************************************************************************************/
/* public static native void Events(); */
JNIEXPORT void JNICALL Java_javasci_Scilab_Events(JNIEnv *env , jobject obj_this)
/********************************************************************************************************/
{
	ScilabDoOneEvent();
}
/********************************************************************************************************/
/* public static native boolean HaveAGraph(); */
JNIEXPORT jboolean JNICALL Java_javasci_Scilab_HaveAGraph (JNIEnv *env , jobject obj_this)
/********************************************************************************************************/
{
	integer iflag =0,ids,num;
	jboolean bOK=0;

	if (version_flag() == 0)
	{
		sciGetIdFigure (&ids,&num,&iflag);
		if (num > 0) bOK=1;
		
	}/* NG end*/
	else
	{
		C2F(getwins)(&num,&ids ,&iflag);
		if (num > 0) bOK=1;
  } 
 
	return bOK;
}
/********************************************************************************************************/
/* public static native boolean Exec(String job); */
JNIEXPORT jboolean JNICALL Java_javasci_Scilab_Exec(JNIEnv *env , jclass cl, jstring job)
/********************************************************************************************************/
{
  jboolean bOK=0;
  const char *cjob;

  cjob = (*env)->GetStringUTFChars(env, job, NULL);

  if (strlen(cjob) >= MAX_STR)
  {
	  fprintf(stderr,"Error in Java_javasci_Scilab_Exec routine (line too long).\n");
	  bOK=0;
  }
  else
  {
	  if ( send_scilab_job((char *)cjob) != 0) 
	  {
		  fprintf(stderr,"Error in Java_javasci_Scilab_Exec routine.\n");
		  bOK=0;
	  }
	  else bOK=1;
  }

  (*env)->ReleaseStringUTFChars(env, job , cjob);

  return bOK;
}
/********************************************************************************************************/
/* public static native boolean Finish(); */
JNIEXPORT jboolean JNICALL Java_javasci_Scilab_Finish (JNIEnv *env , jobject obj_this)
{
	jboolean bOK=0;
	bOK=TerminateScilab(NULL);
	return bOK;
}
/********************************************************************************************************/
/* public static native boolean ExistVar(String VarName); */
JNIEXPORT jboolean JNICALL Java_javasci_Scilab_ExistVar(JNIEnv *env , jclass cl, jstring VarName)
{
  jboolean bOK=0;
  const char *cVarName;

  cVarName = (*env)->GetStringUTFChars(env, VarName, NULL);

  if (strlen(cVarName) >= MAX_STR)
  {
	  fprintf(stderr,"Error in Java_javasci_Scilab_ExistVar routine (line too long).\n");
	  bOK=0;
  }
  else
  {
	  int lw; int fin;
	  
	  if (C2F(objptr)((char*)cVarName,&lw,&fin,(unsigned long)strlen(cVarName)))
	  {
		  bOK=1;
	  }
	  else
	  {
		  bOK=0;
	  }
  }

  (*env)->ReleaseStringUTFChars(env, VarName , cVarName);

  return bOK;
}
/********************************************************************************************************/
/* public static native int TypeVar(String VarName); */
JNIEXPORT jint JNICALL Java_javasci_Scilab_TypeVar(JNIEnv *env , jclass cl, jstring VarName)
{
  jint type=-1;
  const char *cVarName;

  cVarName = (*env)->GetStringUTFChars(env, VarName, NULL);

  if (strlen(cVarName) >= MAX_STR)
  {
	  fprintf(stderr,"Error in Java_javasci_Scilab_ExistVar routine (line too long).\n");
	  type=-1;
  }
  else
  {
	  int lw; int fin;
	  
	  if (C2F(objptr)((char*)cVarName,&lw,&fin,(unsigned long)strlen(cVarName)))
	  {
		  int *header=NULL; 
		  header = (int *)GetDataFromName((char *)cVarName);
		  type = header[0];
	  }
	  else
	  {
		  type =-1;
		  
	  }
  }

  (*env)->ReleaseStringUTFChars(env, VarName , cVarName);

  return type;
}
/********************************************************************************************************/
/* public static native int GetLastErrorCode(); */
JNIEXPORT jint JNICALL Java_javasci_Scilab_GetLastErrorCode (JNIEnv *env , jobject obj_this)
{
	jint ErrorCode=0;

	ErrorCode=GetLastErrorCode();

	return ErrorCode;
}
/********************************************************************************************************/
