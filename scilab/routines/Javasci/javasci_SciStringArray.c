#include "javasci_SciStringArray.h"
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2006 */
/********************************************************************************************************/

/********************************************************************************************************/
/* public native void Job(String job); */
JNIEXPORT void JNICALL Java_javasci_SciStringArray_Job(JNIEnv *env , jobject obj_this, jstring job)
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
		fprintf(stderr,"Error in Java_javasci_SciStringArray_Job routine.\n");
	}

	(*env)->ReleaseStringUTFChars(env, job , cjob);

}
/********************************************************************************************************/
/*! public native String GetElement(int indr, int indc); */
JNIEXPORT jstring JNICALL Java_javasci_SciStringArray_GetElement(JNIEnv *env , jobject obj_this,jint indrarg, jint indcarg)
/********************************************************************************************************/
{
  #define DefaultMaxlenString 1024
  int cm,cn;
  const char *cname; 
  int indx, indy, nlr;
  char *tmpStr=malloc(sizeof(char)*DefaultMaxlenString);
  
  /* get the class */
  jclass class_Mine = (*env)->GetObjectClass(env, obj_this);
  
  /* get the fields i.e x,m,n,name  */
  jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name","Ljava/lang/String;");
  jfieldID id_m = (*env)->GetFieldID(env, class_Mine, "m", "I");
  jfieldID id_n = (*env)->GetFieldID(env, class_Mine, "n", "I");
  
   /* get the field value */
  jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);
  jint jm = (*env)->GetIntField(env, obj_this, id_m);
  jint jn = (*env)->GetIntField(env, obj_this, id_n);

  jstring StrReturn;
  
  cname = (*env)->GetStringUTFChars(env, jname, NULL);

  if ( GetInterfState() == 0) { EnableInterf(); Initialize();} 
  	
  cm=jm;
  cn=jn;
  
  indx= indrarg;
  indy = indcarg;

  nlr = DefaultMaxlenString;

  if (!C2F(creadchains)((char *)cname, &indx, &indy, &nlr, tmpStr, (unsigned long)strlen(cname), (unsigned long)strlen(tmpStr)) )
  {
	  fprintf(stderr,"Error in Java_javasci_SciStringArray_GetElement routine.\n");
  }

  (*env)->ReleaseStringUTFChars(env, jname , cname);

  StrReturn=(*env)->NewStringUTF(env, tmpStr);

  free(tmpStr);

  return StrReturn;
}
/********************************************************************************************************/
/*! private native void SendString(String str,int indx, int indy); */
JNIEXPORT void JNICALL Java_javasci_SciStringArray_SendString(JNIEnv *env , jobject obj_this,jstring strarg,jint indxarg, jint indyarg)
/********************************************************************************************************/
{
	const char *cname;
	const char *cstr;
	char Job[1024*4];

	/* get the class */
	jclass class_Mine = (*env)->GetObjectClass(env, obj_this);

	/* get the fields i.e x,name  */
	jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name","Ljava/lang/String;");

	/* get the field value */
	jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);

	cname = (*env)->GetStringUTFChars(env, jname, NULL);
	cstr = (*env)->GetStringUTFChars(env, strarg, NULL);

	if ( GetInterfState() == 0) { EnableInterf(); Initialize();} 

	sprintf(Job,"%s(%d,%d)=\"\"%s\"\";",cname,indxarg+1,indyarg+1,cstr);
	if (send_scilab_job(Job))
	{
		fprintf(stderr,"Error in Java_javasci_SciStringArray_SendString routine.\n");
	}

	(*env)->ReleaseStringUTFChars(env, jname , cname);
	(*env)->ReleaseStringUTFChars(env, strarg , cstr);
}
/********************************************************************************************************/
