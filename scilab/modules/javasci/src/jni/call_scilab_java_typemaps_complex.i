

%define JAVASCI_COMPLEX_ARRAYS_IMPL(CTYPE, JNITYPE, JAVATYPE, JAVAPRIMITIVETYPE, JNICODE)

/* Transform the input datatype CTYPE[] to JAVAPRIMITIVETYPE[][] to facilitate the 
matching in Java */
%typemap(jni) CTYPE[] "jobjectArray"
%typemap(jtype) CTYPE[] "JAVAPRIMITIVETYPE[]"
%typemap(jstype) CTYPE[] "JAVAPRIMITIVETYPE[]"

%typemap(argout) (CTYPE variable[], int nbRow, int nbCol) {
// Specific target because it was freeing the wrong argument
}

%typemap(in) (CTYPE variable[], int nbRow, int nbCol) {
// Convert the CTYPE[][] => CTYPE *
      $2 = (*jenv)->GetArrayLength(jenv, $input);
   	  $3 = 0;
	  int i=0, j=0;
	  for(i=0; i<$2; i++) {
		  ##JNITYPE##Array oneDim=(##JNITYPE##Array)(*jenv)->GetObjectArrayElement(jenv, jarg2, i);
		  if ($3==0) {
			  /* First time we are here, init + create the array where we store the data */
			  $3 = (*jenv)->GetArrayLength(jenv, oneDim);
			  $1 = (CTYPE*)malloc(sizeof(##CTYPE##)*arg3*arg4);
		  }
		  ##JNITYPE## *element=(*jenv)->Get##JAVATYPE##ArrayElements(jenv, oneDim, 0);

		  for(j=0; j<$3; j++) {
			  $1[j*$2+i]=element[j];
		  }
	  }

}
%enddef



JAVASCI_COMPLEX_ARRAYS_IMPL(double, jdouble, Double, double, "[D")     /* double[] */
