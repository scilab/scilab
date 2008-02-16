/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "javasci_SciBooleanArray.h"


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
  jint jn2 = (*env)->GetIntField(env, obj_this, id_n);
  jboolean *cx = (*env)->GetBooleanArrayElements(env,jx,NULL);
  int *CX = (int*) MALLOC(sizeof(int)*(jm*jn2));

  cname = (*env)->GetStringUTFChars(env, jname, NULL);

  cm=jm;cn=jn2;

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

  if ( (indrarg>jm) || (indcarg>jn2) )
  {
	  fprintf(stderr,"Error with int indr & int indc.\n");
	  return 0;
  }
  
  Value=(int)cx[(indcarg-1)*cm+(indrarg-1)];
  
  return Value;

}
/*****************************************************************************/
