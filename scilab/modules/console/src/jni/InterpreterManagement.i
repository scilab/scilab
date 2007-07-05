/* InterpreterManagement.i */
/*  swig -java -package org.scilab.modules.console -outdir ../java/org/scilab/modules/console/ InterpreterManagement.i */
%module InterpreterManagement
%{
#include "../c/InterpreterManagement.h"
%}

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
";
int putCommandInScilabQueue(char *command);