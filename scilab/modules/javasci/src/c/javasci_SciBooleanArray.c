#include "javasci_SciBooleanArray.h"
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2007 */
/********************************************************************************************************/
/* private static native void Initialize(); */
JNIEXPORT void JNICALL Java_javasci_SciBooleanArray_Initialize (JNIEnv *env, jclass cl)
{
	if ( GetInterfState() == 0) { EnableInterf(); Initialize();} 
}
/********************************************************************************************************/
/* private native int getRowFromScilab(String name); */
JNIEXPORT jint JNICALL Java_javasci_SciBooleanArray_getRowFromScilab(JNIEnv *env , jobject obj_this, jstring name)
{
	int lw; int fin;
	const char *cname=NULL;
	jint row=-1;

	/* get the class */
	jclass class_Mine = (*env)->GetObjectClass(env, obj_this);

	/* get the field value */
	cname = (*env)->GetStringUTFChars(env, name, NULL);
	if (C2F(objptr)((char*)cname,&lw,&fin,(unsigned long)strlen(cname)))
	{
		#define SCIBOOLEANTYPE 4
		int *header=NULL; 
		header = (int *)GetDataFromName((char *)cname);
		if (header[0] == SCIBOOLEANTYPE)
		{
			row = header[1];
		}
	}

	(*env)->ReleaseStringUTFChars(env, name , cname);

	return row;
}
/********************************************************************************************************/
/* private native int getColFromScilab(String name); */
JNIEXPORT jint JNICALL Java_javasci_SciBooleanArray_getColFromScilab(JNIEnv *env , jobject obj_this, jstring name)
{
	int lw; int fin;
	const char *cname=NULL;
	jint col=-1;

	/* get the class */
	jclass class_Mine = (*env)->GetObjectClass(env, obj_this);

	/* get the field value */
	cname = (*env)->GetStringUTFChars(env, name, NULL);

	if (C2F(objptr)((char*)cname,&lw,&fin,(unsigned long)strlen(cname)))
	{
		#define SCIBOOLEANTYPE 4
		int *header=NULL; 
		header = (int *)GetDataFromName((char *)cname);
		if (header[0] == SCIBOOLEANTYPE)
		{
			col = header[2];
		}
	}

	(*env)->ReleaseStringUTFChars(env, name , cname);

	return col;
}
/********************************************************************************************************/
/* public native boolean Job(String job); */
JNIEXPORT jboolean JNICALL Java_javasci_SciBooleanArray_Job(JNIEnv *env , jobject obj_this, jstring job)
/********************************************************************************************************/
{
	const char *cjob = NULL;
	jboolean bOK=0;

	/* get the class */
	jclass class_Mine = (*env)->GetObjectClass(env, obj_this);

	/* get the field value */
	cjob = (*env)->GetStringUTFChars(env, job, NULL);

	if (strlen(cjob) >= MAX_STR)
	{
		fprintf(stderr,"Error in Java_javasci_SciBooleanArray_Job routine (line too long).\n");
		bOK=0;
	}
	else
	{
		if (send_scilab_job((char*)cjob))
		{
			fprintf(stderr,"Error in Java_javasci_SciBooleanArray_Job routine.\n");
			bOK=0;
		}
		else bOK=1;
	}

	(*env)->ReleaseStringUTFChars(env, job , cjob);

	return bOK;

}
/********************************************************************************************************/
/*! public native void Get(); */
JNIEXPORT void JNICALL Java_javasci_SciBooleanArray_Get(JNIEnv *env , jobject obj_this)
/********************************************************************************************************/
{
  int cm=0,cn=0,lp=0;
  int i=0;
  const char *cname=NULL; 

  /* get the class */
  jclass class_Mine = (*env)->GetObjectClass(env, obj_this);

  /* get the fields i.e x,m,n,name  */
  jfieldID id_x = (*env)->GetFieldID(env, class_Mine, "x", "[Z");
  jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name","Ljava/lang/String;");
  jfieldID id_m = (*env)->GetFieldID(env, class_Mine, "m", "I");
  jfieldID id_n = (*env)->GetFieldID(env, class_Mine, "n", "I");

  /* get the field value */
  jbooleanArray jx = (*env)->GetObjectField(env, obj_this, id_x);
  jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);

  jint jm = (*env)->GetIntField(env, obj_this, id_m);
  jint jn = (*env)->GetIntField(env, obj_this, id_n);
  jboolean *cx = (*env)->GetBooleanArrayElements(env,jx,NULL);

  int *CX = (int*) MALLOC(sizeof(int)*(jm*jn));

  cname = (*env)->GetStringUTFChars(env, jname, NULL);

  cm=jm;cn=jn;
  
  if ( ! C2F(cmatbptr)((char *)cname, &cm, &cn, &lp,(unsigned long)strlen(cname))) 
  {
      fprintf(stderr,"Error in Java_javasci_SciBooleanArray_Get (1).\n");
  }
  else 
  if (! C2F(creadbmat)((char *)cname,&cm, &cn,CX,(unsigned long)strlen(cname) ))
  {	
	fprintf(stderr,"Error in Java_javasci_SciBooleanArray_Get (2).\n");
  }

  for(i=0;i<cm*cn;i++)
  {
	  cx[i]=(jboolean)CX[i];
  }
	
  (*env)->ReleaseStringUTFChars(env, jname , cname);
  (*env)->ReleaseBooleanArrayElements(env,jx,cx,0);
  if (CX) {FREE(CX);CX=NULL;}
  
}

	
/********************************************************************************************************/
/*! public native void Send(); */
JNIEXPORT void JNICALL Java_javasci_SciBooleanArray_Send(JNIEnv *env , jobject obj_this)
/********************************************************************************************************/
{
  int cm=0,cn=0;
  int i=0;
  const char *cname=NULL; 

  /* get the class */
  jclass class_Mine = (*env)->GetObjectClass(env, obj_this);

  /* get the fields i.e x,m,n,name  */
  jfieldID id_x = (*env)->GetFieldID(env, class_Mine, "x", "[Z");
  jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name","Ljava/lang/String;");
  jfieldID id_m = (*env)->GetFieldID(env, class_Mine, "m", "I");
  jfieldID id_n = (*env)->GetFieldID(env, class_Mine, "n", "I");

  /* get the field value */
  jbooleanArray jx = (*env)->GetObjectField(env, obj_this, id_x);
  jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);

  jint jm = (*env)->GetIntField(env, obj_this, id_m);
  jint jn = (*env)->GetIntField(env, obj_this, id_n);
  jboolean *cx = (*env)->GetBooleanArrayElements(env,jx,NULL);
  int *CX = (int*) MALLOC(sizeof(int)*(jm*jn));

  cname = (*env)->GetStringUTFChars(env, jname, NULL);


  cm=jm;cn=jn;

  for(i=0;i<cm*cn;i++)
  {
	  CX[i]=(int)cx[i];
  }
  if (!C2F(cwritebmat)((char *)cname,&cm,&cn,CX,(unsigned long)strlen(cname))) 
  {
  	fprintf(stderr,"Error in Java_javasci_SciBooleanArray_Send.\n");
  }

  if (CX){FREE(CX);CX=NULL;}
 
  (*env)->ReleaseStringUTFChars(env, jname , cname);
  (*env)->ReleaseBooleanArrayElements(env,jx,cx,0);
}
/********************************************************************************************************/
/*! public native boolean GetElement(int indr, int indc); */
JNIEXPORT jboolean JNICALL Java_javasci_SciBooleanArray_GetElement(JNIEnv *env , jobject obj_this,jint indrarg, jint indcarg)
{
  int cm,cn,lp;
  const char *cname; 
  int Value=0;

  /* get the class */
  jclass class_Mine = (*env)->GetObjectClass(env, obj_this);

  /* get the fields i.e x,m,n,name  */
  jfieldID id_x = (*env)->GetFieldID(env, class_Mine, "x", "[Z");
  jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name","Ljava/lang/String;");
  jfieldID id_m = (*env)->GetFieldID(env, class_Mine, "m", "I");
  jfieldID id_n = (*env)->GetFieldID(env, class_Mine, "n", "I");

  /* get the field value */
  jbooleanArray jx = (*env)->GetObjectField(env, obj_this, id_x);
  jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);

  jint jm = (*env)->GetIntField(env, obj_this, id_m);
  jint jn = (*env)->GetIntField(env, obj_this, id_n);
  jboolean *cx = (*env)->GetBooleanArrayElements(env,jx,NULL);
  int *CX = (int*) MALLOC(sizeof(int)*(jm*jn));

  cname = (*env)->GetStringUTFChars(env, jname, NULL);

  cm=jm;cn=jn;

  if ( ! C2F(cmatbptr)((char *)cname, &cm, &cn, &lp,(unsigned long)strlen(cname))) 
  {
  	fprintf(stderr,"Error in Java_javasci_SciBooleanArray_GetElement (1).\n");
  }
  else 
  {
	  int i=0;
	  if (! C2F(creadbmat)((char *)cname,&cm, &cn,CX,(unsigned long)strlen(cname) ))
	  {	
		  fprintf(stderr,"Error in Java_javasci_SciBooleanArray_GetElement (2).\n");
	  }
	  for(i=0;i<cm*cn;i++)
	  {
		  cx[i]=(jboolean)CX[i];
	  }
	  if (CX) {FREE(CX);CX=NULL;}
	  
  }
  

  (*env)->ReleaseStringUTFChars(env, jname , cname);

  if ( (indrarg<=0) || (indcarg<=0) )
  {
	fprintf(stderr,"Error with int indr & int indc must be >0.\n");
	return 0;
  }

  if ( (indrarg>jm) || (indcarg>jn) )
  {
	  fprintf(stderr,"Error with int indr & int indc.\n");
	  return 0;
  }
  
  Value=(int)cx[(indcarg-1)*cm+(indrarg-1)];
  
  return Value;

}
/********************************************************************************************************/
