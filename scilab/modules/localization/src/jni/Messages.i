/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
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
 * Windows: swig -java -package org.scilab.modules.localization -outdir ../java/org/scilab/modules/localization/ Messages.i 
 * Others: Use the option --enable-build-swig to the configure
*/
%module Messages

%{
#include "scilabDefaults.h"
#include "localizationJava.h"
%}


/* JavaDoc for MessagesJNI class */
%pragma(java) jniclassclassmodifiers=%{

/* It is generated code. Disable checkstyle */
//CHECKSTYLE:OFF
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
        if (System.getProperty("os.name").toLowerCase().contains("windows") != true) {
            if (System.getProperty("testngTests")!=null) {
                System.loadLibrary("scilab");
            }
        }
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

%rename(dgettext) scidgettext;
char *scidgettext(char *domain, char *key);
