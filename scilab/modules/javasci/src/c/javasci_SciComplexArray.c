/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * ...
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "javasci_SciComplexArray.h"


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
  jint jn2 = (*env)->GetIntField(env, obj_this, id_n);
  double *cx = (*env)->GetDoubleArrayElements(env,jx,NULL);
  cname = (*env)->GetStringUTFChars(env, jname, NULL);

  cm=jm;
  cn=jn2;

  if ( ! C2F(cmatcptr)((char *)cname, &cm, &cn, &lp, (unsigned long)strlen(cname))) 
  {
	  fprintf(stderr,"Error in Java_javasci_SciComplexArray_GetRealPartElement (1).\n");
  }
  else
  {
	  int l=0;
	  double *ComplexArray = NULL;
	  ComplexArray = (double*) MALLOC ( sizeof(double)*(jm*jn2*2) );
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

  if ( (indrarg>jm) || (indcarg>jn2) )
  {
	  fprintf(stderr,"Error with int indr & int indc.\n");
	  return 0.0;
  }
  Value=cx[(indcarg-1)*cm+(indrarg-1)];
  
  return Value;
}
/******************************************************************************/
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
  jint jn2 = (*env)->GetIntField(env, obj_this, id_n);
  double *cy = (*env)->GetDoubleArrayElements(env,jy,NULL);
  cname = (*env)->GetStringUTFChars(env, jname, NULL);

  cm=jm;
  cn=jn2;

  if ( ! C2F(cmatcptr)((char *)cname, &cm, &cn, &lp, (unsigned long)strlen(cname))) 
  {
	  fprintf(stderr,"Error in Java_javasci_SciComplexArray_GetImaginaryPartElement (1).\n");
  }
  else
  {
	  double *ComplexArray = NULL;
	  int l=0;
	  ComplexArray = (double*) MALLOC ( sizeof(double)*(jm*jn2*2) );
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

  if ( (indrarg>jm) || (indcarg>jn2) )
  {
	  fprintf(stderr,"Error with int indr & int indc.\n");
	  return 0.0;
  }
  
  Value=cy[(indcarg-1)*cm+(indrarg-1)];
  
  return Value;

}
/*****************************************************************************/
