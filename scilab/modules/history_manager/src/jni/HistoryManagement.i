/* Allan CORNET */
/* INRIA 2007 */
/* HistoryManagement.i */
/* To run this file execute the command below from current directory */
/**
 * Windows: swig -java -package org.scilab.modules.history_manager -outdir ../java/org/scilab/modules/history_manager/ HistoryManagement.i
 * Other: Use the option --enable-build-swig to the configure
*/

%module HistoryManagement
%{
#include "sci_malloc.h"
#include "../../includes/HistoryManager.h"
#include "../../includes/TerminateHistoryManager.h"
#include "../../includes/InitializeHistoryManager.h"
%}

%include "../../../jvm/src/jni/scilab_typemaps.i"

/* JavaDoc for HistoryManagementJNI class */
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

/* Constructor for HistoryManagementJNI class */
%pragma(java) jniclasscode="
 /**
   * Constructor
   */
 protected HistoryManagementJNI() {
        throw new UnsupportedOperationException();
 }";

 /* static load of library */
%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("scihistory_manager");
    } catch (SecurityException e) {
        System.err.println("A security manager exists and does not allow the loading of the specified dynamic library.");
        System.err.println(e.getLocalizedMessage());
        e.printStackTrace(System.err);
    } catch (UnsatisfiedLinkError e)    {
        System.err.println("The native library scihistory_manager does not exist or cannot be found.");
        System.err.println(e.getLocalizedMessage());
        e.printStackTrace(System.err);
    }
  }
%}

/* JavaDoc for HistoryManagement class */
%pragma(java) moduleclassmodifiers="
 /**
   * All Scilab history management used in Java console
   * @author Vincent COUVERT
   * @author Allan CORNET
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


%javamethodmodifiers appendLineToScilabHistory(char *line) "
    /**
    * add a line to History manager
    * @param line a line to add
    * line isn't added if it is the same as previous (false)
    * @return true or false
    */
public";
    BOOL appendLineToScilabHistory(char *line);

%javamethodmodifiers appendLinesToScilabHistory(char **lines,int numberoflines) "
    /**
    * append lines to History manager
    * @param lines array of string
    * @param numberoflines size of the array of string
    * @return true or false
    */
public";
    BOOL appendLinesToScilabHistory(char **lines,int numberoflines);

%javamethodmodifiers displayScilabHistory(void) "
    /**
    * Display history
    */
public";
    void displayScilabHistory(void);

%javamethodmodifiers writeScilabHistoryToFile(char *filename) "
    /**
    * save history in a file
    * @param filename if NULL saves in default filename
    * @return true or false
    */
public";
    BOOL writeScilabHistoryToFile(char *filename);

%javamethodmodifiers loadScilabHistoryFromFile(char *filename) "
    /**
    * load history from a file
    * @param filename if NULL load from default filename
    * @return true or false
    */
public";
    BOOL loadScilabHistoryFromFile(char *filename);

%javamethodmodifiers setFilenameScilabHistory(char *filename) "
    /**
    * set filename of history
    * @param filename of history
    * @return true or false
    */
public";
    BOOL setFilenameScilabHistory(char *filename);

%javamethodmodifiers getFilenameScilabHistory(void) "
    /**
    * get filename of history
    * @return a filename
    */
public";
    char *getFilenameScilabHistory(void);

%javamethodmodifiers setDefaultFilenameScilabHistory(void) "
    /**
    * set default filename of history
    * @return true or false
    */
public";
    BOOL setDefaultFilenameScilabHistory(void);

%javamethodmodifiers resetScilabHistory(void) "
    /**
    * reset history manager
    */
public";
    void resetScilabHistory(void);

%javamethodmodifiers getAllLinesOfScilabHistory(void) "
    /**
    * Get all lines in history
    * @return an array of strings
    */
public";
    char **getAllLinesOfScilabHistory(void);

%javamethodmodifiers getSizeAllLinesOfScilabHistory(void) "
    /**
    * Get numbers of lines in history
    * size of array returned by getAllLinesOfScilabHistory
    * @return int
    */
