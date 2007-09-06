/********************************************************************************************************/
/* Arnaud Mangin - INRIA 2006 */
/* Fabien Viale - INRIA 2007 */
/********************************************************************************************************/
#include "javasci_Scilab2.h"
/********************************************************************************************************/
/**
* Add on  : Javasci for Pro-Active 
*/
/********************************************************************************************************/
#define MAX_String 1024
/********************************************************************************************************/
JNIEXPORT void JNICALL Java_javasci_Scilab_initialize(JNIEnv *env, jclass cl)
{
	if ( GetInterfState() == 0) { EnableInterf(); Initialize();} 
}
/********************************************************************************************************/
JNIEXPORT void JNICALL Java_javasci_Scilab_sendDoubleMatrix (JNIEnv *env, jclass cl, jobject objMatrix)
{
	const char *cname;
	double *matrix;
	int nbRow, nbCol;

	jclass clMatrix = (*env)->GetObjectClass(env, objMatrix);

	jfieldID id_matrix =  (*env)->GetFieldID(env, clMatrix, "matrix", "[D" );
	jfieldID id_name =  (*env)->GetFieldID(env, clMatrix, "name", "Ljava/lang/String;");
	jfieldID id_nbRow =  (*env)->GetFieldID(env, clMatrix, "nbRow", "I" );
	jfieldID id_nbCol =  (*env)->GetFieldID(env, clMatrix, "nbCol", "I" );

	jdoubleArray jmatrix = (*env)->GetObjectField(env, objMatrix, id_matrix);
	jstring jname = (jstring) (*env)->GetObjectField(env, objMatrix, id_name);
	jint jnbRow = (*env)->GetIntField(env, objMatrix, id_nbRow);
	jint jnbCol = (*env)->GetIntField(env, objMatrix, id_nbCol);

	nbRow = jnbRow;
	nbCol = jnbCol;


	cname = (*env)->GetStringUTFChars(env, jname, NULL); 
	matrix = (*env)->GetDoubleArrayElements(env, jmatrix, NULL);

	if (! C2F(cwritemat)((char *)cname, &nbRow , &nbCol, matrix, (unsigned long)strlen(cname)))
	{
		/* throws exceptions */
	}

	(*env)->ReleaseStringUTFChars(env, jname , cname);
	(*env)->ReleaseDoubleArrayElements(env, jmatrix, matrix, 0);
}
/********************************************************************************************************/
JNIEXPORT void JNICALL Java_javasci_Scilab_receiveDoubleMatrix (JNIEnv *env, jclass cl, jobject objMatrix)
{
	const char *cname=NULL;
	double *matrix=NULL, *tmp=NULL;
	int nbRow, nbCol;
	int i, j;

	jclass clMatrix = (*env)->GetObjectClass(env, objMatrix);

	jfieldID id_matrix =  (*env)->GetFieldID(env, clMatrix, "matrix", "[D" );
	jfieldID id_name =  (*env)->GetFieldID(env, clMatrix, "name", "Ljava/lang/String;");
	jfieldID id_nbRow =  (*env)->GetFieldID(env, clMatrix, "nbRow", "I" );
	jfieldID id_nbCol =  (*env)->GetFieldID(env, clMatrix, "nbCol", "I" );

	jdoubleArray jmatrix = (*env)->GetObjectField(env, objMatrix, id_matrix);
	jstring jname = (jstring) (*env)->GetObjectField(env, objMatrix, id_name);
	jint jnbRow = (*env)->GetIntField(env, objMatrix, id_nbRow);
	jint jnbCol = (*env)->GetIntField(env, objMatrix, id_nbCol);

	nbRow = jnbRow;
	nbCol = jnbCol;


	cname = (*env)->GetStringUTFChars(env, jname, NULL); 
	matrix = (*env)->GetDoubleArrayElements(env, jmatrix, NULL);

	tmp = (double * ) MALLOC(nbRow * nbCol * sizeof(double));
	if (! C2F(creadmat)((char *)cname, &nbRow , &nbCol, tmp, (unsigned long)strlen(cname) ))
	{
		/* throws exceptions */
	}


	for(i=0; i<nbRow; i++)
	{
		for(j=0; j<nbCol; j++)
		{
			matrix[i*nbCol+j] = tmp[j*nbRow+i];
		}
	}

	FREE(tmp);
	(*env)->ReleaseStringUTFChars(env, jname , cname);
	(*env)->ReleaseDoubleArrayElements(env, jmatrix, matrix, 0);
}
/********************************************************************************************************/
JNIEXPORT void JNICALL Java_javasci_Scilab_sendComplexMatrix (JNIEnv *env, jclass cl, jobject objMatrix)
{
	const char *cname;
	double *cx,*cy;
	double *ComplexArray = NULL;
	int nbRow, nbCol;

	jclass clMatrix = (*env)->GetObjectClass(env, objMatrix);

	jfieldID id_x =  (*env)->GetFieldID(env, clMatrix, "x", "[D" );
	jfieldID id_y =  (*env)->GetFieldID(env, clMatrix, "y", "[D" );
	jfieldID id_name =  (*env)->GetFieldID(env, clMatrix, "name", "Ljava/lang/String;");
	jfieldID id_nbRow =  (*env)->GetFieldID(env, clMatrix, "nbRow", "I" );
	jfieldID id_nbCol =  (*env)->GetFieldID(env, clMatrix, "nbCol", "I" );

	jdoubleArray jx = (*env)->GetObjectField(env, objMatrix, id_x);
	jdoubleArray jy = (*env)->GetObjectField(env, objMatrix, id_y);
	jstring jname = (jstring) (*env)->GetObjectField(env, objMatrix, id_name);
	jint jnbRow = (*env)->GetIntField(env, objMatrix, id_nbRow);
	jint jnbCol = (*env)->GetIntField(env, objMatrix, id_nbCol);

	nbRow = jnbRow;
	nbCol = jnbCol;


	cname = (*env)->GetStringUTFChars(env, jname, NULL); 
	cx = (*env)->GetDoubleArrayElements(env, jx, NULL);
	cy = (*env)->GetDoubleArrayElements(env, jy, NULL);

	ComplexArray = (double*) MALLOC ( sizeof(double)*(jnbRow*jnbCol*2) );
	if (ComplexArray)
	{
		int l=0;
		for (l=0;l<nbRow*nbCol;l++)
		{
			ComplexArray[l]=cx[l];
		}


		for (l=nbRow*nbCol;l<nbRow*nbCol*2;l++)
		{
			ComplexArray[l]=cy[l-nbRow*nbCol];
		}


		if (!C2F(cwritecmat)((char *)cname,&nbRow,&nbCol,ComplexArray,(unsigned long)strlen(cname)))
		{
			fprintf(stderr,"Error in Java_javasci_Scilab_sendComplexMatrix (1).\n");
		}
		FREE(ComplexArray);
		ComplexArray = NULL ;
	}
	else
	{
		fprintf(stderr,"Error in Java_javasci_Scilab_sendComplexMatrix (2).\n");
	}

	(*env)->ReleaseStringUTFChars(env, jname , cname);
	(*env)->ReleaseDoubleArrayElements(env,jx,cx,0);
	(*env)->ReleaseDoubleArrayElements(env,jy,cy,0);
}
/********************************************************************************************************/
JNIEXPORT void JNICALL Java_javasci_Scilab_receiveComplexMatrix (JNIEnv *env, jclass cl, jobject objMatrix)
{
	const char *cname=NULL;
	double *cx=NULL, *cy=NULL;
	int nbRow = 0, nbCol = 0, lp = 0;

	jclass clMatrix = (*env)->GetObjectClass(env, objMatrix);

	jfieldID id_x =  (*env)->GetFieldID(env, clMatrix, "x", "[D" );
	jfieldID id_y =  (*env)->GetFieldID(env, clMatrix, "y", "[D" );
	jfieldID id_name =  (*env)->GetFieldID(env, clMatrix, "name", "Ljava/lang/String;");
	jfieldID id_nbRow =  (*env)->GetFieldID(env, clMatrix, "nbRow", "I" );
	jfieldID id_nbCol =  (*env)->GetFieldID(env, clMatrix, "nbCol", "I" );

	jdoubleArray jx = (*env)->GetObjectField(env, objMatrix, id_x);
	jdoubleArray jy = (*env)->GetObjectField(env, objMatrix, id_y);
	jstring jname = (jstring) (*env)->GetObjectField(env, objMatrix, id_name);
	jint jnbRow = (*env)->GetIntField(env, objMatrix, id_nbRow);
	jint jnbCol = (*env)->GetIntField(env, objMatrix, id_nbCol);

	nbRow = jnbRow;
	nbCol = jnbCol;


	cname = (*env)->GetStringUTFChars(env, jname, NULL); 
	cx = (*env)->GetDoubleArrayElements(env, jx, NULL);
	cy = (*env)->GetDoubleArrayElements(env, jy, NULL);


	if ( ! C2F(cmatcptr)((char *)cname, &nbRow, &nbCol, &lp, (unsigned long)strlen(cname))) 
	{
		fprintf(stderr,"Error in Java_javasci_Scilab_receiveComplexMatrix (1).\n");
	}
	else
	{
		double *ComplexArray = NULL;
		int l=0;
		ComplexArray = (double*) MALLOC ( sizeof(double)*(jnbRow*jnbCol*2) );
		if (ComplexArray)
		{
			if ( ! C2F(creadcmat)((char *)cname, &nbRow, &nbCol, ComplexArray,(unsigned long)strlen(cname)) )
			{
				fprintf(stderr,"Error in Java_javasci_Scilab_receiveComplexMatrix (2).\n");
			}
			else
			{
				for (l=0;l<nbRow*nbCol;l++)
				{
					cx[l]=ComplexArray[l];
				}


				for (l=nbRow*nbCol;l<nbRow*nbCol*2;l++)
				{
					cy[l-nbRow*nbCol]=ComplexArray[l];
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
JNIEXPORT void JNICALL Java_javasci_Scilab_sendStringMatrix (JNIEnv *env, jclass cl, jobject objMatrix)
{
#define MAX_String 1024
	const char *cname;
	int nbRow, nbCol;
	int i, j, max=MAX_String;
	jstring jelement;
	const char *element;
	char job[MAX_String*4];

	jclass clMatrix = (*env)->GetObjectClass(env, objMatrix);

	jfieldID id_matrix = (*env)->GetFieldID(env, clMatrix, "matrix", "[Ljava/lang/String;" );
	jfieldID id_name =  (*env)->GetFieldID(env, clMatrix, "name", "Ljava/lang/String;");
	jfieldID id_nbRow = (*env)->GetFieldID(env, clMatrix, "nbRow", "I" );
	jfieldID id_nbCol = (*env)->GetFieldID(env, clMatrix, "nbCol", "I" );


	jobjectArray jmatrix = (*env)->GetObjectField(env, objMatrix, id_matrix);
	jstring jname = (jstring) (*env)->GetObjectField(env, objMatrix, id_name);
	jint jnbRow = (*env)->GetIntField(env, objMatrix, id_nbRow);
	jint jnbCol = (*env)->GetIntField(env, objMatrix, id_nbCol);

	nbRow = jnbRow;
	nbCol = jnbCol;


	cname = (*env)->GetStringUTFChars(env, jname, NULL);

	for(i=0; i<nbRow; i++)
	{
		for(j=0; j<nbCol; j++)
		{
			jelement = (jstring)(*env)->GetObjectArrayElement(env, jmatrix, i*nbCol + j);
			element = (*env)->GetStringUTFChars(env, jelement, NULL);

			sprintf(job,"%s(%d,%d)=\"\"%s\"\";",cname,i+1,j+1,element);

			if (send_scilab_job(job))
			{
				/* throws exceptions */
			}

			(*env)->ReleaseStringUTFChars(env, jelement,  element);
		}
	}


	(*env)->ReleaseStringUTFChars(env, jname , cname);
}
/********************************************************************************************************/
JNIEXPORT void JNICALL Java_javasci_Scilab_receiveStringMatrix (JNIEnv *env, jclass cl, jobject objMatrix)
{
	const char *cname;
	int nbRow, nbCol;
	int i, j,  r, c, l, max=MAX_String;
	char *element = (char *) MALLOC(sizeof(char) * max);

	jclass clMatrix = (*env)->GetObjectClass(env, objMatrix);

	jfieldID id_matrix =  (*env)->GetFieldID(env, clMatrix, "matrix", "[Ljava/lang/String;" );
	jfieldID id_name =  (*env)->GetFieldID(env, clMatrix, "name", "Ljava/lang/String;");
	jfieldID id_nbRow =  (*env)->GetFieldID(env, clMatrix, "nbRow", "I" );
	jfieldID id_nbCol =  (*env)->GetFieldID(env, clMatrix, "nbCol", "I" );

	jobjectArray jmatrix = (*env)->GetObjectField(env, objMatrix, id_matrix);
	jstring jname = (jstring) (*env)->GetObjectField(env, objMatrix, id_name);
	jint jnbRow = (*env)->GetIntField(env, objMatrix, id_nbRow);
	jint jnbCol = (*env)->GetIntField(env, objMatrix, id_nbCol);


	jstring jelement; 

	nbRow = jnbRow;
	nbCol = jnbCol;


	cname = (*env)->GetStringUTFChars(env, jname, NULL);

	for(i=0; i<nbRow; i++)
	{
		for(j=0; j<nbCol; j++)
		{
			r = i+1;
			c = j+1;
			l = max;

			if (!C2F(creadchains)((char *)cname, &r, &c, &l, element, (unsigned long)strlen(cname), (unsigned long)strlen(element)))
			{
				/* throws exceptions */
			}


			jelement = (*env)->NewStringUTF(env, element);
			(*env)->SetObjectArrayElement(env, jmatrix,  i*nbCol + j, jelement);
		}
	}

	FREE(element);
	(*env)->ReleaseStringUTFChars(env, jname , cname);
}
/********************************************************************************************************/
jobject getDoubleMatrix(JNIEnv *env,  jclass cl, jstring name, jint nbRow, jint nbCol)
{
	jclass clMatrix = (*env)->FindClass(env, "javasci/SciDoubleMatrix");
	jmethodID consID = (*env)->GetMethodID(env, clMatrix, "<init>", "(Ljava/lang/String;II)V"); 
	jobject objMatrix =  (*env)->NewObject(env, clMatrix, consID, name, nbRow, nbCol);

	Java_javasci_Scilab_receiveDoubleMatrix(env, cl, objMatrix);
	return objMatrix;
}
/********************************************************************************************************/
jobject getComplexMatrix(JNIEnv *env,  jclass cl, jstring name, jint nbRow, jint nbCol)
{
	jclass clMatrix = (*env)->FindClass(env, "javasci/SciComplexMatrix");
	jmethodID consID = (*env)->GetMethodID(env, clMatrix, "<init>", "(Ljava/lang/String;II)V"); 
	jobject objMatrix =  (*env)->NewObject(env, clMatrix, consID, name, nbRow, nbCol);

	Java_javasci_Scilab_receiveComplexMatrix(env, cl, objMatrix);
	return objMatrix;
}
/********************************************************************************************************/
jobject getStringMatrix(JNIEnv *env,  jclass cl, jstring name, jint nbRow, jint nbCol)
{
	jclass clMatrix = (*env)->FindClass(env, "javasci/SciStringMatrix");
	jmethodID consID = (*env)->GetMethodID(env, clMatrix, "<init>", "(Ljava/lang/String;II)V"); 
	jobject objMatrix =  (*env)->NewObject(env, clMatrix, consID, name, nbRow, nbCol);

	Java_javasci_Scilab_receiveStringMatrix(env, cl, objMatrix);
	return objMatrix;
}
/********************************************************************************************************/
JNIEXPORT jobject JNICALL Java_javasci_Scilab_receiveDataByName (JNIEnv *env, jclass cl, jstring name)
{
#define COMPLEX 1
	const char *cname;
	int *header; 
	jobject obj=NULL;
	int type = 0;


	cname = (*env)->GetStringUTFChars(env, name, NULL); 


	if((header = (int *)GetDataFromName((char *)cname)) == NULL)
	{
		/* throws exception */
	}


	(*env)->ReleaseStringUTFChars(env, name , cname);

	type = header[0]; /* give type */


	switch(type)
	{
	case sci_matrix : 
		if (header[3] == COMPLEX) {
			return getComplexMatrix(env, cl, name, header[1], header[2]);
		}
		else {
			return getDoubleMatrix(env, cl, name, header[1], header[2]);
		}
	case sci_poly :
		break;
	case sci_boolean :
		break;
	case sci_ints :
		break; 
	case sci_strings :
		return getStringMatrix(env, cl, name, header[1], header[2]);
		break;
	case sci_list :
		break;
	default :
		break;
	}
	return obj;
}
/********************************************************************************************************/

