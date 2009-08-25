/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/* ScilabKeywords.i */
/*  swig -java -package org.scilab.modules.xpad -outdir ../java/org/scilab/modules/xpad/ ScilabKeywords.i */
%module ScilabKeywords

%{
#include "MALLOC.h"
#include "ScilabKeywords.h"
%}

%include "../../../jvm/src/jni/scilab_typemaps.i"

/* JavaDoc for ScilabKeywordsJNI class */
%pragma(java) jniclassclassmodifiers=%{
 /** 
   * @author Allan CORNET
   * @copyright DIGITEO 2009
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
  private static final String OSNAME = System.getProperty("os.name").toLowerCase();
  static {
    try {
        if (OSNAME.indexOf("windows") != -1) {
        System.loadLibrary("scixpad");
        }
        else {
        System.loadLibrary("xpad");
        }
    } catch (SecurityException e) {
		System.err.println("A security manager exists and does not allow the loading of the specified dynamic library :");
		e.printStackTrace(System.err);
	} catch (UnsatisfiedLinkError e)	{
		System.err.println("The native library xpad does not exist or cannot be found.");
		e.printStackTrace(System.err);
    }
  }
%}

/* JavaDoc for ScilabKeywords class */
%pragma(java) moduleclassmodifiers="
 /** 
   * @author Allan CORNET
   * @copyright DIGITEO 2009
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
* get Commands Name
* @return a array of chars 
*/
public";
char **ScilabKeyword_GetCommandsName(void);

/* JavaDoc */
%javamethodmodifiers GetVariablesName() "
/**
* get Functions Name
* @return a array of chars 
*/
public";
char **ScilabKeyword_GetFunctionsName(void);

/* JavaDoc */
%javamethodmodifiers GetVariablesName() "
/**
* get Macros Name
* @return a array of chars 
*/
public";
char **ScilabKeyword_GetMacrosName(void);
