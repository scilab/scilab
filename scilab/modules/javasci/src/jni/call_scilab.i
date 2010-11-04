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
%{
#include "BOOL.h"
#define ENABLE_HELPERS
#include "javasci2_helper.h"
#include "MALLOC.h"
#include "../../../call_scilab/includes/call_scilab.h"
#include "../../../call_scilab/includes/fromjava.h"
#include "../../../api_scilab/includes/api_scilab.h"
#include "../../../output_stream/includes/lasterror.h"
#include "../../../modules/graphics/includes/WindowList.h"
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
   * Connector for Javasci v2.
   * This class should <b>not</b> be used directly
   * @see org.scilab.modules.javasci.Scilab
   * @author DIGITEO - Sylvestre LEDRU
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
    if (System.getProperty("os.name").toLowerCase().contains("windows")) {
        System.loadLibrary("javasci");
    } else {
        System.loadLibrary("scilab");
        System.loadLibrary("javasci2");
        }
    } catch (SecurityException e) {
        System.err.println("A security manager exists and does not allow the loading of the specified dynamic library :");
        e.printStackTrace(System.err);
    } catch (UnsatisfiedLinkError e)    {
        System.err.println("The native library javasci does not exist or cannot be found.");
        e.printStackTrace(System.err);
    }
  }
%}

/* JavaDoc for Call_Scilab class */
%pragma(java) moduleclassmodifiers="
import org.scilab.modules.types.ScilabTypeEnum;
import org.scilab.modules.types.ScilabIntegerTypeEnum;

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
 protected Call_Scilab() {
    throw new UnsupportedOperationException();
 }";

#endif


int Call_ScilabOpen(char* SCIpath, BOOL advancedMode, char *ScilabStartup, int Stacksize);

BOOL TerminateScilab(char *ScilabQuit);

int SendScilabJob(char *job);

int SendScilabJobs(char **jobs,int numberjobs);

// Direct access to the Scilab function (no helper)
void SetFromJavaToON(void);

// Direct access to the Scilab function (no helper)
%rename(isGraphicOpened) sciHasFigures;
BOOL sciHasFigures( void );

// Direct access to the Scilab function (no helper)
%rename(GetLastErrorCode) getLastErrorValue;
int getLastErrorValue(void);

BOOL isExistingVariable(char* varname);

%rename(getLastErrorMessage) getLastErrorMessageSingle;
const char* getLastErrorMessageSingle();

sci_types getVariableType(char *varname);

sci_int_types getIntegerPrecision(char* varname);

%rename(isComplex) isComplexVar;
BOOL isComplexVar(char* varname);

%include "call_scilab_java_typemaps_string.i"
// string
char ** getString(char* variableName, int *nbRow, int *nbCol);
int putString(char* variableName, char **variable, int nbRow, int nbCol);


%include "arrays_java.i"
int putDoubleComplex(char * variableName, double variable[], int nbRow, int nbCol);


// This position matters. It will apply only to the following lines
%include "call_scilab_java_typemaps.i"


// double (default Scilab type)
double * getDouble(char *variableName, int *nbRow, int *nbCol);
int putDouble(char * variableName, double variable[], int nbRow, int nbCol);


// boolean (%t / %f)
BOOL * getBoolean(char *variableName, int *nbRow, int *nbCol);
int putBoolean(char * variableName, BOOL variable[], int nbRow, int nbCol);

// byte/char = int8
byte * getByte(char *variableName, int *nbRow, int *nbCol);
int putByte(char * variableName, byte variable[], int nbRow, int nbCol);

byte * getUnsignedByte(char *variableName, int *nbRow, int *nbCol);
int putUnsignedByte(char * variableName, byte variable[], int nbRow, int nbCol);

// short = int16
short * getShort(char *variableName, int *nbRow, int *nbCol);
int putShort(char * variableName, short variable[], int nbRow, int nbCol);

unsigned short * getUnsignedShort(char *variableName, int *nbRow, int *nbCol);
int putUnsignedShort(char * variableName, unsigned short variable[], int nbRow, int nbCol);

// int = int32
int * getInt(char *variableName, int *nbRow, int *nbCol);
int putInt(char * variableName, int variable[], int nbRow, int nbCol);

unsigned int * getUnsignedInt(char *variableName, int *nbRow, int *nbCol);
int putUnsignedInt(char * variableName, unsigned int variable[], int nbRow, int nbCol);

#ifdef __SCILAB_INT64__
// long = int64
long * getLong(char *variableName, int *nbRow, int *nbCol);
int putLong(char * variableName, long variable[], int nbRow, int nbCol);

unsigned long * getUnsignedLong(char *variableName, int *nbRow, int *nbCol);
int putUnsignedLong(char * variableName, unsigned long variable[], int nbRow, int nbCol);
#endif

%include "call_scilab_java_typemaps_complex.i"
// Complex
double * getDoubleComplexReal(char *variableName, int *nbRow, int *nbCol);
double * getDoubleComplexImg(char * variableName, int *nbRow, int *nbCol);
