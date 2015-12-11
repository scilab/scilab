/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/* ScilabTermination.i */
/**
 * Windows: swig -java -package org.scilab.modules.gui -outdir ../java/org/scilab/modules/gui/ ScilabTermination.i
 * Other: Use the option --enable-build-swig to the configure
 */
%module ScilabTermination

%{
#include "ScilabTermination.h"
%}

/* JavaDoc for ScilabKeywordsJNI class */
%pragma(java) jniclassclassmodifiers=%{
/* It is generated code. Disable checkstyle */
//CHECKSTYLE:OFF
 /**
   * @author Calixte DENIZET
   */
public class%}

/* Constructor for ScilabKeywordsJNI class */
%pragma(java) jniclasscode="
  /**
    * Constructor
    */
  protected ScilabTerminationJNI() {
    throw new UnsupportedOperationException();
  }";

/* static load of library */
%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("scigui");
    } catch (SecurityException e) {
        System.err.println("A security manager exists and does not allow the loading of the specified dynamic library.");
        System.err.println(e.getLocalizedMessage());
        e.printStackTrace(System.err);
    } catch (UnsatisfiedLinkError e) {
          if (System.getenv("CONTINUE_ON_JNI_ERROR") == null) {
           System.err.println("The native library core does not exist or cannot be found.");
           System.err.println(e.getLocalizedMessage());
           e.printStackTrace(System.err);
        }
    }
  }
%}

/* JavaDoc for ScilabKeywords class */
%pragma(java) moduleclassmodifiers="
 /**
   * @author Calixte DENIZET
   */
public class";

/* Constructor for ScilabKeywords class */
%pragma(java) modulecode="
 /**
   * Constructor
   */
 protected ScilabTermination() {
    throw new UnsupportedOperationException();
 }";


/* JavaDoc */
%javamethodmodifiers ScilabExit() "
/**
* Exit Scilab
*/
public";
void ScilabExit(void);
