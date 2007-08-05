/* Allan CORNET */
/* INRIA 2007 */
/* HistoryManagement.i */
/* To run this file execute the command below from current directory */
/*  swig -java -package org.scilab.modules.history_manager -outdir ../java/org/scilab/modules/history_manager/ HistoryManagement.i */

%include <windows.i>

%module HistoryManagement
%{
#include "machine.h"
#include "MALLOC.h"
#include "../../includes/HistoryManager.h"
%}

/* String[] <--> char ** support */

%typemap(in) char ** (jint size) {
    int i = 0;
    size = (*jenv)->GetArrayLength(jenv, $input);
    $1 = (char **) MALLOC((size+1)*sizeof(char *));
    /* make a copy of each string */
    for (i = 0; i<size; i++) {
        jstring j_string = (jstring)(*jenv)->GetObjectArrayElement(jenv, $input, i);
        const char * c_string = (*jenv)->GetStringUTFChars(jenv, j_string, 0);
        $1[i] = MALLOC(strlen((c_string)+1)*sizeof(const char *));
        strcpy($1[i], c_string);
        (*jenv)->ReleaseStringUTFChars(jenv, j_string, c_string);
        (*jenv)->DeleteLocalRef(jenv, j_string);
    }
    $1[i] = 0;
}

/* This cleans up the memory we malloc'd before the function call */
%typemap(freearg) char ** {
    int i;
    for (i=0; i<size$argnum-1; i++)
      FREE($1[i]);
    FREE($1);
}

/* This allows a C function to return a char ** as a Java String array */
%typemap(out) char ** {
    int i;
    int len=0;
    jstring temp_string;
    const jclass clazz = (*jenv)->FindClass(jenv, "java/lang/String");

    while ($1[len]) len++;    
    jresult = (*jenv)->NewObjectArray(jenv, len, clazz, NULL);
    /* exception checking omitted */

    for (i=0; i<len; i++) {
      temp_string = (*jenv)->NewStringUTF(jenv, *result++);
      (*jenv)->SetObjectArrayElement(jenv, jresult, i, temp_string);
      (*jenv)->DeleteLocalRef(jenv, temp_string);
    }
}

/* These 3 typemaps tell SWIG what JNI and Java types to use */
%typemap(jni) char ** "jobjectArray"
%typemap(jtype) char ** "String[]"
%typemap(jstype) char ** "String[]"


/* These 2 typemaps handle the conversion of the jtype to jstype typemap type
   and visa versa */
%typemap(javain) char ** "$javainput"
%typemap(javaout) char ** {
    return $jnicall;
  }

/* String[] <--> char ** support */


/* boolean <--> BOOL support */

%typemap(in) BOOL {
	if ($input == JNI_TRUE) $1 = TRUE;
	else $1 = FALSE;
}

%typemap(out) BOOL {
 if ($1) $result = JNI_TRUE   ;
 else  $result = JNI_FALSE   ;
}

/* This typemap tell SWIG what JNI and Java types to use */
%typemap(jtype) BOOL "boolean"
%typemap(jstype) BOOL "boolean"
%typemap(jni) BOOL "jboolean"

/* boolean <--> BOOL support */



/* JavaDoc for HistoryManagementJNI class */
%pragma(java) jniclassclassmodifiers=%{
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
		System.err.println("A security manager exists and does not allow the loading of the specified dynamic library :");
		e.printStackTrace(System.err);
	} catch (UnsatisfiedLinkError e)	{
		System.err.println("The native library scihistory_manager does not exist or cannot be found.");
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


/* JavaDoc */
%javamethodmodifiers getSearchedTokenInScilabHistory(void) "
 /**
   * Gets the token used as the beginning of the search entry
   * @return the token
   */
public";
 char *getSearchedTokenInScilabHistory(void);


%javamethodmodifiers appendLineToScilabHistory(char *line) "
	/**
	* add a line to History manager
	* @param a line to add
	* line isn't added if it is the same as previous (false)
	* @return true or false
	*/
public";
	BOOL appendLineToScilabHistory(char *line);

%javamethodmodifiers appendLinesToScilabHistory(char **lines,int numberoflines) "
	/**
	* append lines to History manager
	* @param array of string
	* @param size of the array of string
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
	* @param a filename if NULL saves in default filename
	* default filename --> SCIHOME/history.scilab
	* @return true or false
	*/
public";
	BOOL writeScilabHistoryToFile(char *filename);

%javamethodmodifiers loadScilabHistoryFromFile(char *filename) "
	/**
	* load history from a file
	* @param a filename if NULL load from default filename
	* default filename --> SCIHOME/history.scilab
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
	* @return a array of string
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
	* after a appendLine iterator go to end
	*/
public";
	char *getPreviousLineInScilabHistory(void);

%javamethodmodifiers getNextLineInScilabHistory(void) "
	/**
	* Get the next line in Scilab history
	* @return a line or NULL
	* after a appendLine iterator go to end
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

%javamethodmodifiers setSaveConsecutiveDuplicateLinesInScilabHistory(boolean doit) "
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
	* @param number between 0 and Max
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
	* @param N
	* @return the Nth Line
	*/
public";
	char *getNthLineInScilabHistory(int N);

%javamethodmodifiers deleteNthLineScilabHistory(int N) "
	/**
	* delete the Nth Line in history
	* @param N
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
	


