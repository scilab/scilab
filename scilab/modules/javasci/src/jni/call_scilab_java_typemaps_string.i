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

%typemap(in) (char* variableName, int *nbRow, int *nbCol)  {
    $2 = &nbRow;
    $3 = &nbCol;
    $1 = 0;
    if ($input) {
       $1 = (char *)(*jenv)->GetStringUTFChars(jenv, $input, 0);
       if (!$1) return 0;
    }
}



// retrieve from the native code a char ** => String[][] (java)
%typemap(out) (char **) (int nbRow, int nbCol) {
      const jclass clazz = (*jenv)->FindClass(jenv, "java/lang/Object");
      int i = 0, j = 0;


    jresult = (*jenv)->NewObjectArray(jenv, nbRow,clazz, NULL);

    for (i=0; i < nbRow; i++) {

//      jdouble array[nbCol];
        jobjectArray jarray = (*jenv)->NewObjectArray(jenv, nbCol, clazz, NULL);
      if (jarray == NULL) {
        printf("Could not allocate\n");fflush(NULL);
      }
      
      for (j=0; j < nbCol; j++) {
        /* Scilab is storing matrice cols by cols while Java is doing it
                       row by row. Therefor, we need to convert it */
        
          jstring temp_string = (*jenv)->NewStringUTF(jenv, (const char *)result[nbRow*j+i]);
          (*jenv)->SetObjectArrayElement(jenv, jarray, j, temp_string);
          (*jenv)->DeleteLocalRef(jenv, temp_string);
      }
      
      (*jenv)->SetObjectArrayElement(jenv, jresult, i, jarray);
      
      (*jenv)->DeleteLocalRef(jenv, jarray);
      
    }
    
    
    if (arg1) (*jenv)->ReleaseStringUTFChars(jenv, jarg1, (const char *)arg1);
    free(result);
  
}

/* Transform the input datatype char[] to char[][] to facilitate the 
matching in Java */
%typemap(jni) (char **) "jobjectArray"
%typemap(jtype) (char **) "String[][]"
%typemap(jstype) (char **) "String[][]"

%typemap(argout) (char **variable, int nbRow, int nbCol) {
// Specific target because it was freeing the wrong argument
}

%typemap(in) (char **variable, int nbRow, int nbCol) {
    int i=0, j=0;
    // Convert the String[][] => char *
    arg3 = (*jenv)->GetArrayLength(jenv, jarg2);
    arg4 = 0;
    
    for(i=0; i<arg3; i++) {
      jobjectArray oneDim=(jobjectArray)(*jenv)->GetObjectArrayElement(jenv, jarg2, i);
      if (arg4==0) {
        /* First time we are here, init + create the array where we store the data */
        arg4 = (*jenv)->GetArrayLength(jenv, oneDim);
        arg2 = (char**)malloc(sizeof(char*)*arg3*arg4);
      }
//      jstring*element=(*jenv)->GetStringArrayElements(jenv, oneDim, 0);
      for(j=0; j<arg4; j++) {
          jstring j_string = (jstring)(*jenv)->GetObjectArrayElement(jenv, oneDim, j);
          char *str = (char *)(*jenv)->GetStringUTFChars(jenv, j_string, 0);
          arg2[j*arg3+i]=(char*)malloc(sizeof(char)*(strlen(str)+1));
          strcpy(arg2[j*arg3+i],str);
          (*jenv)->ReleaseStringUTFChars(jenv, j_string, (const char *)str);

      }
    }
   
}

%typemap(javain) char[ANY], char[] "$javainput"

%typemap(jtype) (char* variableName, int *nbRow, int *nbCol) "String"
%typemap(jstype) (char* variableName, int *nbRow, int *nbCol) "String"



