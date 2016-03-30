/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - 2007 - Vincent Couvert
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


/* GuiManagement.i */
/** 
 * Windows: swig -java -package org.scilab.modules.console -outdir ../java/org/scilab/modules/console/ GuiManagement.i 
 * Other: Use the option --enable-build-swig to the configure
*/
%module GuiManagement
%{
#include "sci_malloc.h"
#include "../c/GuiManagement.h"
%}

%include "../../../jvm/src/jni/scilab_typemaps.i"

/* JavaDoc for GuiManagementJNI class */
%pragma(java) jniclassclassmodifiers=%{
/* It is generated code. Disable checkstyle */
//CHECKSTYLE:OFF
 /** 
   * All Scilab gui management used in Java console 
   * @author Vincent COUVERT
   * @copyright INRIA 2007
   */
public class%}

/* Constructor for GuiManagementJNI class */
%pragma(java) jniclasscode="
  /**
    * Constructor
    */
  protected GuiManagementJNI() {
    throw new UnsupportedOperationException();
  }";
  
/* static load of library */
%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("sciconsole");
    } catch (SecurityException e) {
        System.err.println("A security manager exists and does not allow the loading of the specified dynamic library.");
        System.err.println(e.getLocalizedMessage());
        System.exit(-1);
    } catch (UnsatisfiedLinkError e)    {
           System.err.println("The native library sciconsole does not exist or cannot be found.");
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


/* JavaDoc for GuiManagement class */
%pragma(java) moduleclassmodifiers="
 /** 
   * All Scilab gui management used in Java console 
   * @author Vincent COUVERT
   * @copyright INRIA 2007 
   */
public class";

/* Constructor for GuiManagement class */
%pragma(java) modulecode="
 /**
   * Constructor
   */
 protected GuiManagement() {
    throw new UnsupportedOperationException();
 }";

/* JavaDoc */
%javamethodmodifiers setScilabLines(int nbRows, int nbCols) "
 /**
   * Set Scilab parameters used to display data according the size of the console
   * @param nbRows number of rows that can be used to display data
   * @param nbCols number of columns that can be used to display data
   */
";
void setScilabLines(int nbRows, int nbCols);

%javamethodmodifiers setScilabLines(int nbRows, int nbCols) "
 /**
   * Set Scilab parameters used to display data
   * @param nbRows number of rows that can be used to display data
   * @param nbCols number of columns that can be used to display data
   */
";
void forceScilabLines(int nbRows, int nbCols);
