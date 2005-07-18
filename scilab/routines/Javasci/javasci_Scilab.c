#include "javasci_Scilab.h"
/********************************************************************************************************/
/* Copyright Cermics/Enpc : jean-philippe Chancelier */
/* Modifications et ameliorations Allan CORNET */
/* INRIA 2005 */
/********************************************************************************************************/
/* public static native void Events(); */
JNIEXPORT void JNICALL Java_javasci_Scilab_Events(JNIEnv *env , jobject obj_this)
/********************************************************************************************************/
{
	ScilabDoOneEvent();
}
/********************************************************************************************************/
/* public static native int HaveAGraph(); */
JNIEXPORT jint JNICALL Java_javasci_Scilab_HaveAGraph (JNIEnv *env , jobject obj_this)
/********************************************************************************************************/
{
	integer iflag =0,ids,num;
	jint vInt=0;

	if (version_flag() == 0)
	{
		sciGetIdFigure (&ids,&num,&iflag);
		if (num > 0) vInt=1;
		
	}/* NG end*/
	else
	{
		C2F(getwins)(&num,&ids ,&iflag);
		if (num > 0) vInt=1;
    } 
 
	return vInt;
}
/********************************************************************************************************/
/* public static native void Exec(String job); */
JNIEXPORT void JNICALL Java_javasci_Scilab_Exec(JNIEnv *env , jclass cl, jstring job)
/********************************************************************************************************/
{
  const char *cjob;
  cjob = (*env)->GetStringUTFChars(env, job, NULL);
  if ( GetInterfState() == 0) { EnableInterf(); Initialize();} 
  if ( send_scilab_job((char *)cjob) != 0) 
    {
      fprintf(stderr,"Bug in scilab \n");
    }
  (*env)->ReleaseStringUTFChars(env, job , cjob);
}
/********************************************************************************************************/