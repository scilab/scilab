/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "javasci_SciAbstractArray.h"
#include "javasci_globals.h"
#include "CallScilab.h"

char* detectSignatureTypeFromObjectName(JNIEnv *env, jobject obj_this);

/*
 * Class:     javasci_SciAbstractArray
 * Method:    Initialize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_javasci_SciAbstractArray_Initialize (JNIEnv *env, jclass cl)
{
	if ( GetInterfState() == 0) { EnableInterf(); Initialize();} 
}


/*
 * Class:     javasci_SciAbstractArray
 * Method:    getNumberOfRowsFromScilab
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_javasci_SciAbstractArray_getNumberOfRowsFromScilab(JNIEnv *env, jobject obj_this, jstring name){
	int lw; int fin;
	const char *cname=NULL;
	jint row=-1;

	/* get the field value */
	cname = (*env)->GetStringUTFChars(env, name, NULL);
	if (C2F(objptr)((char*)cname,&lw,&fin,(unsigned long)strlen(cname)))
	{
		#define SCIBOOLEANTYPE 4
		#define SCIMATRIXREALORCOMPLEXTYPE 1
		#define COMPLEX 1
		#define SCISTRINGTYPE 10

		int *header=NULL; 
		header = (int *)GetDataFromName((char *)cname);

		if (header[0] == SCIBOOLEANTYPE || header[0] == SCISTRINGTYPE)
		{
			row = header[1];
		}

		if ( header[0] == SCIMATRIXREALORCOMPLEXTYPE )
		{
			int Cmplx=header[3];

			if (Cmplx == COMPLEX)
			{
				row = header[1];
			}
		}
	}

	(*env)->ReleaseStringUTFChars(env, name , cname);

	return row;
}

/*
 * Class:     javasci_SciAbstractArray
 * Method:    getColFromScilab
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_javasci_SciAbstractArray_getNumberOfColsFromScilab(JNIEnv *env, jobject obj_this, jstring name){
	int lw; int fin;
	const char *cname=NULL;
	jint col=-1;

	/* get the field value */
	cname = (*env)->GetStringUTFChars(env, name, NULL);

	if (C2F(objptr)((char*)cname,&lw,&fin,(unsigned long)strlen(cname)))
	{
		#define SCIBOOLEANTYPE 4
		#define SCISTRINGTYPE 10
		#define SCIMATRIXREALORCOMPLEXTYPE 1
		#define COMPLEX 1

		int *header=NULL; 
		header = (int *)GetDataFromName((char *)cname);
		if (header[0] == SCIBOOLEANTYPE || header[0] == SCISTRINGTYPE)
		{
			col = header[2];
		}

		if ( header[0] == SCIMATRIXREALORCOMPLEXTYPE )
		{
			int Cmplx=header[3];

			if (Cmplx == COMPLEX)
			{
				col = header[2];
			}
		}
	}

	(*env)->ReleaseStringUTFChars(env, name , cname);

	return col;

}


/*
 * Class:     javasci_SciAbstractArray
 * Method:    Job
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_javasci_SciAbstractArray_Job(JNIEnv *env, jobject obj_this, jstring job){
	const char *cjob = NULL;
	jboolean bOK=0;

	/* get the field value */
	cjob = (*env)->GetStringUTFChars(env, job, NULL);

	if (strlen(cjob) >= MAX_STR)
	{
		fprintf(stderr,"Error in Java_javasci_SciAsbtractArray_Job routine (line too long).\n");
		bOK=0;
	}
	else
	{
		if (SendScilabJob((char*)cjob))
		{
			fprintf(stderr,"Error in Java_javasci_SciAsbtractArray_Job routine.\n");
			bOK=0;
		}
		else bOK=1;
	}

	(*env)->ReleaseStringUTFChars(env, job , cjob);

	return bOK;
}

