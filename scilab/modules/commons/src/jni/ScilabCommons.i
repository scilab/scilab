/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - 2010 - Sylvestre Ledru
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


/* ScilabCommons.i */
/** 
 * Windows: swig -java -package org.scilab.modules.commons -outdir ../java/org/scilab/modules/commons/ ScilabCommons.i 
 * Other: Use the option --enable-build-swig to the configure
*/
%module ScilabCommons
%{
#include "../../../core/includes/SCIHOME.h"
#include "../../../core/includes/tmpdir.h"
#include "../../../fileio/includes/createtempfilename.h"
#include "../../../localization/includes/setgetlanguage.h"
#include "../../../core/includes/setieee.h"
#include "../../../core/includes/setformat.h"
%}

/* JavaDoc for ScilabCommonsJNI class */
%pragma(java) jniclassclassmodifiers=%{
/* It is generated code. Disable checkstyle */
//CHECKSTYLE:OFF
 /** 
   * Some commons values from Scilab engine to Java
   * @author Sylvestre LEDRU
   * @copyright DIGITEO 2010
   */
public class%}

/* Constructor for ScilabCommonsJNI class */
%pragma(java) jniclasscode="
  /**
    * Constructor
    */
  protected ScilabCommonsJNI() {
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


/* JavaDoc for ScilabCommons class */
%pragma(java) moduleclassmodifiers="
 /** 
   * Some commons values from Scilab engine to Java
   * @author Sylvestre LEDRU
   * @copyright DIGITEO 2010
   */
public class";

/* Constructor for ScilabCommons class */
%pragma(java) modulecode="
 /**
   * Constructor
   */
 protected ScilabCommons() {
    throw new UnsupportedOperationException();
 }";

/* JavaDoc */
%javamethodmodifiers getSCIHOME() "
 /**
   * Get SCIHOME variable from Scilab
   * @return SCIHOME value
   */
public";
char* getSCIHOME();


/* JavaDoc */
%javamethodmodifiers getTMPDIR() "
 /**
   * returns TMPDIR directory value
   * @return TMPDIR value
   */
public";
char *getTMPDIR(void);


/* JavaDoc */
%javamethodmodifiers getScilabLanguage() "
 /**
   * returns Scilab current language
   * @return language
   */
public";
char *getlanguage(void);


/* JavaDoc */
%javamethodmodifiers createTempFileName() "
 /**
  * Create file with unique file name in TMPDIR
  * TMPDIR/PREFIX[XXXXXXX]
  * @param prefix the file prefix
  * @param bShortFormat set 1 for a 8.5 filename, 0 otherwise
  * @return a unique filename
  */
public";
char *createtempfilename(const char* prefix, int bShortFormat);

/* JavaDoc */
%javamethodmodifiers getScilabIeeeMode() "
 /**
   * returns the Scilab ieee mode 
   * @return ieee mode
   */
public";
int getieee(void);

/* JavaDoc */
%javamethodmodifiers setScilabIeeeMode(int) "
 /**
   * Set the Scilab ieee mode 
   */
public";
void setieee(int);

/* JavaDoc */
%javamethodmodifiers setformat(const char *, int) "
 /**
   * Set the Scilab ieee mode 
   */
public";
void setformat(const char * format, int width);

/* JavaDoc */
%javamethodmodifiers getformat() "
 /**
   * returns the Scilab format (\"v\" or \"e\") 
   * @return format
   */
public";
char * getformat();

/* JavaDoc */
%javamethodmodifiers getformatwidth() "
 /**
   * returns the Scilab format width 
   * @return format
   */
public";
int getformatwidth();
