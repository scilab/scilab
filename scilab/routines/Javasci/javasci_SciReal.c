#include "javasci_SciReal.h"
/********************************************************************************************************/
/* Copyright Cermics/Enpc : jean-philippe Chancelier */
/* Modifications et ameliorations Allan CORNET */
/* INRIA 2005 */
/********************************************************************************************************/

/********************************************************************************************************/
/* public native void Job(String job); */
JNIEXPORT void JNICALL Java_javasci_SciReal_Job(JNIEnv *env , jobject obj_this, jstring job)
/********************************************************************************************************/
{
  int cm,cn,lp;
  const char *cname,*cjob;
  /* get the class */
  jclass class_Mine = (*env)->GetObjectClass(env, obj_this);
  /* get the fields i.e x,m,n,name  */
  jfieldID id_x = (*env)->GetFieldID(env, class_Mine, "x", "[D");
  jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name", 
					 "Ljava/lang/String;");
  jfieldID id_m = (*env)->GetFieldID(env, class_Mine, "m", "I");
  jfieldID id_n = (*env)->GetFieldID(env, class_Mine, "n", "I");
  /* get the field value */
  jdoubleArray jx = (*env)->GetObjectField(env, obj_this, id_x);
  jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);

  jint jm = (*env)->GetIntField(env, obj_this, id_m);
  jint jn = (*env)->GetIntField(env, obj_this, id_n);
  double *cx = (*env)->GetDoubleArrayElements(env,jx,NULL);
  cname = (*env)->GetStringUTFChars(env, jname, NULL);
  cjob = (*env)->GetStringUTFChars(env, job, NULL);

  if ( GetInterfState() == 0) { EnableInterf(); Initialize();} 
  cm=jm;cn=jn;
  if (!  C2F(cwritemat)((char *)cname,&cm,&cn,cx,(unsigned long)strlen(cname)))
  {
	   fprintf(stderr,"erreur lors de l'envoi a scilab \n");
  }
  else 
  { 
    if ( send_scilab_job((char *)cjob) != 0) 
      {
		fprintf(stderr,"Bug in scilab \n");
      }
    else 
      {
		if ( ! C2F(cmatptr)((char *)cname, &cm, &cn, &lp,(unsigned long)strlen(cname))) 
		{
			fprintf(stderr,"erreur lors de la reception \n");
		}
		else 
		if (! C2F(creadmat)((char *)cname,&cm, &cn,cx,(unsigned long)strlen(cname) ))
		{	
			fprintf(stderr,"erreur lors de la reception \n");
		}
      }
  }
  (*env)->ReleaseStringUTFChars(env, jname , cname);
  (*env)->ReleaseStringUTFChars(env, job , cjob);
  (*env)->ReleaseDoubleArrayElements(env,jx,cx,0);
}
/********************************************************************************************************/
/*! public native void Get(); */
JNIEXPORT void JNICALL Java_javasci_SciReal_Get(JNIEnv *env , jobject obj_this)
/********************************************************************************************************/
{
  int cm,cn,lp;
  const char *cname; 
  /* get the class */
  jclass class_Mine = (*env)->GetObjectClass(env, obj_this);
  /* get the fields i.e x,m,n,name  */
  jfieldID id_x = (*env)->GetFieldID(env, class_Mine, "x", "[D");
  jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name", 
					 "Ljava/lang/String;");
  jfieldID id_m = (*env)->GetFieldID(env, class_Mine, "m", "I");
  jfieldID id_n = (*env)->GetFieldID(env, class_Mine, "n", "I");
  /* get the field value */
  jdoubleArray jx = (*env)->GetObjectField(env, obj_this, id_x);
  jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);

  jint jm = (*env)->GetIntField(env, obj_this, id_m);
  jint jn = (*env)->GetIntField(env, obj_this, id_n);
  double *cx = (*env)->GetDoubleArrayElements(env,jx,NULL);
  cname = (*env)->GetStringUTFChars(env, jname, NULL);

  if ( GetInterfState() == 0) { EnableInterf(); Initialize();} 
  cm=jm;cn=jn;
  
  if ( ! C2F(cmatptr)((char *)cname, &cm, &cn, &lp,(unsigned long)strlen(cname))) 
  {
      fprintf(stderr,"erreur lors de la reception \n");
  }
  else 
  if (! C2F(creadmat)((char *)cname,&cm, &cn,cx,(unsigned long)strlen(cname) ))
	{	
		fprintf(stderr,"erreur lors de la reception \n");
	}
	
  (*env)->ReleaseStringUTFChars(env, jname , cname);
  (*env)->ReleaseDoubleArrayElements(env,jx,cx,0);
}

	
/********************************************************************************************************/
/*! public native void Send(); */
JNIEXPORT void JNICALL Java_javasci_SciReal_Send(JNIEnv *env , jobject obj_this)
/********************************************************************************************************/
{
  int cm,cn;
  const char *cname; 
  /* get the class */
  jclass class_Mine = (*env)->GetObjectClass(env, obj_this);
  /* get the fields i.e x,m,n,name  */
  jfieldID id_x = (*env)->GetFieldID(env, class_Mine, "x", "[D");
  jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name", 
					 "Ljava/lang/String;");
  jfieldID id_m = (*env)->GetFieldID(env, class_Mine, "m", "I");
  jfieldID id_n = (*env)->GetFieldID(env, class_Mine, "n", "I");
  /* get the field value */
  jdoubleArray jx = (*env)->GetObjectField(env, obj_this, id_x);
  jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);

  jint jm = (*env)->GetIntField(env, obj_this, id_m);
  jint jn = (*env)->GetIntField(env, obj_this, id_n);
  double *cx = (*env)->GetDoubleArrayElements(env,jx,NULL);
  cname = (*env)->GetStringUTFChars(env, jname, NULL);

  if ( GetInterfState() == 0) { EnableInterf(); Initialize();} 
  cm=jm;cn=jn;
  if (!  C2F(cwritemat)((char *)cname,&cm,&cn,cx,(unsigned long)strlen(cname))) 
    fprintf(stderr,"erreur lors de l'envoi a scilab \n");
  (*env)->ReleaseStringUTFChars(env, jname , cname);
  (*env)->ReleaseDoubleArrayElements(env,jx,cx,0);
}
/********************************************************************************************************/
