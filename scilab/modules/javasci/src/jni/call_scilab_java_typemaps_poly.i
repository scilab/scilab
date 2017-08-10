/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */


//////////////////////////////////////////////



//////////////////////////////////////////////

%define JAVASCI_POLY_ARRAYS_IMPL(CTYPE, JNITYPE, JAVATYPE, JAVAPRIMITIVETYPE)

%typemap(jni) (CTYPE **) "jobjectArray"
%typemap(jtype) (CTYPE **) "JAVAPRIMITIVETYPE[][][]"
%typemap(jstype) (CTYPE **) "JAVAPRIMITIVETYPE[][][]"
%typemap(javain) (CTYPE **) "$javainput"
%typemap(javaout) CTYPE** {
    return $jnicall;
}

//////////////////////////

%typemap(argout) (CTYPE ** data, int nbRow, int nbCol, int * nbCoef) {
// Specific target because it was freeing the wrong argument
    int i = 0;
    for (; i < $2 * $3; i++)
    {
	FREE($1[i]);
    }
    FREE($1);
    FREE($4);
}


%typemap(in) (CTYPE ** data, int nbRow, int nbCol, int * nbCoef) {
// Convert the CTYPE[][] => CTYPE *
    int i = 0, j = 0, k = 0;
      $2 = (*jenv)->GetArrayLength(jenv, $input);
      $3 = 0;
      $1 = NULL;

      for (; i < $2; i++) {
          jobjectArray dblDim = (jobjectArray)(*jenv)->GetObjectArrayElement(jenv, $input, i);
          if ($3 == 0) {
              /* First time we are here, init + create the array where we store the data */
              $3 = (*jenv)->GetArrayLength(jenv, dblDim);
              $1 = (CTYPE**)malloc(sizeof(##CTYPE##*) * $2 * $3);
	      $4 = (int*)malloc(sizeof(int) * $2 * $3);
          }
          for (j = 0; j < $3; j++)
	  {
	      ##JNITYPE##Array oneDim = (##JNITYPE##Array)(*jenv)->GetObjectArrayElement(jenv, dblDim, j);
	      jboolean isCopy = JNI_FALSE;
	      ##JNITYPE##* element = NULL;
	      $4[j * $2 + i] = (*jenv)->GetArrayLength(jenv, oneDim);
	      $1[j * $2 + i] = (##CTYPE##*)MALLOC(sizeof(##JNITYPE##) * $4[j * $2 + i]);
	      isCopy = JNI_FALSE;
	      element = (##JNITYPE##*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDim, &isCopy);
	      memcpy($1[j * $2 + i], element, sizeof(##JNITYPE##) * $4[j * $2 + i]);
	      (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDim, element, 0);
	      (*jenv)->DeleteLocalRef(jenv, oneDim);
          }
	  (*jenv)->DeleteLocalRef(jenv, dblDim);
      }
 }

%typemap(argout) (CTYPE ** imag, int nbRowI, int nbColI, int * nbCoefI) = (CTYPE ** data, int nbRow, int nbCol, int * nbCoef);
%typemap(in) (CTYPE ** imag, int nbRowI, int nbColI, int * nbCoefI) = (CTYPE ** data, int nbRow, int nbCol, int * nbCoef);

%enddef

JAVASCI_POLY_ARRAYS_IMPL(double, jdouble, Double, double)     /* double[] */

