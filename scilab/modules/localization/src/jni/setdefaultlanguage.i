/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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
/* Messages.i */
/**
 * Windows: swig -java -package org.scilab.modules.localization -outdir ../java/org/scilab/modules/localization/ setdefaultlanguage.i 
 * Others: Use the option --enable-build-swig to the configure
*/
%module WindowsDefaultLanguage

%{
#include "setgetlanguage.h"
%}

/* JavaDoc for WindowsDefaultLanguage class */
%pragma(java) jniclassclassmodifiers=%{
/* It is generated code. Disable checkstyle */
//CHECKSTYLE:OFF
 /** 
   * @author Calixte DENIZET
   * @copyright Scilab Enterprises - 2012
   */
public class%}

/* Constructor for WindowsDefaultLanguageJNI class */
%pragma(java) jniclasscode="
  /**
    * Constructor
    */
  protected WindowsDefaultLanguageJNI() {
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

/* JavaDoc for WindowsDefaultLanguage class */
%pragma(java) moduleclassmodifiers="
 /** 
   * @author Calixte DENIZET
   * @copyright Scilab Enterprises - 2012
   */
public class";

/* Constructor for WindowsDefaultLanguage class */
%pragma(java) modulecode="
 /**
   * Constructor
   */
 protected WindowsDefaultLanguage() {
    throw new UnsupportedOperationException();
 }";

/* JavaDoc */
%javamethodmodifiers setdefaultlanguage(const char *locale) "
/**
* Set and save the language in the registry (Windows only)
* @param[in] lang the language to set and save.
*/
public";


void setdefaultlanguage(const char *lang);

%javamethodmodifiers getdefaultlanguage(void) "
/**
* Get the language from the registry (Windows only)
* @return the default language.
*/
public";


char * getdefaultlanguage(void);
