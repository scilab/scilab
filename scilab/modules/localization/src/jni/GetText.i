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
/*  swig -java -package org.scilab.modules.localization -outdir ../java/org/scilab/modules/localization/ GetText.i */
%module Localization

%{
#include "MALLOC.h"
#include "getTextWarp.h"
%}

%include "../../../jvm/src/jni/scilab_typemaps.i"

/* JavaDoc for GetTextWrapJNI class */
%pragma(java) jniclassclassmodifiers=%{
 /** 
   * @author Allan CORNET
   * @copyright DIGITEO 2008
   */
public class%}

/* static load of library */
%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("scilocalization");
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
   * @copyright DIGITEO 2008
   */
public class";

/* JavaDoc */
%javamethodmodifiers GetText(char *stringIn) "
/**
* completion function on all types for scilab
* @param[in] english string
* @return translated string
*/
public";
char *getTextWarp(char *stringIn);
