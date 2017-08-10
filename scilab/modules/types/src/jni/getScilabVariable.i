/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
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

/* getScilabVariable.i */
/**
 * Windows: swig -java -package org.scilab.modules.types -outdir ../java/org/scilab/modules/types/ getScilabVariable.i
 * Other: Use the option --enable-build-swig to the configure
*/

%module GetScilabVariable

%{
#include "GetScilabVariable.h"
%}

#ifdef SWIGJAVA

/* JavaDoc for GetScilabVariableJNI class */
%pragma(java) jniclassclassmodifiers=%{
/* It is generated code. Disable checkstyle */
//CHECKSTYLE:OFF
 /**
   * Connector for Javasci v2.
   * This class should <b>not</b> be used directly
   * @see org.scilab.modules.javasci.Scilab
   * @author DIGITEO - Sylvestre LEDRU
   */
class%}

/* Constructor for GetScilabVariableJNI class */
%pragma(java) jniclasscode="
  /**
    * Constructor
    */
  protected GetScilabVariableJNI() {
    throw new UnsupportedOperationException();
  }";

/* static load of library */
%pragma(java) jniclasscode=%{
  static {
    try {
        if (System.getProperty("os.name").toLowerCase().contains("windows")) {
           System.loadLibrary("typesjni");
        } else {
           System.loadLibrary("scilab");
        }
    } catch (SecurityException e) {
        System.err.println("A security manager exists and does not allow the loading of the specified dynamic library :");
        e.printStackTrace(System.err);
    } catch (UnsatisfiedLinkError e)    {
        System.err.println("The native library libscilab does not exist or cannot be found.");
        e.printStackTrace(System.err);
    }
  }
%}

/* JavaDoc for  class */
%pragma(java) moduleclassmodifiers="
 /**
   * Connector for Javasci v2.
   * This class should <b>not</b> be used directly
   * @see org.scilab.modules.javasci.Scilab
   * @author DIGITEO - Sylvestre LEDRU
   */
public class";

/* Constructor for Call_Scilab class */
%pragma(java) modulecode="
 /**
   * Constructor
   */
 protected GetScilabVariable() {
    throw new UnsupportedOperationException();
 }";

#endif

void getScilabVariable(const char * variableName, int, int);

void getScilabVariableAsReference(const char * variableName, int);
