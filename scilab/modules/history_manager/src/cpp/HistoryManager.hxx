/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
#ifndef __HISTORYMANAGER_CPP_H__
#define __HISTORYMANAGER_CPP_H__
/*------------------------------------------------------------------------*/
#ifdef __cplusplus
/*------------------------------------------------------------------------*/
extern "C"
{
#include <time.h>
};
/*------------------------------------------------------------------------*/
#include <list>
#include "CommandLine.hxx"
#include "HistoryFile.hxx"
#include "HistorySearch.hxx"
using namespace std;
/*------------------------------------------------------------------------*/
class HistoryManager
{
public:
    /**
    * Constructor
    */
    HistoryManager();

    /**
    * Destructor
    */
    ~HistoryManager();

    /*
    * add a line to History manager
    * @param a line to add
    * line isn't added if it is the same as previous (FALSE)
    * @return TRUE or FALSE
    */
    BOOL appendLine(char *cline);

    /**
    * append lines to History manager
    * @param array of string
    * @param size of the array of string
    * @return TRUE or FALSE
    */
    BOOL appendLines(char **lines,int nbrlines);

    /**
    * Display history
    */
    void displayHistory(void);

    /**
    * get filename of history
    * @return a filename
    */
    char *getFilename(void);

    /**
    * set filename of history
    * @param filename of history
    * @return TRUE or FALSE
    */
    void setFilename(char *filename);

    /**
    * set default filename of history
    * @return TRUE or FALSE
    */
    BOOL setDefaultFilename(void);

    /**
    * save history in a file
    * @param a filename if NULL saves in default filename
    * default filename --> SCIHOME/history.scilab
    * @return TRUE or FALSE
    */
    BOOL writeToFile(char *filename);

    /**
    * load history from a file
    * @param a filename if NULL load from default filename
    * default filename --> SCIHOME/<scilab version>history.scilab
    * @return TRUE or FALSE
    */
    BOOL loadFromFile(char *filename);

    /**
    * reset history manager
    */
    void reset(void);

    /**
    * Get all lines in history
    * @param output size of the array of string
    * @return a array of string
    */
    char **getAllLines(int *numberoflines);

    /**
    * returns the first line in history
    * @return a string
    */
    char *getFirstLine(void);

    /**
    * returns the last line in history
    * @return a string
    */
    char *getLastLine(void);

    /**
    * get number of lines of history
    * @return a number >= 0
    */
    int getNumberOfLines(void);

    /**
    * Get the Nth Line in history
    * @param N
    * @return the Nth Line
    */
    char *getNthLine(int N);

    /**
    * delete the Nth Line in history
    * @param N
    * @return TRUE or FALSE
    */
    BOOL deleteNthLine(int N);

    /**
    * Allow to save consecutive duplicate lines
    * @param doit : TRUE (to allow) or FALSE
    */
    void setSaveConsecutiveDuplicateLines(BOOL doit);

    /**
    * get state about to save consecutive duplicate lines
    * @return TRUE or FALSE
    */
    BOOL getSaveConsecutiveDuplicateLines(void);

    /**
    * set after how many lines history is saved
    * @param num : number of lines
    */
    void setAfterHowManyLinesHistoryIsSaved(int num);

    /**
    * get after how many lines history is saved
    * @return a int : number of lines
    */
    int getAfterHowManyLinesHistoryIsSaved(void);

    /**
    * set new token to search in history
    * @param token (a string)
    * @return TRUE or FALSE
    */
    BOOL setToken(char *token);

    /**
    * get token searched in history
    * @return token (a string)
    */
    char *getToken(void);

    /**
    * resettoken searched in history
    * @return TRUE or FALSE
    */
    BOOL resetToken(void);

    /**
    * Get the previous line in search
    * @return a line
    */
    char *getPreviousLine(void);

    /**
    * Get the next line in Scilab history
    * @return a line or NULL
    * after a appendLine iterator go to end
    */
    char *getNextLine(void);

protected:

private:
    HistoryFile my_file;
    HistorySearch my_search;

    list<CommandLine> CommandsList;

    BOOL saveconsecutiveduplicatelines;
    int afterhowmanylineshistoryissaved;
    int numberoflinesbeforehistoryissaved;

    /**
    * search if line is a beginning of a session
    * @return TRUE or FALSE
    */
    BOOL isBeginningSessionLine(char *line);

    /**
    * add as first line  beginning session info
    */
    void fixHistorySession(void);


};
/*------------------------------------------------------------------------*/
#endif /* __cplusplus */
/*------------------------------------------------------------------------*/
#endif /* __HISTORYMANAGER_CPP_H__ */
