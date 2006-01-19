#include "javasci_SciString.h"
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2005 */
/********************************************************************************************************/

/********************************************************************************************************/
/* public native void Job(String job); */
JNIEXPORT void JNICALL Java_javasci_SciString_Job(JNIEnv *env , jobject obj_this, jstring job)
/********************************************************************************************************/
{
	const char *cjob;

	/* get the class */
	jclass class_Mine = (*env)->GetObjectClass(env, obj_this);

	/* get the field value */
	cjob = (*env)->GetStringUTFChars(env, job, NULL);

	if ( GetInterfState() == 0) { EnableInterf(); Initialize();} 

	if (send_scilab_job((char*)cjob))
	{
		fprintf(stderr,"Error in Java_javasci_SciString_Job routine.\n");
	}

	(*env)->ReleaseStringUTFChars(env, job , cjob);

}
/********************************************************************************************************/
/*! private native String GetString(); */
JNIEXPORT jstring JNICALL Java_javasci_SciString_GetString(JNIEnv *env , jobject obj_this)
/********************************************************************************************************/
{
  const char *cname;
  char StrValue[1024];

  int lencname=0;
  int lenStrValue=1024;
  int m,n;
  
  /* get the class */
  jclass class_Mine = (*env)->GetObjectClass(env, obj_this);
  /* get the fields i.e name */
  jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name","Ljava/lang/String;");
  
  /* get the field value */
  jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);
    
  cname = (*env)->GetStringUTFChars(env, jname, NULL);
    
  if ( GetInterfState() == 0) { EnableInterf(); Initialize();} 

  m=1;
  n=1;
  
  if (!C2F(creadchains)((char*)cname, &m, &n, &lenStrValue, StrValue, (unsigned long)strlen(cname), (unsigned long)strlen(StrValue)) )
  {
  	fprintf(stderr,"Error in Java_javasci_SciString_GetString routine.\n");
  }
  	
  (*env)->ReleaseStringUTFChars(env, jname , cname);

  return (*env)->NewStringUTF(env, StrValue);	
}
/********************************************************************************************************/
/*! public native void Send(); */
JNIEXPORT void JNICALL Java_javasci_SciString_Send(JNIEnv *env , jobject obj_this)
/********************************************************************************************************/
{
  const char *cname;
  const char *StrValue; 

  int lencname=0;
  int lenStrValue=0;
  
  /* get the class */
  jclass class_Mine = (*env)->GetObjectClass(env, obj_this);
  /* get the fields i.e name,StrValue  */
  jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name","Ljava/lang/String;");
  jfieldID id_StrValue =  (*env)->GetFieldID(env, class_Mine, "StrValue","Ljava/lang/String;");
  
  /* get the field value */
  jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);
  jstring jStrValue = (jstring) (*env)->GetObjectField(env, obj_this, id_StrValue);
  
  cname = (*env)->GetStringUTFChars(env, jname, NULL);
  StrValue = (*env)->GetStringUTFChars(env, jStrValue, NULL);

  if ( GetInterfState() == 0) { EnableInterf(); Initialize();} 
  	
  lencname=(int)strlen(cname);
  lenStrValue=(int)strlen(StrValue);

  if (!C2F(cwritechain)((char*)cname,&lenStrValue,(char*)StrValue,(unsigned long)lencname,(unsigned long)lenStrValue ))
  { 
	  fprintf(stderr,"Error in Java_javasci_SciString_Send routine.\n");
  }
  
  (*env)->ReleaseStringUTFChars(env, jname , cname);
  (*env)->ReleaseStringUTFChars(env, jname , StrValue);
    
}
/********************************************************************************************************/
