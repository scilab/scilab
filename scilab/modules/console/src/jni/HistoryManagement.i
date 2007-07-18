/* HistoryManagement.i */
/* To run this file execute the command below from current directory */
/*  swig -java -package org.scilab.modules.console -outdir ../java/org/scilab/modules/console/ HistoryManagement.i */

%include <windows.i>

%module HistoryManagement
%{
#include "../../../shell/includes/HistoryManager.h"
%}

/* JavaDoc for HistoryManagementJNI class */
%pragma(java) jniclassclassmodifiers=%{
 /** 
   * All Scilab history management used in Java console 
   * @author Vincent COUVERT
   * @copyright INRIA 
   */
class%}

/* Constructor for HistoryManagementJNI class */
%pragma(java) jniclasscode="
 /**
   * Constructor
   */
 protected HistoryManagementJNI() {
        throw new UnsupportedOperationException();
 }";

/* JavaDoc for HistoryManagement class */
%pragma(java) moduleclassmodifiers="
 /** 
   * All Scilab history management used in Java console 
   * @author Vincent COUVERT
   * @copyright INRIA 
   */
public class";

/* Constructor for HistoryManagement class */
%pragma(java) modulecode="
  /**
    * Constructor
    */
  protected HistoryManagement() {
	throw new UnsupportedOperationException();
  }";

/* JavaDoc */
%javamethodmodifiers getSearchedTokenInScilabHistory(void) "
 /**
   * Gets the token used as the beginning of the search entry
   * @return the token
   */
";
char *getSearchedTokenInScilabHistory(void);

/* JavaDoc */
%javamethodmodifiers setSearchedTokenInScilabHistory(char *token) "
 /**
   * Sets the token used as the beginning of the search entry
   * @param token the token to used for next search
   */
";
void setSearchedTokenInScilabHistory(char *token);

/* JavaDoc */
%javamethodmodifiers getNextLineInScilabHistory(void) "
 /**
   * Gets next entry in Scilab history beginning with a user input
   * @return next matching entry or null if no entry found
   */
";
char *getNextLineInScilabHistory(void);

/* JavaDoc */
%javamethodmodifiers getPreviousLineInScilabHistory(void) "
 /***
   * Gets previous entry in Scilab history beginning with a user input
   * @return previous matching entry or null if no entry found
   */
";
char *getPreviousLineInScilabHistory(void);

/* JavaDoc */
%javamethodmodifiers appendLineToScilabHistory(char *line) "
 /**
   * Adds an entry to Scilab history
   * @param line the line to add to Scilab history
   * @return status flag indicating if the operation failed or not
   */
";
BOOL appendLineToScilabHistory(char *line);

/* JavaDoc */
%javamethodmodifiers displayScilabHistory(void) "
 /**
   * Displays Scilab history
   */
";
void displayScilabHistory(void);

/* JavaDoc */
%javamethodmodifiers getNthLineInScilabHistory(int n) "
 /**
  * Get the Nth Line in history
  * @param n the number of the line
  * @return the Nth Line
  */
";
char *getNthLineInScilabHistory(int n);
