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

%define JAVASCI_SPARSE_ARRAYS_IMPL(CTYPE, JNITYPE, JAVATYPE, JAVAPRIMITIVETYPE)

%typemap(jni) (CTYPE *) "jobjectArray"
%typemap(jtype) (CTYPE *) "JAVAPRIMITIVETYPE[]"
%typemap(jstype) (CTYPE *) "JAVAPRIMITIVETYPE[]"
%typemap(javain) (CTYPE *) "$javainput"
%typemap(javaout) (CTYPE *) {
    return $jnicall;
}

//////////////////////////

%typemap(argout) (CTYPE * nbRowItem, int nbRowItemL) {
// Specific target because it was freeing the wrong argument
    FREE($1);
}


%typemap(in) (CTYPE * nbRowItem, int nbRowItemL) {
     jboolean isCopy = JNI_FALSE;
     ##JNITYPE##* element = NULL;
// Convert the CTYPE[][] => CTYPE *
      $2 = (*jenv)->GetArrayLength(jenv, $input);
      $1 = (CTYPE*)MALLOC(sizeof(##CTYPE##) * $2);

      isCopy = JNI_FALSE;
      element = (##JNITYPE##*)(*jenv)->GetPrimitiveArrayCritical(jenv, $input, &isCopy);
      memcpy($1, element, sizeof(##CTYPE##) * $2);
      (*jenv)->ReleasePrimitiveArrayCritical(jenv, $input, element, 0); 
 }

%typemap(in) (CTYPE * colPos, int colPosL) = (CTYPE * nbRowItem, int nbRowItemL);
%typemap(argout) (CTYPE * colPos, int colPosL) = (CTYPE * nbRowItem, int nbRowItemL);
%typemap(in) (CTYPE * data, int dataL) = (CTYPE * nbRowItem, int nbRowItemL);
%typemap(argout) (CTYPE * data, int dataL) = (CTYPE * nbRowItem, int nbRowItemL);
%typemap(in) (CTYPE * imag, int imagL) = (CTYPE * nbRowItem, int nbRowItemL);
%typemap(argout) (CTYPE * imag, int imagL) = (CTYPE * nbRowItem, int nbRowItemL);
%enddef

// See SWIG documentation for the full list:
// http://www.swig.org/Doc1.3/Java.html#default_primitive_type_mappings

JAVASCI_SPARSE_ARRAYS_IMPL(double, jdouble, Double, double)     /* double[] */
JAVASCI_SPARSE_ARRAYS_IMPL(int, jint, Int, int)                 /* int[] */
