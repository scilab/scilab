#include "javasci_SciStringArray.h"
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2006 */
/********************************************************************************************************/
#define DefaultMaxlenString 1024
/********************************************************************************************************/
/* private static native void Initialize(); */
JNIEXPORT void JNICALL Java_javasci_SciStringArray_Initialize (JNIEnv *env, jclass cl)
{
	if ( GetInterfState() == 0) { EnableInterf(); Initialize();} 
}
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
  int cm,cn;
  const char *cname; 
  int indx, indy, nlr;
  char *tmpStr=MALLOC(sizeof(char)*DefaultMaxlenString);
  
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

  FREE(tmpStr);

  return StrReturn;
}
/********************************************************************************************************/
/*! private native void SendString(String str,int indx, int indy); */
JNIEXPORT void JNICALL Java_javasci_SciStringArray_SendString(JNIEnv *env , jobject obj_this,jstring strarg,jint indxarg, jint indyarg)
/********************************************************************************************************/
{
	const char *cname;
	const char *cstr;
	char Job[DefaultMaxlenString];
	int lencstr=0;

	/* get the class */
	jclass class_Mine = (*env)->GetObjectClass(env, obj_this);

	/* get the fields i.e x,name  */
	jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name","Ljava/lang/String;");

	/* get the field value */
	jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);

	cname = (*env)->GetStringUTFChars(env, jname, NULL);
	cstr = (*env)->GetStringUTFChars(env, strarg, NULL);

	lencstr=(int)strlen(cstr);

	if (!C2F(cwritechain)("TMP_SendString",&lencstr,(char*)cstr,(int)strlen("TMP_SendString"),(int)strlen(cstr)) )
	{
		fprintf(stderr,"Error in Java_javasci_SciStringArray_SendString routine (1).\n");
	}
	else
	{
		sprintf(Job,"%s(%d,%d)=TMP_SendString;",cname,indyarg+1,indxarg+1);
		if (send_scilab_job(Job))
		{
			fprintf(stderr,"Error in Java_javasci_SciStringArray_SendString routine (2).\n");
		}
		else
		{
			sprintf(Job,"clear TMP_SendString;");
			if (send_scilab_job(Job))
			{
				fprintf(stderr,"Error in Java_javasci_SciStringArray_SendString routine (3).\n");
			}
		}
	}

	(*env)->ReleaseStringUTFChars(env, jname , cname);
	(*env)->ReleaseStringUTFChars(env, strarg , cstr);
}
/********************************************************************************************************/
