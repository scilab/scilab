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
/* getCommonPart.i */
/*  swig -java -package org.scilab.modules.completion -outdir ../java/org/scilab/modules/completion/ getCommonPart.i */
%module GetCommonPart

%{
#include "getCommonPart.h"
#include "MALLOC.h"
%}

%include "../../../jvm/src/jni/scilab_typemaps.i"

/* JavaDoc for GetCommonPartJNI class */
%pragma(java) jniclassclassmodifiers=%{
 /** 
   * @author Allan CORNET
   * @copyright DIGITEO 2008
   */
public class%}

/* Constructor for etCommonPartJNI class */
%pragma(java) jniclasscode="
  /**
    * Constructor
    */
  protected GetCommonPartJNI() {
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

/* JavaDoc for GetCommonPart class */
%pragma(java) moduleclassmodifiers="
 /** 
   * @author Allan CORNET
   * @copyright DIGITEO 2008
   */
public class";

/* Constructor for GetCommonPart class */
%pragma(java) modulecode="
 /**
   * Constructor
   */
 protected GetCommonPart() {
	throw new UnsupportedOperationException();
 }";

/* JavaDoc */
%javamethodmodifiers getCommonPart(char **dictionnary, int sizeDictionnary) "
/**
* Get common part to a array
* @param[in] string array
* @param[in] size of string array
* @return common part
*/
public";
char *getCommonPart(char **dictionnary, int sizeDictionnary);
