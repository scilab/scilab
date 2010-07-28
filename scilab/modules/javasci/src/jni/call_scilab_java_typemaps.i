/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


//////////////////////////////////////////////



//////////////////////////////////////////////

%typemap(in) (char* variableName, int *nbRow, int *nbCol)  {
    $2 = &nbRow;
    $3 = &nbCol;
    $1 = 0;
    if ($input) {
       $1 = (char *)(*jenv)->GetStringUTFChars(jenv, $input, 0);
       if (!$1) return 0;
    }
}

// retrieve from the native code a double * => double[][] (java)
%typemap(out) (double *) (int nbRow, int nbCol) {
  
  jclass dblArr = (*jenv)->FindClass(jenv, "[D");
   int i,j;
    jresult = (*jenv)->NewObjectArray(jenv, nbRow, dblArr, NULL);

    for (i=0; i < nbRow; i++) {
        double array[nbCol];
        jdoubleArray jarray = (*jenv)->NewDoubleArray(jenv, nbCol);
        if (jarray == NULL) {
            printf("Could not allocate\n");fflush(NULL);
        }

        for (j=0; j < nbCol; j++) { 
            /* Scilab is storing matrice cols by cols while Java is doing it
               row by row. Therefor, we need to convert it */
            array[j]=result[nbRow*j+i];
        }
        
        (*jenv)->SetDoubleArrayRegion(jenv, jarray, 0, nbCol, array);

        (*jenv)->SetObjectArrayElement(jenv, jresult, i, jarray);

        (*jenv)->DeleteLocalRef(jenv, jarray);
      
    }
    

    if (arg1) (*jenv)->ReleaseStringUTFChars(jenv, jarg1, (const char *)arg1);
    free(result);

}

%typemap(jni) (double *) "jobjectArray"
%typemap(jtype) (double *) "double[][]"
%typemap(jstype) (double *) "double[][]"
%typemap(javain) double *OUTVALUE "$javainput"
%typemap(javaout) (double *) {
    return $jnicall;
  }

%typemap(jtype) (char* variableName, int *nbRow, int *nbCol) "String"
%typemap(jstype) (char* variableName, int *nbRow, int *nbCol) "String"

//////////////////////////

%include "arrays_java.i"

/* Transform the input datatype double[] to double[][] to facilitate the 
matching in Java */
%typemap(jni) double[] "jobjectArray"
%typemap(jtype) double[] "double[][]"
%typemap(jstype) double[] "double[][]"

%typemap(in) (double variable[], int nbRow, int nbCol) {
// Convert the double[][] => double *
      $2 = (*jenv)->GetArrayLength(jenv, $input);
   	  $3 = 0;
	  int i=0, j=0;
	  for(i=0; i<$2; i++) {
		  jdoubleArray oneDim=(jdoubleArray)(*jenv)->GetObjectArrayElement(jenv, jarg2, i);
		  if ($3==0) {
			  /* First time we are here, init + create the array where we store the data */
			  $3 = (*jenv)->GetArrayLength(jenv, oneDim);
			  $1 = (double*)malloc(sizeof(double)*arg3*arg4);
		  }
		  jdouble *element=(*jenv)->GetDoubleArrayElements(jenv, oneDim, 0);

		  for(j=0; j<$3; j++) {
			  $1[j*$2+i]=element[j];
		  }
	  }

}


%typemap(argout) (double variable[], int nbRow, int nbCol) {
// Specific target because it was freeing the wrong argument
}
