/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/* Messages.i */
/*  swig -java -package org.scilab.modules.localization -outdir ../java/org/scilab/modules/localization/ Messages.i */
%module Messages

%{
#include "scilabDefaults.h"
#include "localizationJava.h"
%}

/* JavaDoc for MessagesJNI class */
%pragma(java) jniclassclassmodifiers=%{
 /** 
   * @author Sylvestre LEDRU
   * @copyright DIGITEO 2009
   */
public class%}

/* Constructor for MessagesJNI class */
%pragma(java) jniclasscode="
  /**
    * Constructor
    */
  protected MessagesJNI() {
	throw new UnsupportedOperationException();
  }";

/* static load of library */
%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("scilocalization");
    } catch (SecurityException e) {
		System.err.println("A security manager exists and does not allow the loading of the specified dynamic library :");
		e.printStackTrace(System.err);
	} catch (UnsatisfiedLinkError e)	{
		System.err.println("The native library scilocalization does not exist or cannot be found.");
		e.printStackTrace(System.err);
    }
  }
%}

/* JavaDoc for Messages class */
%pragma(java) moduleclassmodifiers="
 /** 
   * @author Sylvestre Ledru
   * @copyright DIGITEO 2008
   */
public class";

/* Constructor for Messages class */
%pragma(java) modulecode="
 /**
   * Constructor
   */
 protected Messages() {
	throw new UnsupportedOperationException();
 }";

/* JavaDoc */
%javamethodmodifiers gettext(char *key) "
/**
* Translate a message
* @param[in] string the key of the message (usually the english msg)
* @return The localized string
*/
public";

/* Thanks to rename, gettext is the Java method and in the native code, 
we are calling the C function scigettext */
%rename(gettext) scigettext;
char *scigettext(char *key);
