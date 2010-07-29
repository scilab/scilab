/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* Call_Scilab.i */
/**
 * Windows: swig -java -package org.scilab.modules.javasci -outdir ../java/org/scilab/modules/javasci/ call_scilab.i 
 * Other: Use the option --enable-build-swig to the configure
*/

//%rename(ScilabTypeEnum) sci_types;

%module Call_Scilab
//%{
//#include "../c/Call_Scilab.h"
//%}
%{
#include "BOOL.h"
#include "../../../core/includes/sci_types.h"
%}
%include "../../../jvm/src/jni/scilab_typemaps.i"

%import "../../../types/src/jni/sci_types.i"

#ifdef SWIGJAVA

/* JavaDoc for Call_ScilabJNI class */
%pragma(java) jniclassclassmodifiers=%{
/* It is generated code. Disable checkstyle */
//CHECKSTYLE:OFF
 /**
   * Connector for Javasci v2
   * @author Sylvestre LEDRU
   * @copyright DIGITEO
   */
class%}

/* Constructor for Call_ScilabJNI class */
%pragma(java) jniclasscode="
  /**
    * Constructor
    */
  protected Call_ScilabJNI() {
	throw new UnsupportedOperationException();
  }";

/* static load of library */
%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("scilab");
        System.loadLibrary("javasci");
    } catch (SecurityException e) {
		System.err.println("A security manager exists and does not allow the loading of the specified dynamic library :");
		e.printStackTrace(System.err);
	} catch (UnsatisfiedLinkError e)	{
		System.err.println("The native library javasci does not exist or cannot be found.");
		e.printStackTrace(System.err);
    }
  }
%}

/* JavaDoc for Call_Scilab class */
%pragma(java) moduleclassmodifiers="
import org.scilab.modules.types.scilabTypes.ScilabTypeEnum;

 /**
   * Connector for Javasci v2
   * @author Sylvestre LEDRU
   * @copyright DIGITEO
   */
public class";

/* Constructor for Call_Scilab class */
%pragma(java) modulecode="
 /**
   * Constructor
   */
 protected Call_Scilab() {
	throw new UnsupportedOperationException();
 }";

#endif


BOOL StartScilab(char *SCIpath, char *ScilabStartup, int *Stacksize);

BOOL TerminateScilab(char *ScilabQuit);

int SendScilabJob(char *job);

int SendScilabJobs(char **jobs,int numberjobs);

int ScilabHaveAGraph(void);

int GetLastErrorCode(void);

//int getVariableType(char *varname);

sci_types getVariableType(char *varname);

//int putInteger(char *varname, int variable[][]);

//int putInteger(char * variableName, int **variable, int nbRow, int nbCol);

// This position matters. It will apply only to the following lines
%include "call_scilab_java_typemaps.i"

double * getDouble(char *variableName, int *nbRow, int *nbCol);

int putDouble(char * variableName, double variable[], int nbRow, int nbCol);

BOOL * getBoolean(char *variableName, int *nbRow, int *nbCol);

int putBoolean(char * variableName, BOOL variable[], int nbRow, int nbCol);

int putDoubleComplex(char * variableName, double variable[], double variableImg[], int nbRow, int nbCol);
