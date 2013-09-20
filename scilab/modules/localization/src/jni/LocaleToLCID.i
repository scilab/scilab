/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/* Messages.i */
/**
 * Windows: swig -java -package org.scilab.modules.localization -outdir ../java/org/scilab/modules/localization/ LocaleToLCID.i 
 * Others: Use the option --enable-build-swig to the configure
*/
%module LocaleToLCID

%{
#include "getLcidString.h"
%}

/* JavaDoc for LocaleToLCIDJNI class */
%pragma(java) jniclassclassmodifiers=%{
/* It is generated code. Disable checkstyle */
//CHECKSTYLE:OFF
 /** 
   * @author Allan CORNET
   * @copyright DIGITEO 2010
   */
public class%}

/* Constructor for LocaleToLCIDJNI class */
%pragma(java) jniclasscode="
  /**
    * Constructor
    */
  protected LocaleToLCIDJNI() {
    throw new UnsupportedOperationException();
  }";

/* static load of library */
%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("scilocalization");
    } catch (SecurityException e) {
        System.err.println("A security manager exists and does not allow the loading of the specified dynamic library.");
        System.err.println(e.getLocalizedMessage());
        e.printStackTrace(System.err);
    } catch (UnsatisfiedLinkError e)    {
        System.err.println("The native library scilocalization does not exist or cannot be found.");
        System.err.println(e.getLocalizedMessage());
        e.printStackTrace(System.err);
    }
  }
%}

/* JavaDoc for LocaleToLCID class */
%pragma(java) moduleclassmodifiers="
 /** 
   * @author Allan CORNET
   * @copyright DIGITEO 2010
   */
public class";

/* Constructor for LocaleToLCID class */
%pragma(java) modulecode="
 /**
   * Constructor
   */
 protected LocaleToLCID() {
    throw new UnsupportedOperationException();
 }";

/* JavaDoc */
%javamethodmodifiers convert(const char *locale) "
/**
* convert a locale string to LCID string used by CHM builder
* @param[in] string the key of the message (usually the english msg)
* @return LCID string
*/
public";


%rename(convert) getLcidString;
char *getLcidString(const char *pStrLocale);
