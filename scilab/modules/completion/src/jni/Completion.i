/* Allan CORNET */
/* INRIA 2007 */
/* Completion.i */
/*  swig -java -package org.scilab.modules.completion -outdir ../java/org/scilab/modules/completion/ Completion.i */
%module Completion

%{
#include "../c/completion_wrap_java.h"
#include "MALLOC.h"
%}

/* String[] <--> char ** support */

%typemap(in) char ** (jint size) {
    int i = 0;
    size = (*jenv)->GetArrayLength(jenv, $input);
    $1 = (char **) MALLOC((size+1)*sizeof(char *));
    /* make a copy of each string */
    for (i = 0; i<size; i++) {
        jstring j_string = (jstring)(*jenv)->GetObjectArrayElement(jenv, $input, i);
        const char * c_string = (*jenv)->GetStringUTFChars(jenv, j_string, 0);
        $1[i] = MALLOC(strlen((c_string)+1)*sizeof(const char *));
        strcpy($1[i], c_string);
        (*jenv)->ReleaseStringUTFChars(jenv, j_string, c_string);
        (*jenv)->DeleteLocalRef(jenv, j_string);
    }
    $1[i] = 0;
}

/* This cleans up the memory we malloc'd before the function call */
%typemap(freearg) char ** {
    int i;
    for (i=0; i<size$argnum-1; i++)
      FREE($1[i]);
    FREE($1);
}

/* This allows a C function to return a char ** as a Java String array */
%typemap(out) char ** {
    int i;
    int len=0;
    jstring temp_string;
    const jclass clazz = (*jenv)->FindClass(jenv, "java/lang/String");

    while ($1[len]) len++;    
    jresult = (*jenv)->NewObjectArray(jenv, len, clazz, NULL);
    /* exception checking omitted */

    for (i=0; i<len; i++) {
      temp_string = (*jenv)->NewStringUTF(jenv, *result++);
      (*jenv)->SetObjectArrayElement(jenv, jresult, i, temp_string);
      (*jenv)->DeleteLocalRef(jenv, temp_string);
    }
}

/* These 3 typemaps tell SWIG what JNI and Java types to use */
%typemap(jni) char ** "jobjectArray"
%typemap(jtype) char ** "String[]"
%typemap(jstype) char ** "String[]"


/* These 2 typemaps handle the conversion of the jtype to jstype typemap type
   and visa versa */
%typemap(javain) char ** "$javainput"
%typemap(javaout) char ** {
    return $jnicall;
  }

/* String[] <--> char ** support */

/* JavaDoc for CompletioJNI class */
%pragma(java) jniclassclassmodifiers=%{
 /** 
   * @author Allan CORNET
   * @copyright INRIA 2007
   */
public class%}

/* Constructor for CompletionJNI class */
%pragma(java) jniclasscode="
  /**
    * Constructor
    */
  protected CompletionJNI() {
	throw new UnsupportedOperationException();
  }";

/* JavaDoc for Completion class */
%pragma(java) moduleclassmodifiers="
 /** 
   * @author Allan CORNET
   * @copyright INRIA 2007
   */
public class";

/* Constructor for Completion class */
%pragma(java) modulecode="
 /**
   * Constructor
   */
 protected Completion() {
	throw new UnsupportedOperationException();
 }";

/* JavaDoc */
%javamethodmodifiers searchAllDictionaries(char *somechars) "
/**
* completion function on all types for scilab
* @param[in] somechars first characters of a word
* @return a array of chars 
*/
";
char **searchAllDictionaries(char *somechars);

/* JavaDoc */
%javamethodmodifiers searchFunctionsDictionary(char *somechars) "
/**
* completion function on Functions for scilab
* @param[in] somechars first characters of a word
* @return a array of chars 
*/
";
char **searchFunctionsDictionary(char *somechars);

/* JavaDoc */
%javamethodmodifiers searchCommandsDictionary(char *somechars) "
/**
* completion function on command words for scilab
* @param[in] somechars first characters of a word
* @return a array of chars 
*/
";
char **searchCommandsDictionary(char *somechars);

/* JavaDoc */
%javamethodmodifiers searchMacrosDictionary(char *somechars) "
/**
* completion function on Macros in libraries for scilab
* @param[in] somechars first characters of a word
* @return a array of chars 
*/
";
char **searchMacrosDictionary(char *somechars);

/* JavaDoc */
%javamethodmodifiers searchVariablesDictionary(char *somechars) "
/**
* completion function on Variables for scilab
* @param[in] somechars first characters of a word
* @return a array of chars 
*/
";
char **searchVariablesDictionary(char *somechars);

/* JavaDoc */
%javamethodmodifiers searchFilesDictionary(char *somechars) "
/**
* completion function on files for scilab
* @param[in] somechars first characters of a word
* @return a array of chars 
*/
";
char **searchFilesDictionary(char *somechars);

/* JavaDoc */
%javamethodmodifiers searchHandleGraphicsPropertiesDictionary(char *somechars) "
/**
* completion handle graphics properties for scilab
* @param[in] somechars first characters of a word
* @return a array of chars 
*/
";
char **searchHandleGraphicsPropertiesDictionary(char *somechars);

