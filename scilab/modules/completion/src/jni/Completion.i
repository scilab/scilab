/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
/* Completion.i */
/**
 * Windows: swig -java -package org.scilab.modules.completion -outdir ../java/org/scilab/modules/completion/ Completion.i
 * Other: Use the option --enable-build-swig to the configure
*/
%module Completion

%{
#include "../c/completion_wrap_java.h"
#include "../c/completeLine_wrap_java.h"
#include "getCommonPart.h"
#include "getPartLine.h"
#include "sci_malloc.h"
%}

%include "../../../jvm/src/jni/scilab_typemaps.i"

/* JavaDoc for CompletioJNI class */
%pragma(java) jniclassclassmodifiers=%{
/* It is generated code. Disable checkstyle */
//CHECKSTYLE:OFF
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
        System.err.println("A security manager exists and does not allow the loading of the specified dynamic library.");
        System.err.println(e.getLocalizedMessage());
        e.printStackTrace(System.err);
    } catch (UnsatisfiedLinkError e)    {
        System.err.println("The native library scicompletion does not exist or cannot be found.");
        System.err.println(e.getLocalizedMessage());
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
* @return an array of chars
*/
public";
char **searchAllDictionaries(char *somechars);

/* JavaDoc */
%javamethodmodifiers searchFunctionsDictionary(char *somechars) "
/**
* completion function on Functions for scilab
* @param[in] somechars first characters of a word
* @return an array of chars
*/
public";
char **searchFunctionsDictionary(char *somechars);

/* JavaDoc */
%javamethodmodifiers searchCommandsDictionary(char *somechars) "
/**
* completion function on command words for scilab
* @param[in] somechars first characters of a word
* @return an array of chars
*/
public";
char **searchCommandsDictionary(char *somechars);

/* JavaDoc */
%javamethodmodifiers searchMacrosDictionary(char *somechars) "
/**
* completion function on Macros in libraries for scilab
* @param[in] somechars first characters of a word
* @return an array of chars
*/
public";
char **searchMacrosDictionary(char *somechars);

/* JavaDoc */
%javamethodmodifiers searchVariablesDictionary(char *somechars) "
/**
* completion function on Variables for scilab
* @param[in] somechars first characters of a word
* @return an array of chars
*/
public";
char **searchVariablesDictionary(char *somechars);

/* JavaDoc */
%javamethodmodifiers searchFilesDictionary(char *somechars) "
/**
* completion function on files for scilab
* @param[in] somechars first characters of a word
* @return an array of chars
*/
public";
char **searchFilesDictionary(char *somechars);

/* JavaDoc */
%javamethodmodifiers searchFieldsDictionary(char *lineBeforeCaret, char *pattern) "
/**
* completion function on files for scilab
* @param[in] the line before the caret
* @param[in] the part
* @return an array of chars
*/
public";
char **searchFieldsDictionary(char *lineBeforeCaret, char *pattern);

/* JavaDoc */
%javamethodmodifiers searchHandleGraphicsPropertiesDictionary(char *somechars) "
/**
* completion handle graphics properties for scilab
* @param[in] somechars first characters of a word
* @return an array of chars
*/
public";
char **searchHandleGraphicsPropertiesDictionary(char *somechars);

/* JavaDoc */
%javamethodmodifiers getPartLevel(char *line) "
/**
* pre parsing current line to search with completion
* @param[in] current line
* @return word to find with completion
*/
public";
char *getPartLevel(char *line);

/* JavaDoc */
%javamethodmodifiers getFilePartLevel(char *line) "
/**
* pre parsing current line to search with completion (file)
* @param[in] current line
* @return word to find with completion
*/
public";
char *getFilePartLevel(char *line);

/* JavaDoc */
%javamethodmodifiers getCommonPart(char **dictionary, int sizeDictionary) "
/**
* Get common part to a array
* @param[in] string array
* @param[in] size of string array
* @return common part
*/
public";
char *getCommonPart(char **dictionary, int sizeDictionary);

/* JavaDoc */
%javamethodmodifiers completelineforjava(char *currentLine, char *stringToAdd, BOOL isFile, char *postCaretLine) "
/**
* complete line
* @param[in] current line (before caret)
* @param[in] line to add
* @param[in] type of line to add
* @param[in] line after caret
* @return modified line
*/
public";
char *completelineforjava(char *currentLine,char *stringToAdd,BOOL isFile, char *postCaretLine);
