#include "javasci_SciComplexArray.h"
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2006 */
/********************************************************************************************************/
/* private static native void Initialize(); */
JNIEXPORT void JNICALL Java_javasci_SciComplexArray_Initialize (JNIEnv *env, jclass cl)
{
	if ( GetInterfState() == 0) { EnableInterf(); Initialize();} 
}
/********************************************************************************************************/
/*! public native boolean Job(String job); */
JNIEXPORT jboolean JNICALL Java_javasci_SciComplexArray_Job(JNIEnv *env , jobject obj_this, jstring job)
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
		fprintf(stderr,"Error in Java_javasci_SciComplexArray_Job routine (line too long).\n");
		bOK=0;
	}
	else
	{
		if (send_scilab_job((char*)cjob))
		{
			fprintf(stderr,"Error in Java_javasci_SciComplexArray_Job routine.\n");
			bOK=0;
		}
		else bOK=1;
	}

	(*env)->ReleaseStringUTFChars(env, job , cjob);

	return bOK;

}
/********************************************************************************************************/
/*! public native void Get(); */
JNIEXPORT void JNICALL Java_javasci_SciComplexArray_Get(JNIEnv *env , jobject obj_this)
/********************************************************************************************************/
{
  int cm,cn,lp;
  const char *cname; 

  /* get the class */
  jclass class_Mine = (*env)->GetObjectClass(env, obj_this);

  /* get the fields i.e x,m,n,name  */
  jfieldID id_x = (*env)->GetFieldID(env, class_Mine, "x", "[D");
  jfieldID id_y = (*env)->GetFieldID(env, class_Mine, "y", "[D");

  jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name","Ljava/lang/String;");
  jfieldID id_m = (*env)->GetFieldID(env, class_Mine, "m", "I");
  jfieldID id_n = (*env)->GetFieldID(env, class_Mine, "n", "I");

  /* get the field value */
  jdoubleArray jx = (*env)->GetObjectField(env, obj_this, id_x);
  jdoubleArray jy = (*env)->GetObjectField(env, obj_this, id_y);

  jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);

  jint jm = (*env)->GetIntField(env, obj_this, id_m);
  jint jn = (*env)->GetIntField(env, obj_this, id_n);

  double *cx = (*env)->GetDoubleArrayElements(env,jx,NULL);
  double *cy = (*env)->GetDoubleArrayElements(env,jy,NULL);

  cname = (*env)->GetStringUTFChars(env, jname, NULL);

  cm=jm;cn=jn;

  if ( ! C2F(cmatcptr)((char *)cname, &cm, &cn, &lp, (unsigned long)strlen(cname))) 
  {
	  fprintf(stderr,"Error in Java_javasci_SciComplexArray_Get (1).\n");
  }
  else
  {
	  double *ComplexArray = NULL;
	  int l=0;
	  ComplexArray = (double*) MALLOC ( sizeof(double)*(jm*jn*2) );
	  if (ComplexArray)
	  {
		  if ( ! C2F(creadcmat)((char *)cname, &cm, &cn, ComplexArray,(unsigned long)strlen(cname)) )
		  {
			fprintf(stderr,"Error in Java_javasci_SciComplexArray_Get (2).\n");
		  }
		  else
		  {
			  for (l=0;l<cm*cn;l++)
			  {
				  cx[l]=ComplexArray[l];
			  }

			  for (l=cm*cn;l<cm*cn*2;l++)
			  {
				  cy[l-cm*cn]=ComplexArray[l];
			  }
		  }

		  FREE(ComplexArray);
		  ComplexArray=NULL;
	  }
	  else
	  {
		  fprintf(stderr,"Error in Java_javasci_SciComplexArray_Get (3).\n");
	  }
  }
	
  (*env)->ReleaseStringUTFChars(env, jname , cname);
  (*env)->ReleaseDoubleArrayElements(env,jx,cx,0);
  (*env)->ReleaseDoubleArrayElements(env,jy,cy,0);
}
/********************************************************************************************************/
/*! public native void Send(); */
JNIEXPORT void JNICALL Java_javasci_SciComplexArray_Send(JNIEnv *env , jobject obj_this)
/********************************************************************************************************/
{
  int cm,cn;
  const char *cname; 
  double *ComplexArray = NULL;

  /* get the class */
  jclass class_Mine = (*env)->GetObjectClass(env, obj_this);

  /* get the fields i.e x,m,n,name  */
  jfieldID id_x = (*env)->GetFieldID(env, class_Mine, "x", "[D");
  jfieldID id_y = (*env)->GetFieldID(env, class_Mine, "y", "[D");
  jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name","Ljava/lang/String;");
  jfieldID id_m = (*env)->GetFieldID(env, class_Mine, "m", "I");
  jfieldID id_n = (*env)->GetFieldID(env, class_Mine, "n", "I");

  /* get the field value */
  jdoubleArray jx = (*env)->GetObjectField(env, obj_this, id_x);
  jdoubleArray jy = (*env)->GetObjectField(env, obj_this, id_y);
  jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);

  jint jm = (*env)->GetIntField(env, obj_this, id_m);
  jint jn = (*env)->GetIntField(env, obj_this, id_n);
  double *cx = (*env)->GetDoubleArrayElements(env,jx,NULL);
  double *cy = (*env)->GetDoubleArrayElements(env,jy,NULL);
  cname = (*env)->GetStringUTFChars(env, jname, NULL);

  cm=jm;cn=jn;

  ComplexArray = (double*) MALLOC ( sizeof(double)*(jm*jn*2) );
  if (ComplexArray)
  {
	  int l=0;
	  for (l=0;l<cm*cn;l++)
	  {
		  ComplexArray[l]=cx[l];
	  }

	  for (l=cm*cn;l<cm*cn*2;l++)
	  {
		  ComplexArray[l]=cy[l-cm*cn];
	  }

	  if (!C2F(cwritecmat)((char *)cname,&cm,&cn,ComplexArray,(unsigned long)strlen(cname)))
	  {
		  fprintf(stderr,"Error in Java_javasci_SciComplexArray_Send (1).\n");
	  }
	  FREE(ComplexArray);
	  ComplexArray = NULL ;
  }
  else
  {
	fprintf(stderr,"Error in Java_javasci_SciComplexArray_Send (2).\n");
  }
  
  (*env)->ReleaseStringUTFChars(env, jname , cname);
  (*env)->ReleaseDoubleArrayElements(env,jx,cx,0);
  (*env)->ReleaseDoubleArrayElements(env,jy,cy,0);
}
/********************************************************************************************************/
/*! public native double GetRealPartElement(int indr,int indc); */
JNIEXPORT jdouble JNICALL Java_javasci_SciComplexArray_GetRealPartElement(JNIEnv *env , jobject obj_this,jint indrarg, jint indcarg)
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

  if ( ! C2F(cmatcptr)((char *)cname, &cm, &cn, &lp, (unsigned long)strlen(cname))) 
  {
	  fprintf(stderr,"Error in Java_javasci_SciComplexArray_GetRealPartElement (1).\n");
  }
  else
  {
	  int l=0;
	  double *ComplexArray = NULL;
	  ComplexArray = (double*) MALLOC ( sizeof(double)*(jm*jn*2) );
	  if (ComplexArray)
	  {
		  if ( ! C2F(creadcmat)((char *)cname, &cm, &cn, ComplexArray,(unsigned long)strlen(cname)) )
		  {
			  fprintf(stderr,"Error in Java_javasci_SciComplexArray_GetRealPartElement (2).\n");
		  }
		  else
		  {
			  for (l=0;l<cm*cn;l++)
			  {
				  cx[l]=ComplexArray[l];
			  }
		  }

		  FREE(ComplexArray);
		  ComplexArray=NULL;
	  }
	  else
	  {
		  fprintf(stderr,"Error in Java_javasci_SciComplexArray_GetRealPartElement (3).\n");
	  }
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
/*! public native double GetImaginaryPartElement(int indr,int indc); */
JNIEXPORT jdouble JNICALL Java_javasci_SciComplexArray_GetImaginaryPartElement(JNIEnv *env , jobject obj_this,jint indrarg, jint indcarg)
{
  int cm,cn,lp;
  const char *cname; 
  double Value=0.0;

  /* get the class */
  jclass class_Mine = (*env)->GetObjectClass(env, obj_this);

  /* get the fields i.e x,m,n,name  */
  jfieldID id_y = (*env)->GetFieldID(env, class_Mine, "y", "[D");
  jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name","Ljava/lang/String;");
  jfieldID id_m = (*env)->GetFieldID(env, class_Mine, "m", "I");
  jfieldID id_n = (*env)->GetFieldID(env, class_Mine, "n", "I");

  /* get the field value */
  jdoubleArray jy = (*env)->GetObjectField(env, obj_this, id_y);
  jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);

  jint jm = (*env)->GetIntField(env, obj_this, id_m);
  jint jn = (*env)->GetIntField(env, obj_this, id_n);
  double *cy = (*env)->GetDoubleArrayElements(env,jy,NULL);
  cname = (*env)->GetStringUTFChars(env, jname, NULL);

  cm=jm;cn=jn;
  if ( ! C2F(cmatcptr)((char *)cname, &cm, &cn, &lp, (unsigned long)strlen(cname))) 
  {
	  fprintf(stderr,"Error in Java_javasci_SciComplexArray_GetImaginaryPartElement (1).\n");
  }
  else
  {
	  double *ComplexArray = NULL;
	  int l=0;
	  ComplexArray = (double*) MALLOC ( sizeof(double)*(jm*jn*2) );
	  if (ComplexArray)
	  {
		  if ( ! C2F(creadcmat)((char *)cname, &cm, &cn, ComplexArray,(unsigned long)strlen(cname)) )
		  {
			  fprintf(stderr,"Error in Java_javasci_SciComplexArray_GetImaginaryPartElement (2).\n");
		  }
		  else
		  {
			  for (l=cm*cn;l<cm*cn*2;l++)
			  {
				  cy[l-cm*cn]=ComplexArray[l];
			  }
		  }

		  FREE(ComplexArray);
		  ComplexArray=NULL;
	  }
	  else
	  {
		  fprintf(stderr,"Error in Java_javasci_SciComplexArray_GetImaginaryPartElement (3).\n");
	  }
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
  
  Value=cy[(indcarg-1)*cm+(indrarg-1)];
  
  return Value;

}
/********************************************************************************************************/
