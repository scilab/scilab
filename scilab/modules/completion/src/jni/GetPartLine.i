/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/* Completion.i */
/*  swig -java -package org.scilab.modules.completion -outdir ../java/org/scilab/modules/completion/ GetPartLine.i */
%module GetPartLine

%{
#include "getPartLine.h"
#include "MALLOC.h"
%}

%include "../../../jvm/src/jni/scilab_typemaps.i"

/* JavaDoc for GetPartLineJNI class */
%pragma(java) jniclassclassmodifiers=%{
 /** 
   * @author Allan CORNET
   * @copyright DIGITEO 2008
   */
public class%}

/* Constructor for GetPartLineJNI class */
%pragma(java) jniclasscode="
  /**
    * Constructor
    */
  protected GetPartLineJNI() {
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

/* JavaDoc for GetPartLine class */
%pragma(java) moduleclassmodifiers="
 /** 
   * @author Allan CORNET
   * @copyright DIGITEO 2008
   */
public class";

/* Constructor for GetPartLine class */
%pragma(java) modulecode="
 /**
   * Constructor
   */
 protected GetPartLine() {
	throw new UnsupportedOperationException();
 }";

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

