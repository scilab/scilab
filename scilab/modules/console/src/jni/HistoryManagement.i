/* HistoryManagement.i */
/* To run this file execute the command below from current directory */
/*  swig -java -outdir ../java/org/scilab/modules/console/ HistoryManagement.i */
%module HistoryManagement
%{
#include "../c/HistoryManagement.h"
%}

/* JavaDoc for HistoryManagementJNI class */
%pragma(java) jniclassclassmodifiers=%{
package org.scilab.modules.console;


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
package org.scilab.modules.console;


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
%javamethodmodifiers getNextEntry(char *beg) "
 /**
   * Gets next entry in Scilab history beginning with a user input
   * @param beg the beginning of the entry we are looking for
   * @return next matching entry or null if no entry found
   */
";
char *getNextEntry(char *beg);

/* JavaDoc */
%javamethodmodifiers getPreviousEntry(char *beg) "
 /**
   * Gets previous entry in Scilab history beginning with a user input
   * @param beg the beginning of the entry we are looking for
   * @return previous matching entry or null if no entry found
   */
";
char *getPreviousEntry(char *beg);

/* JavaDoc */
%javamethodmodifiers setNewSearchInHistory(int state) "
 /**
   * Resets all pointers to history initilized by a previous history search
   * @param state if true then all pointers are reset
   */
";
void setNewSearchInHistory(int state);

/* JavaDoc */
%javamethodmodifiers searchBackward(char *line) "
 /**
   * Gets previous entry in Scilab history beginning with a user input
   * (used for history browsing based on user input beginning with a !)
   * @param line the beginning of the entry we are looking for
   * @return previous matching entry or null if no entry found
   */
";
char *searchBackward(char *line);

/* JavaDoc */
%javamethodmodifiers AddHistory(char *line) "
 /**
   * Adds an entry to Scilab history
   * @param line the line to add to Scilab history
   */
";
void AddHistory(char *line);

