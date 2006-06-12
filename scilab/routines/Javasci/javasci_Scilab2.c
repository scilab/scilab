/********************************************************************************************************/
/* Arnaud Mangin */
/* INRIA 2006 */
/********************************************************************************************************/
#include "javasci_Scilab2.h"
/********************************************************************************************************/
/* Add on  : Javasci for Pro-Active */
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
    //throws exceptions 
  }
  									
  (*env)->ReleaseStringUTFChars(env, jname , cname);
  (*env)->ReleaseDoubleArrayElements(env, jmatrix, matrix, 0);
}
/********************************************************************************************************/
JNIEXPORT void JNICALL Java_javasci_Scilab_receiveDoubleMatrix (JNIEnv *env, jclass cl, jobject objMatrix)
{
  int i, j;
  const char *cname;
  double *matrix, *tmp;
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
  
  tmp = (double * ) malloc(nbRow * nbCol * sizeof(double));
  if (! C2F(creadmat)((char *)cname, &nbRow , &nbCol, tmp, (unsigned long)strlen(cname) ))
  {
  }
  

  for(i=0; i<nbRow; i++)
  {
    for(j=0; j<nbCol; j++)
	{
      matrix[i*nbCol+j] = tmp[j*nbRow+i];
    }
  }
							
  free(tmp);
  (*env)->ReleaseStringUTFChars(env, jname , cname);
  (*env)->ReleaseDoubleArrayElements(env, jmatrix, matrix, 0);
}
/********************************************************************************************************/
jobject getDoubleMatrix(JNIEnv *env,  jclass cl, jstring name, jint nbRow, jint nbCol)
{
  double *matrix=NULL; 
  const char *cname=NULL;
  
  jclass clMatrix = (*env)->FindClass(env, "javasci/SciDoubleMatrix");
  jmethodID consID = (*env)->GetMethodID(env, clMatrix, "<init>", "(Ljava/lang/String;II)V"); 
  jobject objMatrix =  (*env)->NewObject(env, clMatrix, consID, name, nbRow, nbCol);

  Java_javasci_Scilab_receiveDoubleMatrix(env, cl, objMatrix);
  return objMatrix;
}
/********************************************************************************************************/
JNIEXPORT void JNICALL Java_javasci_Scilab_sendStringMatrix (JNIEnv *env, jclass cl, jobject objMatrix)
{
  const char *cname;
  int nbRow, nbCol;
  int i, j, max=1024;
  jstring jelement;
  const char *element;
  char job[1024*4];
  
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
  int i, j,  r, c, l, max=1024;
  char *element = (char *) malloc(sizeof(char) * max);
  jstring jelement; 
  
  jclass clMatrix = (*env)->GetObjectClass(env, objMatrix);
  
  jfieldID id_matrix =  (*env)->GetFieldID(env, clMatrix, "matrix", "[Ljava/lang/String;" );
  jfieldID id_name =  (*env)->GetFieldID(env, clMatrix, "name", "Ljava/lang/String;");
  jfieldID id_nbRow =  (*env)->GetFieldID(env, clMatrix, "nbRow", "I" );
  jfieldID id_nbCol =  (*env)->GetFieldID(env, clMatrix, "nbCol", "I" );
  
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
      r = i+1;
      c = j+1;
      l = max;
      if (!C2F(creadchains)((char *)cname, &r, &c, &l, element, (unsigned long)strlen(cname), (unsigned long)strlen(element)))
	  {
		//throws exceptions
      }

      jelement = (*env)->NewStringUTF(env, element);
      (*env)->SetObjectArrayElement(env, jmatrix,  i*nbCol + j, jelement);
    }
  }
  
  free(element);
  (*env)->ReleaseStringUTFChars(env, jname , cname);
}
/********************************************************************************************************/
jobject getStringMatrix(JNIEnv *env,  jclass cl, jstring name, jint nbRow, jint nbCol)
{
  double *matrix=NULL; 
  const char *cname=NULL;
  
  jclass clMatrix = (*env)->FindClass(env, "javasci/SciStringMatrix");
  jmethodID consID = (*env)->GetMethodID(env, clMatrix, "<init>", "(Ljava/lang/String;II)V"); 
  jobject objMatrix =  (*env)->NewObject(env, clMatrix, consID, name, nbRow, nbCol);

  Java_javasci_Scilab_receiveStringMatrix(env, cl, objMatrix);
  return objMatrix;
}
/********************************************************************************************************/
JNIEXPORT jobject JNICALL Java_javasci_Scilab_receiveDataByName (JNIEnv *env, jclass cl, jstring name)
{
  const char *cname=NULL;
  int *header=NULL; 
  int type = 0;
  jobject obj=NULL;

  cname = (*env)->GetStringUTFChars(env, name, NULL); 

  if((header = (int *)GetDataFromName((char *)cname)) == NULL)
  {
    // throws exception
  }

  (*env)->ReleaseStringUTFChars(env, name , cname);
  
  type = header[0]; //give type 

  switch(type)
  {
	case sci_matrix : 
		return getDoubleMatrix(env, cl, name, header[1], header[2]);
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
