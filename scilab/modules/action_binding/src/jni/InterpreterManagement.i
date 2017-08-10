/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA
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

/* InterpreterManagement.i */
/**
 * Windows: swig -java -package org.scilab.modules.action_binding -outdir ../java/org/scilab/modules/action_binding/ InterpreterManagement.i 
 * Other: Use the option --enable-build-swig to the configure
*/
%module InterpreterManagement
%{
#include "../c/InterpreterManagement.h"
%}

%include "../../../jvm/src/jni/scilab_typemaps.i"

/* JavaDoc for InterpreterManagementJNI class */
%pragma(java) jniclassclassmodifiers=%{
/* It is generated code. Disable checkstyle */
//CHECKSTYLE:OFF
 /**
   * All Scilab interpreter management used in Java console
   * @author Vincent COUVERT
   * @copyright INRIA
   */
class%}

/* Constructor for InterpreterManagementJNI class */
%pragma(java) jniclasscode="
  /**
    * Constructor
    */
  protected InterpreterManagementJNI() {
    throw new UnsupportedOperationException();
  }";

/* static load of library */
%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("sciaction_binding");
    } catch (SecurityException e) {
        System.err.println("A security manager exists and does not allow the loading of the specified dynamic library.");
        System.err.println(e.getLocalizedMessage());
        e.printStackTrace(System.err);
    } catch (UnsatisfiedLinkError e)    {
        System.err.println("The native library sciaction_binding does not exist or cannot be found.");
        System.err.println(e.getLocalizedMessage());
        e.printStackTrace(System.err);
    }
  }
%}

/* JavaDoc for InterpreterManagement class */
%pragma(java) moduleclassmodifiers="
 /**
   * All Scilab interpreter management used in Java console
   * @author Vincent COUVERT
   * @copyright INRIA
   */
public class";

/* Constructor for InterpreterManagement class */
%pragma(java) modulecode="
 /**
   * Constructor
   */
 protected InterpreterManagement() {
    throw new UnsupportedOperationException();
 }";

/* JavaDoc */
%javamethodmodifiers putCommandInScilabQueue(char *command) "
 /**
   * Put a command in Scilab command queue so that Scilab executes it
   * The command is displayed in Scilab Window
   *
   * @param command the command to execute
   * @return execution status
   */
public ";
int putCommandInScilabQueue(char *command);

/* JavaDoc */
%javamethodmodifiers requestScilabExec(char *command) "
 /**
   * Put a command in Scilab command queue so that Scilab executes it
   * The command is executed as soon as possible and may not be interrupted by another one.
   * WARNING : if the command is taking some time, scilab will not do anything else
   * before the command returns.
   *
   * @param command the command to execute
   * @return execution status
   */
public ";
int requestScilabExec(char *command);

/* JavaDoc */
%javamethodmodifiers interruptScilab(void) "
 /**
  * Stop Scilab current work
  * @return execution status
  */
public ";
int interruptScilab(void);
