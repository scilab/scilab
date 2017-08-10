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
    
    for (; i < nbRow; i++)
    {
        jobjectArray jarray = (*jenv)->NewObjectArray(jenv, nbCol, clazz, NULL);
	if (jarray == NULL)
	{
	    printf("Could not allocate\n");
	    fflush(NULL);
	}
	
	for (j = 0; j < nbCol; j++) {
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
    int i = 0;
    for (; i < $2 * $3; i++)
    {
	FREE($1[i]);
    }
    FREE($1);
}

%typemap(in) (char **variable, int nbRow, int nbCol) {
    int i=0, j=0;
    // Convert the String[][] => char *
    $2 = (*jenv)->GetArrayLength(jenv, $input);
    $3 = 0;
    $1 = NULL;

    for (; i < $2; i++)
    {
	jobjectArray oneDim = (jobjectArray)(*jenv)->GetObjectArrayElement(jenv, $input, i);
	if ($3 == 0)
	{
	    /* First time we are here, init + create the array where we store the data */
	    $3 = (*jenv)->GetArrayLength(jenv, oneDim);
	    $1 = (char**)malloc(sizeof(char*) * $2 * $3);
	}
	for (j = 0; j < $3; j++)
	{
	    jstring j_string = (jstring)(*jenv)->GetObjectArrayElement(jenv, oneDim, j);
	    jboolean isCopy = JNI_FALSE;
	    char *str = (char *)(*jenv)->GetStringUTFChars(jenv, j_string, &isCopy);
	    $1[j * $2 + i] = (char*)MALLOC(sizeof(char)*(strlen(str) + 1));
	    strcpy($1[j * $2 + i], str);
	    if (isCopy)
	    {
		(*jenv)->ReleaseStringUTFChars(jenv, j_string, (const char *)str);
	    }
	    (*jenv)->DeleteLocalRef(jenv, j_string);
	}
	(*jenv)->DeleteLocalRef(jenv, oneDim);
    }
}

%typemap(javain) char[ANY], char[] "$javainput"

%typemap(jtype) (char* variableName, int *nbRow, int *nbCol) "String"
%typemap(jstype) (char* variableName, int *nbRow, int *nbCol) "String"



