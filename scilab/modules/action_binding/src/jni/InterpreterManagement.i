/* InterpreterManagement.i */
/*  swig -java -package org.scilab.modules.console -outdir ../java/org/scilab/modules/console/ InterpreterManagement.i */
%module InterpreterManagement
%{
#include "../c/InterpreterManagement.h"
%}

%include "../../../jvm/src/jni/scilab_typemaps.i"

/* JavaDoc for InterpreterManagementJNI class */
%pragma(java) jniclassclassmodifiers=%{
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
        System.loadLibrary("sciconsole");
    } catch (SecurityException e) {
		System.err.println("A security manager exists and does not allow the loading of the specified dynamic library :");
		e.printStackTrace(System.err);
	} catch (UnsatisfiedLinkError e)	{
		System.err.println("The native library sciconsole does not exist or cannot be found.");
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
%javamethodmodifiers execScilabEventLoop(void) "
 /**
  * Executes Scilab event loop so that queued callbacks can be executed
  * @return execution status
  */
public ";
int execScilabEventLoop(void);

/* JavaDoc */
%javamethodmodifiers interruptScilab(void) "
 /**
  * Stop Scilab current work
  * @return execution status
  */
public ";
int interruptScilab(void);

/* JavaDoc */
%javamethodmodifiers haveCommandsInTheQueue(void) "
 /**
  * Are there some commands in the queue to be executed
  * @return true if there are commands in the queue
  */
public ";
BOOL haveCommandsInTheQueue(void);
