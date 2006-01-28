#include "javasci_Scilab.h"
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2005 */
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
	  if ( GetInterfState() == 0) { EnableInterf(); Initialize();} 

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