/*
 * Class:     javasci_SciAbstractArray
 * Method:    Get
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_javasci_SciAbstractArray_Get(JNIEnv *env, jobject obj_this){

  int cm=0,cn=0,lp=0;
  const char *cname=NULL; 
  char *signatureType=detectSignatureTypeFromObjectName(env, obj_this);
  jfieldID id_x;
  jfieldID id_name;
  jfieldID id_m;
  jfieldID id_n;
  jstring jname;
  jint jm;
  jint jn2;

  /* get the class */
  jclass class_Mine = (*env)->GetObjectClass(env, obj_this);

  /* get the fields i.e x,m,n,name  */
  char *signatureType2=MALLOC(sizeof(char)*4);
  strcpy(signatureType2,"[");
  strcat(signatureType2, signatureType);
  if (strcmp(signatureType,"CO")==0)
  { // Complex
	  id_x = (*env)->GetFieldID(env, class_Mine, "x", "[D");
  }
  else
  {
	  id_x = (*env)->GetFieldID(env, class_Mine, "x", signatureType2);
  }
  id_name =  (*env)->GetFieldID(env, class_Mine, "name","Ljava/lang/String;");
  id_m = (*env)->GetFieldID(env, class_Mine, "m", "I");
  id_n = (*env)->GetFieldID(env, class_Mine, "n", "I");


  jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);
  jm = (*env)->GetIntField(env, obj_this, id_m);
  jn2 = (*env)->GetIntField(env, obj_this, id_n);
  cname = (*env)->GetStringUTFChars(env, jname, NULL);

  cm=jm;cn=jn2;



  // The class of object is SciComplexArray
  if (strcmp(signatureType,"CO")==0){

	  jfieldID id_y = (*env)->GetFieldID(env, class_Mine, "y", "[D");

	  /* get the field value */
	  jdoubleArray jx = (*env)->GetObjectField(env, obj_this, id_x);
	  jdoubleArray jy = (*env)->GetObjectField(env, obj_this, id_y);

	  double *cx = (*env)->GetDoubleArrayElements(env,jx,NULL);
	  double *cy = (*env)->GetDoubleArrayElements(env,jy,NULL);


	  if ( ! C2F(cmatcptr)((char *)cname, &cm, &cn, &lp, (unsigned long)strlen(cname))) 
		  {
			  fprintf(stderr,"Error in Java_javasci_SciComplexArray_Get (1).\n");
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
	  (*env)->ReleaseDoubleArrayElements(env,jx,cx,0);
	  (*env)->ReleaseDoubleArrayElements(env,jy,cy,0);
	  
  }
  // The class of object is SciDoubleArray
  if (strcmp(signatureType,"D")==0){
	  // double
	  jdoubleArray jx = (*env)->GetObjectField(env, obj_this, id_x);
	  double *cx = (*env)->GetDoubleArrayElements(env,jx,NULL);
	  if ( ! C2F(cmatptr)((char *)cname, &cm, &cn, &lp,(unsigned long)strlen(cname))) 
		  {
			  fprintf(stderr,"Error in Java_javasci_SciDoubleArray_Get (1).\n");
		  }
	  else 
		  if (! C2F(creadmat)((char *)cname,&cm, &cn,cx,(unsigned long)strlen(cname) ))
			  {	
				  fprintf(stderr,"Error in Java_javasci_SciDoubleArray_Get (2).\n");
			  }
	
	  (*env)->ReleaseDoubleArrayElements(env,jx,cx,0);


  }

  // The class of object is SciBooleanArray
  if (strcmp(signatureType,"Z")==0){
	  jbooleanArray jx = (*env)->GetObjectField(env, obj_this, id_x);
	  jboolean *cx = (*env)->GetBooleanArrayElements(env,jx,NULL);
	  int i=0;

	  int *CX = (int*) MALLOC(sizeof(int)*(jm*jn2));
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
	  (*env)->ReleaseBooleanArrayElements(env,jx,cx,0);
	  if (CX) {FREE(CX);CX=NULL;}

  }

}




/**
 * Detect which Scilab object we are dealing with
 * might need some cleaning
 * @return Scilab Type of the object we are dealing with
 */
char* detectSignatureTypeFromObjectName(JNIEnv *env, jobject obj_this){

	char *signatureType=MALLOC(sizeof(char)*3);
	
	if((*env)->IsInstanceOf(env, obj_this,  (*env)->FindClass(env,"javasci/SciDoubleArray")) == JNI_TRUE){
		return "D";
	}

	if((*env)->IsInstanceOf(env, obj_this,  (*env)->FindClass(env,"javasci/SciBooleanArray")) == JNI_TRUE){
		return "Z";
	}

	if((*env)->IsInstanceOf(env, obj_this,  (*env)->FindClass(env,"javasci/SciComplexArray")) == JNI_TRUE){
		return "CO";
	}

	return signatureType;

}

