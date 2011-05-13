/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - 2010 - Sylvestre Ledru
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


/* ScilabCommons.i */
/** 
 * Windows: swig -java -package org.scilab.modules.commons -outdir ../java/org/scilab/modules/commons/ ScilabCommons.i 
 * Other: Use the option --enable-build-swig to the configure
*/
%module ScilabCommons
%{
#include "../../../core/includes/SCIHOME.h"
#include "../../../core/includes/tmpdir.h"
%}

/* JavaDoc for ScilabCommonsJNI class */
%pragma(java) jniclassclassmodifiers=%{
/* It is generated code. Disable checkstyle */
//CHECKSTYLE:OFF
 /** 
   * Some commons values from Scilab engine to Java
   * @author Sylvestre LEDRU
   * @copyright DIGITEO 2010
   */
public class%}

/* Constructor for ScilabCommonsJNI class */
%pragma(java) jniclasscode="
  /**
    * Constructor
    */
  protected ScilabCommonsJNI() {
    throw new UnsupportedOperationException();
  }";
  
/* static load of library */
%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("scicommons");
    } catch (SecurityException e) {
        System.err.println("A security manager exists and does not allow the loading of the specified dynamic library.");
        System.err.println(e.getLocalizedMessage());
        System.exit(-1);
    } catch (UnsatisfiedLinkError e)    {
           System.err.println("The native library scicommons does not exist or cannot be found.");
        if (System.getenv("CONTINUE_ON_JNI_ERROR") == null) {
           System.err.println(e.getLocalizedMessage());
           System.err.println("Current java.library.path is : "+System.getProperty("java.library.path"));
           System.exit(-1);
        }else{
           System.err.println("Continuing anyway because of CONTINUE_ON_JNI_ERROR");
        }
    }
  }
%}


/* JavaDoc for ScilabCommons class */
%pragma(java) moduleclassmodifiers="
 /** 
   * Some commons values from Scilab engine to Java
   * @author Sylvestre LEDRU
   * @copyright DIGITEO 2010
   */
public class";

/* Constructor for ScilabCommons class */
%pragma(java) modulecode="
 /**
   * Constructor
   */
 protected ScilabCommons() {
    throw new UnsupportedOperationException();
 }";

/* JavaDoc */
%javamethodmodifiers getSCIHOME() "
 /**
   * Get SCIHOME variable from Scilab
   * @return SCIHOME value
   */
public";
char* getSCIHOME();


/* JavaDoc */
%javamethodmodifiers getTMPDIR() "
 /**
   * returns TMPDIR directory value
   * @return TMPDIR value
   */
public";
char *getTMPDIR(void);
