/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2011 - Calixte DENIZET
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


/* CommonFileUtils.i */
/** 
 * Windows: swig -java -package org.scilab.modules.commons -outdir ../java/org/scilab/modules/commons/ CommonFileUtils.i 
 * Other: Use the option --enable-build-swig to the configure
*/
%module CommonFileUtils
%{
#include "../../includes/fileutils.h"
#include "sci_malloc.h"
%}

%include "../../../jvm/src/jni/scilab_typemaps.i"

/* JavaDoc for CommonFileUtilsJNI class */
%pragma(java) jniclassclassmodifiers=%{
/* It is generated code. Disable checkstyle */
//CHECKSTYLE:OFF
 /** 
   * Some file utils
   * @author Calixte DENIZET
   * @copyright DIGITEO 2011
   */
public class%}

/* Constructor for CommonFileUtilsJNI class */
%pragma(java) jniclasscode="
  /**
    * Constructor
    */
  protected CommonFileUtilsJNI() {
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


/* JavaDoc for CommonFileUtils class */
%pragma(java) moduleclassmodifiers="
 /** 
   * Some file utils
   * @author Calixte DENIZET
   * @copyright DIGITEO 2011
   */
public class";

/* Constructor for CommonFileUtils class */
%pragma(java) modulecode="
 /**
   * Constructor
   */
 protected CommonFileUtils() {
    throw new UnsupportedOperationException();
 }";

/* JavaDoc */
%javamethodmodifiers isEmptyDirectory(String dirName) "
 /**
   * Test if a directory is empty or not
   * @param dirName the directory name
   * @return true if it is a directory
   */
public";
int isEmptyDirectory(char * dirName);

/* JavaDoc */
%javamethodmodifiers getCWD() "
 /**
   * Get the current working directory
   * @return the Scilab CWD
   */
public";
char * getCWD();