/*
 * Class:     javasci_SciAbstractArray
 * Method:    Send
 * Signature: ()V
 * String is not managed as it is done by SendString
 * @see #Java_javasci_SciStringArray_SendString(JNIEnv *env , jobject obj_this,jstring strarg,jint indxarg, jint indyarg)
 */
JNIEXPORT void JNICALL Java_javasci_SciAbstractArray_Send(JNIEnv *env, jobject obj_this){

  int cm,cn;
  char *signatureType=detectSignatureTypeFromObjectName(env, obj_this);
  const char *cname; 
  jfieldID id_x;

  /* get the class */
  jclass class_Mine = (*env)->GetObjectClass(env, obj_this);

  jfieldID id_name;
  jfieldID id_m;
  jfieldID id_n;

  jstring jname;
  jint jm;
  jint jn2;
	
  /* get the fields i.e x,m,n,name  */
  char signatureType2[4];
  signatureType2[0]='[';
  signatureType2[1]='\0';
  strcat(signatureType2, signatureType);
  

  if (strcmp(signatureType,"CO")==0){ // Complex
	  id_x = (*env)->GetFieldID(env, class_Mine, "x", "[D");
  }else{
	  id_x = (*env)->GetFieldID(env, class_Mine, "x", signatureType2);
  }
  id_name =  (*env)->GetFieldID(env, class_Mine, "name","Ljava/lang/String;");
  id_m = (*env)->GetFieldID(env, class_Mine, "m", "I");
  id_n = (*env)->GetFieldID(env, class_Mine, "n", "I");


  /* get the field value */
  jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);
  jm = (*env)->GetIntField(env, obj_this, id_m);
  jn2= (*env)->GetIntField(env, obj_this, id_n);

  cname = (*env)->GetStringUTFChars(env, jname, NULL);
  cm=jm;cn=jn2;


  // Core part of the function

  // The class of object is SciComplexArray
  if (strcmp(signatureType,"CO")==0){
	  double *ComplexArray = NULL;

	  jfieldID id_y = (*env)->GetFieldID(env, class_Mine, "y", "[D");
	  /* get the field value */
	  jdoubleArray jx = (*env)->GetObjectField(env, obj_this, id_x);
	  jdoubleArray jy = (*env)->GetObjectField(env, obj_this, id_y);
	  double *cx = (*env)->GetDoubleArrayElements(env,jx,NULL);
	  double *cy = (*env)->GetDoubleArrayElements(env,jy,NULL);

	  ComplexArray = (double*) MALLOC ( sizeof(double)*(jm*jn2*2) );
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
	  (*env)->ReleaseDoubleArrayElements(env,jx,cx,0);
	  (*env)->ReleaseDoubleArrayElements(env,jy,cy,0);
  }

  // The class of object is SciDoubleArray
  if (strcmp(signatureType,"D")==0){
	  jdoubleArray jx = (*env)->GetObjectField(env, obj_this, id_x);
	  double *cx = (*env)->GetDoubleArrayElements(env,jx,NULL);

	  if (!C2F(cwritemat)((char *)cname,&cm,&cn,cx,(unsigned long)strlen(cname))) 
		  {
			  fprintf(stderr,"Error in Java_javasci_SciDoubleArray_Send.\n");
		  }
	  (*env)->ReleaseDoubleArrayElements(env,jx,cx,0);
  }


	  // The class of object is SciBooleanArray
  if (strcmp(signatureType,"Z")==0){
	  jbooleanArray jx = (*env)->GetObjectField(env, obj_this, id_x);
	  int *CX = (int*) MALLOC(sizeof(int)*(jm*jn2));
	  jboolean *cx = (*env)->GetBooleanArrayElements(env,jx,NULL);

	  int i=0;

	  for(i=0;i<cm*cn;i++)
		  {
			  CX[i]=(int)cx[i];
		  }
	  if (!C2F(cwritebmat)((char *)cname,&cm,&cn,CX,(unsigned long)strlen(cname))) 
		  {
			  fprintf(stderr,"Error in Java_javasci_SciBooleanArray_Send.\n");
		  }

	  if (CX){FREE(CX);CX=NULL;}
	  (*env)->ReleaseBooleanArrayElements(env,jx,(jboolean*)cx,0);
  }


  // Common cleanning
  (*env)->ReleaseStringUTFChars(env, jname , cname);


}
