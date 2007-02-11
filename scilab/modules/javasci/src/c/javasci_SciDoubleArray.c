#include "javasci_SciDoubleArray.h"
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2005 */
/********************************************************************************************************/
/* private static native void Initialize(); */
JNIEXPORT void JNICALL Java_javasci_SciDoubleArray_Initialize (JNIEnv *env, jclass cl)
{
	if ( GetInterfState() == 0) { EnableInterf(); Initialize();} 
}
/********************************************************************************************************/
/* public native boolean Job(String job); */
JNIEXPORT jboolean JNICALL Java_javasci_SciDoubleArray_Job(JNIEnv *env , jobject obj_this, jstring job)
/********************************************************************************************************/
{
	const char *cjob;
	jboolean bOK=0;

	/* get the class */
	jclass class_Mine = (*env)->GetObjectClass(env, obj_this);

	/* get the field value */
	cjob = (*env)->GetStringUTFChars(env, job, NULL);

	if (strlen(cjob) >= MAX_STR)
	{
		fprintf(stderr,"Error in Java_javasci_SciDoubleArray_Job routine (line too long).\n");
		bOK=0;
	}
	else
	{
		if (send_scilab_job((char*)cjob))
		{
			fprintf(stderr,"Error in Java_javasci_SciDoubleArray_Job routine.\n");
			bOK=0;
		}
		else bOK=1;
	}

	(*env)->ReleaseStringUTFChars(env, job , cjob);

	return bOK;

}
/********************************************************************************************************/
/* private native int getRowFromScilab(String name); */
JNIEXPORT jint JNICALL Java_javasci_SciDoubleArray_getRowFromScilab(JNIEnv *env , jobject obj_this, jstring name)
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
		#define SCIMATRIXREALORCOMPLEXTYPE 1
		#define COMPLEX 1
		
		int *header=NULL; 
		header = (int *)GetDataFromName((char *)cname);
		

		if ( SCIMATRIXREALORCOMPLEXTYPE == header[0] )
		{
			int Cmplx=header[3];

			if (Cmplx != COMPLEX)
			{
				row = header[1];
			}
		}
	}

	(*env)->ReleaseStringUTFChars(env, name , cname);

	return row;
}
/********************************************************************************************************/
/* private native int getColFromScilab(String name); */
JNIEXPORT jint JNICALL Java_javasci_SciDoubleArray_getColFromScilab(JNIEnv *env , jobject obj_this, jstring name)
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
		#define SCIMATRIXREALORCOMPLEXTYPE 1
		#define COMPLEX 1

		int *header=NULL; 
		header = (int *)GetDataFromName((char *)cname);

		if ( SCIMATRIXREALORCOMPLEXTYPE == header[0] )
		{
			int Cmplx=header[3];

			if (Cmplx != COMPLEX)
			{
				col = header[2];
			}
		}
	}
	(*env)->ReleaseStringUTFChars(env, name , cname);

	return col;
}
/********************************************************************************************************/
/*! public native void Get(); */
JNIEXPORT void JNICALL Java_javasci_SciDoubleArray_Get(JNIEnv *env , jobject obj_this)
/********************************************************************************************************/
{
  int cm,cn,lp;
  const char *cname; 

  /* get the class */
  jclass class_Mine = (*env)->GetObjectClass(env, obj_this);

  /* get the fields i.e x,m,n,name  */
  jfieldID id_x = (*env)->GetFieldID(env, class_Mine, "x", "[D");
  jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name","Ljava/lang/String;");
  jfieldID id_m = (*env)->GetFieldID(env, class_Mine, "m", "I");
  jfieldID id_n = (*env)->GetFieldID(env, class_Mine, "n", "I");

  /* get the field value */
  jdoubleArray jx = (*env)->GetObjectField(env, obj_this, id_x);
  jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);

  jint jm = (*env)->GetIntField(env, obj_this, id_m);
  jint jn = (*env)->GetIntField(env, obj_this, id_n);
  double *cx = (*env)->GetDoubleArrayElements(env,jx,NULL);
  cname = (*env)->GetStringUTFChars(env, jname, NULL);

  cm=jm;cn=jn;
  
  if ( ! C2F(cmatptr)((char *)cname, &cm, &cn, &lp,(unsigned long)strlen(cname))) 
  {
      fprintf(stderr,"Error in Java_javasci_SciDoubleArray_Get (1).\n");
  }
  else 
  if (! C2F(creadmat)((char *)cname,&cm, &cn,cx,(unsigned long)strlen(cname) ))
  {	
	fprintf(stderr,"Error in Java_javasci_SciDoubleArray_Get (2).\n");
  }
	
  (*env)->ReleaseStringUTFChars(env, jname , cname);
  (*env)->ReleaseDoubleArrayElements(env,jx,cx,0);
}

	
/********************************************************************************************************/
/*! public native void Send(); */
JNIEXPORT void JNICALL Java_javasci_SciDoubleArray_Send(JNIEnv *env , jobject obj_this)
/********************************************************************************************************/
{
  int cm,cn;
  const char *cname; 

  /* get the class */
  jclass class_Mine = (*env)->GetObjectClass(env, obj_this);

  /* get the fields i.e x,m,n,name  */
  jfieldID id_x = (*env)->GetFieldID(env, class_Mine, "x", "[D");
  jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name","Ljava/lang/String;");
  jfieldID id_m = (*env)->GetFieldID(env, class_Mine, "m", "I");
  jfieldID id_n = (*env)->GetFieldID(env, class_Mine, "n", "I");

  /* get the field value */
  jdoubleArray jx = (*env)->GetObjectField(env, obj_this, id_x);
  jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);

  jint jm = (*env)->GetIntField(env, obj_this, id_m);
  jint jn = (*env)->GetIntField(env, obj_this, id_n);
  double *cx = (*env)->GetDoubleArrayElements(env,jx,NULL);
  cname = (*env)->GetStringUTFChars(env, jname, NULL);

  cm=jm;cn=jn;
  if (!C2F(cwritemat)((char *)cname,&cm,&cn,cx,(unsigned long)strlen(cname))) 
  {
  	fprintf(stderr,"Error in Java_javasci_SciDoubleArray_Send.\n");
  }
 
  (*env)->ReleaseStringUTFChars(env, jname , cname);
  (*env)->ReleaseDoubleArrayElements(env,jx,cx,0);
}
/********************************************************************************************************/
/*! public native double GetElement(int indr, int indc); */
JNIEXPORT jdouble JNICALL Java_javasci_SciDoubleArray_GetElement(JNIEnv *env , jobject obj_this,jint indrarg, jint indcarg)
{
  int cm,cn,lp;
  const char *cname; 
  double Value=0.0;

  /* get the class */
  jclass class_Mine = (*env)->GetObjectClass(env, obj_this);

  /* get the fields i.e x,m,n,name  */
  jfieldID id_x = (*env)->GetFieldID(env, class_Mine, "x", "[D");
  jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name","Ljava/lang/String;");
  jfieldID id_m = (*env)->GetFieldID(env, class_Mine, "m", "I");
  jfieldID id_n = (*env)->GetFieldID(env, class_Mine, "n", "I");

  /* get the field value */
  jdoubleArray jx = (*env)->GetObjectField(env, obj_this, id_x);
  jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);

  jint jm = (*env)->GetIntField(env, obj_this, id_m);
  jint jn = (*env)->GetIntField(env, obj_this, id_n);
  double *cx = (*env)->GetDoubleArrayElements(env,jx,NULL);
  cname = (*env)->GetStringUTFChars(env, jname, NULL);

  cm=jm;cn=jn;

  if ( ! C2F(cmatptr)((char *)cname, &cm, &cn, &lp,(unsigned long)strlen(cname))) 
  {
  	fprintf(stderr,"Error in Java_javasci_SciDoubleArray_GetElement (1).\n");
  }
  else 
  if (! C2F(creadmat)((char *)cname,&cm, &cn,cx,(unsigned long)strlen(cname) ))
  {	
	fprintf(stderr,"Error in Java_javasci_SciDoubleArray_GetElement (2).\n");
  }

  (*env)->ReleaseStringUTFChars(env, jname , cname);

  if ( (indrarg<=0) || (indcarg<=0) )
  {
	fprintf(stderr,"Error with int indr & int indc must be >0.\n");
	return 0.0;
  }

  if ( (indrarg>jm) || (indcarg>jn) )
  {
	  fprintf(stderr,"Error with int indr & int indc.\n");
	  return 0.0;
  }
  
  Value=cx[(indcarg-1)*cm+(indrarg-1)];
  
   return Value;

}
/********************************************************************************************************/
