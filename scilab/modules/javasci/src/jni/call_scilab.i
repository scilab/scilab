
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
 * Copyright (C) 2011 - Scilab Enterprises - Sylvestre LEDRU
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
#include "sci_malloc.h"
#include "call_scilab.h"
#include "fromjava.h"
#include "api_scilab.h"
#include "lasterror.h"
#include "FigureList.h"
#include "sci_types.h"
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

  public final static native int putList(String varNmae, Object list, char type);
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
%rename(GetLastErrorCode) getLastErrorNumber;
int getLastErrorNumber(void);

BOOL isExistingVariable(char* varname);

%rename(getLastErrorMessage) getLastErrorMessage;
const char* getLastErrorMessage();

sci_types getVariableType(char *varname);

sci_int_types getIntegerPrecision(char* varname);

%rename(isComplex) isComplexVar;
BOOL isComplexVar(char* varname);

%include "call_scilab_java_typemaps_string.i"
// string
int putString(char* variableName, char **variable, int nbRow, int nbCol);

// This position matters. It will apply only to the following lines
%include "call_scilab_java_typemaps.i"

int putDoubleComplex(char * variableName, double variable[], int nbRow, int nbCol, double imag[], int nbRowI, int nbColI);

// double (default Scilab type)
int putDouble(char * variableName, double variable[], int nbRow, int nbCol);

// boolean (%t / %f)
int putBoolean(char * variableName, BOOL variable[], int nbRow, int nbCol);

// byte/char = int8
int putByte(char * variableName, byte variable[], int nbRow, int nbCol);

int putUnsignedByte(char * variableName, byte variable[], int nbRow, int nbCol);

// short = int16
int putShort(char * variableName, short variable[], int nbRow, int nbCol);

int putUnsignedShort(char * variableName, unsigned short variable[], int nbRow, int nbCol);

// int = int32
int putInt(char * variableName, int variable[], int nbRow, int nbCol);

int putUnsignedInt(char * variableName, unsigned int variable[], int nbRow, int nbCol);

#ifdef __SCILAB_INT64__
// long = int64
int putLong(char * variableName, long variable[], int nbRow, int nbCol);

int putUnsignedLong(char * variableName, unsigned long variable[], int nbRow, int nbCol);
#endif

// This position matters. It will apply only to the following lines
%include "call_scilab_java_typemaps_sparse.i"

int putSparse(char * variableName, int nbRow, int nbCol, int * nbRowItem, int nbRowItemL, int * colPos, int colPosL, double * data, int dataL);

int putComplexSparse(char * variableName, int nbRow, int nbCol, int * nbRowItem, int nbRowItemL, int * colPos, int colPosL, double * data, int dataL, double * imag, int imagL);

int putBooleanSparse(char * variableName, int nbRow, int nbCol, int * nbRowItem, int nbRowItemL, int * colPos, int colPosL);

// This position matters. It will apply only to the following lines
%include "call_scilab_java_typemaps_poly.i"

int putPolynomial(char * variableName, char * polyVarName, double ** data, int nbRow, int nbCol, int * nbCoef);
int putComplexPolynomial(char * variableName, char * polyVarName, double ** data, int nbRow, int nbCol, int * nbCoef, double ** imag, int nbRowI, int nbColI, int * nbCoefI);
