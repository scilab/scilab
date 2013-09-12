/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/* ScilabKeywords.i */
/**
 * Windows: swig -java -package org.scilab.modules.scinotes -outdir ../java/org/scilab/modules/scinotes/ ScilabKeywords.i
 * Other: Use the option --enable-build-swig to the configure
 */
%module ScilabKeywords

%{
#include "MALLOC.h"
#include "ScilabKeywords.h"
%}

%include "../../../jvm/src/jni/scilab_typemaps.i"

/* JavaDoc for ScilabKeywordsJNI class */
%pragma(java) jniclassclassmodifiers=%{
/* It is generated code. Disable checkstyle */
//CHECKSTYLE:OFF
 /**
   * @author Allan CORNET - DIGITEO 2009
   * @author Calixte DENIZET
   */
public class%}

/* Constructor for ScilabKeywordsJNI class */
%pragma(java) jniclasscode="
  /**
    * Constructor
    */
  protected ScilabKeywordsJNI() {
    throw new UnsupportedOperationException();
  }";

/* static load of library */
%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("sciscinotes");
    } catch (SecurityException e) {
        System.err.println("A security manager exists and does not allow the loading of the specified dynamic library.");
        System.err.println(e.getLocalizedMessage());
        e.printStackTrace(System.err);
    } catch (UnsatisfiedLinkError e) {
          if (System.getenv("CONTINUE_ON_JNI_ERROR") == null) {
           System.err.println("The native library scinotes does not exist or cannot be found.");
           System.err.println(e.getLocalizedMessage());
           e.printStackTrace(System.err);
        }
    }
  }
%}

/* JavaDoc for ScilabKeywords class */
%pragma(java) moduleclassmodifiers="
 /**
   * @author Allan CORNET - DIGITEO 2009
   * @author Calixte DENIZET
   */
public class";

/* Constructor for ScilabKeywords class */
%pragma(java) modulecode="
 /**
   * Constructor
   */
 protected ScilabKeywords() {
    throw new UnsupportedOperationException();
 }";


/* JavaDoc */
%javamethodmodifiers GetVariablesName() "
/**
* get Variables Name
* @return an array of chars
*/
public";
char **GetVariablesName(void);

/* JavaDoc */
%javamethodmodifiers GetFunctionsName() "
/**
* get Functions Name
* @return an array of chars
*/
public";
char **GetFunctionsName(void);

/* JavaDoc */
%javamethodmodifiers GetMacrosName() "
/**
* get Macros Name
* @return an array of chars
*/
public";
char **GetMacrosName(void);
