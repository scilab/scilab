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

%define JAVASCI_ARRAYS_IMPL(CTYPE, JNITYPE, JAVATYPE, JAVAPRIMITIVETYPE, JNICODE)

// retrieve from the native code a CTYPE * => JAVAPRIMITIVETYPE[][] (java)
%typemap(out) (CTYPE *) (int nbRow, int nbCol) {

  jclass JAVAPRIMITIVETYPE##Arr = (*jenv)->FindClass(jenv, JNICODE);
  int i = 0, j = 0;
  jresult = (*jenv)->NewObjectArray(jenv, nbRow, ##JAVAPRIMITIVETYPE##Arr, NULL);

    for (i=0; i < nbRow; i++) {
        JNITYPE *array = (JNITYPE*)malloc(nbCol * sizeof(JNITYPE)) ;
        ##JNITYPE##Array jarray = (*jenv)->New##JAVATYPE##Array(jenv, nbCol);
        if (jarray == NULL) {
            printf("Could not allocate\n");fflush(NULL);
        }

        if (array) {
            for (j=0; j < nbCol; j++) {
            /* Scilab is storing matrice cols by cols while Java is doing it
               row by row. Therefor, we need to convert it */
                array[j]=result[nbRow*j+i];
                }
        }

        (*jenv)->Set##JAVATYPE##ArrayRegion(jenv, jarray, 0, nbCol, array);

        (*jenv)->SetObjectArrayElement(jenv, jresult, i, jarray);

        (*jenv)->DeleteLocalRef(jenv, jarray);
        if (array) {
            free(array);
            array = NULL;
        }
    }


     if (arg1) (*jenv)->ReleaseStringUTFChars(jenv, jarg1, (const char *)arg1);
    free(result);

}

%typemap(jni) (CTYPE *) "jobjectArray"
%typemap(jtype) (CTYPE *) "JAVAPRIMITIVETYPE[][]"
%typemap(jstype) (CTYPE *) "JAVAPRIMITIVETYPE[][]"
%typemap(javain) CTYPE *OUTVALUE "$javainput"
%typemap(javaout) (CTYPE *) {
    return $jnicall;
  }


%typemap(javain) CTYPE[ANY], CTYPE[] "$javainput"

%typemap(jtype) (char* variableName, int *nbRow, int *nbCol) "String"
%typemap(jstype) (char* variableName, int *nbRow, int *nbCol) "String"

//////////////////////////

%include "arrays_java.i"

/* Transform the input datatype CTYPE[] to JAVAPRIMITIVETYPE[][] to facilitate the
matching in Java */
%typemap(jni) CTYPE[] "jobjectArray"
%typemap(jtype) CTYPE[] "JAVAPRIMITIVETYPE[][]"
%typemap(jstype) CTYPE[] "JAVAPRIMITIVETYPE[][]"



%typemap(argout) (CTYPE variable[], int nbRow, int nbCol) {
// Specific target because it was freeing the wrong argument
}


%typemap(in) (CTYPE variable[], int nbRow, int nbCol) {
// Convert the CTYPE[][] => CTYPE *
      int i=0, j=0;
      $2 = (*jenv)->GetArrayLength(jenv, $input);
      $3 = 0;

      for(i=0; i<$2; i++) {
          ##JNITYPE## *element = NULL;
          ##JNITYPE##Array oneDim=(##JNITYPE##Array)(*jenv)->GetObjectArrayElement(jenv, jarg2, i);
          if ($3==0) {
              /* First time we are here, init + create the array where we store the data */
              $3 = (*jenv)->GetArrayLength(jenv, oneDim);
              $1 = (CTYPE*)malloc(sizeof(##CTYPE##)*arg3*arg4);
          }
          element=(*jenv)->Get##JAVATYPE##ArrayElements(jenv, oneDim, 0);

          for(j=0; j<$3; j++) {
              $1[j*$2+i]=element[j];
          }
      }

}
%enddef

// See SWIG documentation for the full list:
// http://www.swig.org/Doc1.3/Java.html#default_primitive_type_mappings

// Scilab: int8
JAVASCI_ARRAYS_IMPL(byte, jbyte, Byte, byte, "[B")     /* signed char[] */
JAVASCI_ARRAYS_IMPL(unsigned char, jshort, Short, jshort, "[C") /* unsigned char */

// Scilab: int16

JAVASCI_ARRAYS_IMPL(short, jshort, Short, short, "[S")         /* short[] */
JAVASCI_ARRAYS_IMPL(unsigned short, jchar, Char, short, "[C")   /* unsigned short[] */

// Scilab: int32
JAVASCI_ARRAYS_IMPL(int, jint, Int, int, "[I")                 /* int[] */
JAVASCI_ARRAYS_IMPL(unsigned int, jint, Int, int, "[I")     /* unsigned int[] */

#ifdef __SCILAB_INT64__
// Scilab: int64
JAVASCI_ARRAYS_IMPL(long, jint, int, long, "[J")               /* long[] */
JAVASCI_ARRAYS_IMPL(unsigned long, jlong, Long, long, "[J")   /* unsigned long[] */
#endif

//JAVASCI_ARRAYS_IMPL(float, jfloat, Float, float, )         /* float[] */
// Scilab: double
JAVASCI_ARRAYS_IMPL(double, jdouble, Double, double, "[D")     /* double[] */
// Scilab: boolean
JAVASCI_ARRAYS_IMPL(BOOL, jboolean, Boolean, boolean, "[Z")     /* double[] */
