/* Allan CORNET */
/* INRIA 2007 */
/* Completion.i */
/*  swig -java -package org.scilab.modules.completion -outdir ../java/org/scilab/modules/completion/ Completion.i */
%module Completion

%{
#include "../c/completion_wrap_java.h"
#include "MALLOC.h"
%}

%include "../../../jvm/src/jni/scilab_typemaps.i"

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

/* static load of library */
%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("scicompletion");
    } catch (SecurityException e) {
		System.err.println("A security manager exists and does not allow the loading of the specified dynamic library :");
		e.printStackTrace(System.err);
	} catch (UnsatisfiedLinkError e)	{
		System.err.println("The native library scicompletion does not exist or cannot be found.");
		e.printStackTrace(System.err);
    }
  }
%}

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
public";
char **searchAllDictionaries(char *somechars);

/* JavaDoc */
%javamethodmodifiers searchFunctionsDictionary(char *somechars) "
/**
* completion function on Functions for scilab
* @param[in] somechars first characters of a word
* @return a array of chars 
*/
public";
char **searchFunctionsDictionary(char *somechars);

/* JavaDoc */
%javamethodmodifiers searchCommandsDictionary(char *somechars) "
/**
* completion function on command words for scilab
* @param[in] somechars first characters of a word
* @return a array of chars 
*/
public";
char **searchCommandsDictionary(char *somechars);

/* JavaDoc */
%javamethodmodifiers searchMacrosDictionary(char *somechars) "
/**
* completion function on Macros in libraries for scilab
* @param[in] somechars first characters of a word
* @return a array of chars 
*/
public";
char **searchMacrosDictionary(char *somechars);

/* JavaDoc */
%javamethodmodifiers searchVariablesDictionary(char *somechars) "
/**
* completion function on Variables for scilab
* @param[in] somechars first characters of a word
* @return a array of chars 
*/
public";
char **searchVariablesDictionary(char *somechars);

/* JavaDoc */
%javamethodmodifiers searchFilesDictionary(char *somechars) "
/**
* completion function on files for scilab
* @param[in] somechars first characters of a word
* @return a array of chars 
*/
public";
char **searchFilesDictionary(char *somechars);

/* JavaDoc */
%javamethodmodifiers searchHandleGraphicsPropertiesDictionary(char *somechars) "
/**
* completion handle graphics properties for scilab
* @param[in] somechars first characters of a word
* @return a array of chars 
*/
public";
char **searchHandleGraphicsPropertiesDictionary(char *somechars);

