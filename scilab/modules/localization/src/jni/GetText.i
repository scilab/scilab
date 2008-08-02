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

/* GetText.i */
/* swig -java -package org.scilab.modules.localization -outdir ../java/org/scilab/modules/localization/ GetText.i */


%module Localization
%{
#include "MALLOC.h"
#include "getTextWarp.h"
%}

%include "../../../jvm/src/jni/scilab_typemaps.i"

/* JavaDoc for GetTextWarpJNI class */
%pragma(java) jniclassclassmodifiers=%{
 /** 
   * gettext warp
   * @author Allan CORNET
   * @copyright DIGITEO 2008
   */
public class%}

/* Constructor for GetTextWarpJNI class */
%pragma(java) jniclasscode="
  /**
    * Constructor
    */
  protected LocalizationJNI() {
	throw new UnsupportedOperationException();
  }";
  
/* static load of library */
%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("scilocalization");
    } catch (SecurityException e) {
		System.err.println("A security manager exists and does not allow the loading of the specified dynamic library :");
		System.err.println(e.getLocalizedMessage());
		System.exit(-1);
	} catch (UnsatisfiedLinkError e)	{
		System.err.println("The native library scilocalization does not exist or cannot be found :");
		System.err.println(e.getLocalizedMessage());
		System.err.println("Current java.library.path is : "+System.getProperty("java.library.path"));
		System.exit(-1);
    }
  }
%}

/* JavaDoc for GetTextWarp class */
%pragma(java) moduleclassmodifiers="
 /** 
   * gettext warp
   * @author Allan CORNET
   * @copyright DIGITEO 2008
   */
public class";

/* Constructor for GetTextWarp class */
%pragma(java) modulecode="
 /**
   * Constructor
   */
 protected Localization() {
	throw new UnsupportedOperationException();
 }";

/* JavaDoc */
%javamethodmodifiers getTextC(char *stringIn) "
/**
* gettext function
* @param[in] english string
* @return translated string
*/
public";
char *getTextC(char *stringIn);
