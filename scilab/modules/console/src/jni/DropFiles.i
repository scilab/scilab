/* Allan CORNET */
/* INRIA 2007 */
/* Completion.i */
/** 
 * Windows: swig -java -package org.scilab.modules.console -outdir ../java/org/scilab/modules/console/ DropFiles.i 
 * Other: Use the option --enable-build-swig to the configure
*/

%module DropFiles

%include "../../../jvm/src/jni/scilab_typemaps.i"

%{
#include "../c/dropFiles.h"
#include "machine.h"
#include "sci_malloc.h"
%}

/* JavaDoc for DropFilesJNI class */
%pragma(java) jniclassclassmodifiers=%{
/* It is generated code. Disable checkstyle */
//CHECKSTYLE:OFF
 /** 
   * @author Allan CORNET
   * @copyright INRIA 2007
   */
public class%}

/* Constructor for DropFilesJNI class */
%pragma(java) jniclasscode="
  /**
    * Constructor
    */
  protected DropFilesJNI() {
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
    } catch (UnsatisfiedLinkError e) {
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

/* JavaDoc for DropFiles class */
%pragma(java) moduleclassmodifiers="
 /** 
   * @author Allan CORNET
   * @copyright INRIA 2007
   */
public class";

/* Constructor for DropFiles class */
%pragma(java) modulecode="
 /**
   * Constructor
   */
 protected DropFiles() {
    throw new UnsupportedOperationException();
 }";

/* JavaDoc */
%javamethodmodifiers dropFiles(char **files) "
/**
* drop function on all types for scilab
* @param files list of files
* @return true or false
*/
public";
BOOL dropFiles(char **files);