public";
    int getSizeAllLinesOfScilabHistory(void);


%javamethodmodifiers setSearchedTokenInScilabHistory(char *token) "
    /**
    * set new token to search in history
    * @param token (a string)
    * @return true or false
    */
public";
    BOOL setSearchedTokenInScilabHistory(char *token);

%javamethodmodifiers resetSearchedTokenInScilabHistory(void) "
    /**
    * reset search in history
    * @return true or false
    */
public";
    BOOL resetSearchedTokenInScilabHistory(void);

%javamethodmodifiers getSearchedTokenInScilabHistory(void) "
    /**
    * get token searched in history
    * @return token (a string)
    */
public";
    char *getSearchedTokenInScilabHistory(void);

%javamethodmodifiers getPreviousLineInScilabHistory(void) "
    /**
    * Get the previous line in Scilab history
    * @return a line or NULL
    * after an appendLine iterator go to end
    */
public";
    char *getPreviousLineInScilabHistory(void);

%javamethodmodifiers getNextLineInScilabHistory(void) "
    /**
    * Get the next line in Scilab history
    * @return a line or NULL
    * after an appendLine iterator go to end
    */
public";
    char *getNextLineInScilabHistory(void);

%javamethodmodifiers getNumberOfLinesInScilabHistory(void) "
    /**
    * Get numbers of lines in history
    * @return number of lines
    */
public";
    int getNumberOfLinesInScilabHistory(void);

%javamethodmodifiers setSaveConsecutiveDuplicateLinesInScilabHistory(BOOL doit) "
    /**
    * set consecutive duplicate lines are added
    * @param doit (true or false)
    */
public";
    void setSaveConsecutiveDuplicateLinesInScilabHistory(BOOL doit);

%javamethodmodifiers getSaveConsecutiveDuplicateLinesInScilabHistory(void) "
    /**
    * indicate if consecutive duplicate lines are added
    * @return true or false
    */
public";
    BOOL getSaveConsecutiveDuplicateLinesInScilabHistory(void);

%javamethodmodifiers setAfterHowManyLinesScilabHistoryIsSaved(int num) "
    /**
    * Set after how many lines history is saved
    * @param num number between 0 and Max
    * default value is 0 (disabled)
    */
public";
    void setAfterHowManyLinesScilabHistoryIsSaved(int num);

%javamethodmodifiers getAfterHowManyLinesScilabHistoryIsSaved(void) "
    /**
    * Get after how many lines history is saved
    * @return number between 0 and Max
    * default value is 0 (disabled)
    */
public";
    int getAfterHowManyLinesScilabHistoryIsSaved(void);

%javamethodmodifiers getNthLineInScilabHistory(int N) "
    /**
    * Get the Nth Line in history
    * @param N the number of the line to get
    * @return the Nth Line
    */
public";
    char *getNthLineInScilabHistory(int N);

%javamethodmodifiers deleteNthLineScilabHistory(int N) "
    /**
    * delete the Nth Line in history
    * @param N the number of the line to delete
    * @return true or false
    */
public";
    BOOL deleteNthLineScilabHistory(int N);

%javamethodmodifiers getSizeScilabHistory(void) "
    /**
    * get number of lines of history
    * @return a number >= 0
    */
public";
    int getSizeScilabHistory(void);

%javamethodmodifiers historyIsEnabled(void) "
    /**
    * get history manager state
    * @return true or false
    */
public";
    BOOL historyIsEnabled(void);

%javamethodmodifiers setSizeMaxScilabHistory(int N)"
    /**
    * set the number of max lines
    * @return true or false
    */
public";
    BOOL setSizeMaxScilabHistory(int nbLinesMax);

%javamethodmodifiers InitializeHistoryManager(void)"
    /**
    * terminate the history manager
    * @return true or false
    */
public";
    BOOL InitializeHistoryManager(void);

%javamethodmodifiers TerminateHistoryManager(void)"
    /**
    * terminate the history manager
    * @return true or false
    */
public";
    BOOL TerminateHistoryManager(void);
