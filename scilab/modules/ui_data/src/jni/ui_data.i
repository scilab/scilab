/* Allan CORNET */
/* INRIA 2007 */
/* HistoryManagement.i */
/* To run this file execute the command below from current directory */
/**
 * Windows: swig -java -package org.scilab.modules.ui_data -outdir ../java/org/scilab/modules/ui_data/ ui_data.i 
 * Other: Use the option --enable-build-swig to the configure
*/

%module UI_data
%{
#include "sci_malloc.h"
#include "../../includes/ui_data.h"
%}

%include "../../../jvm/src/jni/scilab_typemaps.i"

/* JavaDoc for UI_dataJNI class */
%pragma(java) jniclassclassmodifiers=%{
/* It is generated code. Disable checkstyle */
//CHECKSTYLE:OFF
 /** 
   * All Scilab history management used in Java console 
   * @author Vincent COUVERT
   * @author Allan CORNET
   * @copyright INRIA 
   */
class%}

/* Constructor for UI_dataJNI class */
%pragma(java) jniclasscode="
 /**
   * Constructor
   */
 protected UI_dataJNI() {
        throw new UnsupportedOperationException();
 }";
 
 /* static load of library */
%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("sciui_data");
    } catch (SecurityException e) {
        System.err.println("A security manager exists and does not allow the loading of the specified dynamic library.");
        System.err.println(e.getLocalizedMessage());
        e.printStackTrace(System.err);
    } catch (UnsatisfiedLinkError e)    {
        System.err.println("The native library sciui_data does not exist or cannot be found.");
        System.err.println(e.getLocalizedMessage());
        e.printStackTrace(System.err);
    }
  }
%}

/* JavaDoc for UI_data class */
%pragma(java) moduleclassmodifiers="
 /** 
   * ui_data management 
   * @author Vincent COUVERT
   * @author Allan CORNET
   * @copyright INRIA 
   */
public class";

/* Constructor for UI_data class */
%pragma(java) modulecode="
  /**
    * Constructor
    */
  protected UI_data() {
    throw new UnsupportedOperationException();
  }";


%javamethodmodifiers putScilabVariable(char * name, char ** lines, int rows, int cols) "
    /**
    * Put a scilab variable as an array of strings in C envrionment
    * @param lines array of string
    * @param rows the number of rows
    * @param cols the number of cols
    */
public";
void putScilabVariable(char * name, char ** lines, int rows, int cols);

%javamethodmodifiers getUnnamedVariable() "
    /**
    * Put a scilab variable as an array of strings in C envrionment
    * @param lines array of string
    * @param rows the number of rows
    * @param cols the number of cols
    */
public";
char * getUnnamedVariable();
