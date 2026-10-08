/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
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

/* imports needed by the JNI class */
%pragma(java) jniclassimports=%{
import org.scilab.modules.localization.NativeLibraryLoader;
%}

/* static load of library */
%pragma(java) jniclasscode=%{
  static {
    try {
        NativeLibraryLoader.loadLibrary("sciaction_binding");
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
%javamethodmodifiers putCallbackInScilabQueue(char *command) "
 /**
   * Put the callback of a user interface in Scilab command queue.
   * It is debugged when Scilab is in debug mode; while the debugger is stopped,
   * it runs at once, not debugged, as a line typed in the console.
   *
   * @param command the command to execute
   * @return execution status
   */
public ";
int putCallbackInScilabQueue(char *command);

/* JavaDoc */
%javamethodmodifiers requestCallbackExec(char *command) "
 /**
   * Same as putCallbackInScilabQueue, for a callback executed as soon as possible
   * and that may not be interrupted by another one.
   *
   * @param command the command to execute
   * @return execution status
   */
public ";
int requestCallbackExec(char *command);

/* JavaDoc */
%javamethodmodifiers callScilabExecstr(const char *command) "
 /**
   * call the command, directly in the interpreter. This only works if the Scilab execution thread is running.
   *
   * @param command the command to execute
   * @return execution status (0 if the command is valid and has been executed to its end)
   */
public ";
int callScilabExecstr(const char *command);

/* JavaDoc */
%javamethodmodifiers interruptScilab(void) "
 /**
  * Stop Scilab current work
  * @return execution status
  */
public ";
int interruptScilab(void);

/* JavaDoc */
%javamethodmodifiers startScilabOutputCapture(int silent) "
 /**
  * Start capturing all Scilab output (stdout + stderr).
  * @param silent if non-zero, output is captured only (not forwarded to the console)
  */
public ";
void startScilabOutputCapture(int silent);

/* JavaDoc */
%javamethodmodifiers stopScilabOutputCapture(void) "
 /**
  * Stop capturing output and return the captured text.
  * @return captured output as a string, or null
  */
public ";
char* stopScilabOutputCapture(void);

/* JavaDoc */
%javamethodmodifiers getScilabDebugState(void) "
 /**
  * What a user interface shows of the debugger
  * @return 0 outside debug mode, 1 in debug mode with nothing running, 2 while a debugged execution runs, 3 while it is stopped
  */
public ";
int getScilabDebugState(void);
