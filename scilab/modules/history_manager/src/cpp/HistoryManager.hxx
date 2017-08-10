/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007-2008 - INRIA - Allan CORNET
* Copyright (C) 2011 - DIGITEO - Allan CORNET
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

/*------------------------------------------------------------------------*/
#ifndef __HISTORYMANAGER_CPP_H__
#define __HISTORYMANAGER_CPP_H__
/*------------------------------------------------------------------------*/
#ifdef __cplusplus
/*------------------------------------------------------------------------*/
extern "C"
{
#include "dynlib_history_manager.h"
#include <time.h>
};
/*------------------------------------------------------------------------*/
#include <list>
#include "HistoryFile.hxx"
#include "HistorySearch.hxx"

#ifdef _MSC_VER
//disable warnings about exports STL items
#pragma warning (disable : 4251)
#endif

/*------------------------------------------------------------------------*/
class HISTORY_MANAGER_IMPEXP HistoryManager
{
public:

    static HistoryManager* getInstance(void);
    static void killInstance(void);

    static BOOL historyIsEnabled(void);

    /*
    * add a line to History manager
    * @param a line to add
    * line isn't added if it is the same as previous (FALSE)
    * @return TRUE or FALSE
    */
    BOOL appendLine(char *_pstLine);

    /**
    * append lines to History manager
    * @param array of string
    * @param size of the array of string
    * @return TRUE or FALSE
    */
    BOOL appendLines(char **_pstLines, int _iLines);

    /**
    * Display history
    */
    void displayHistory(void);

    /**
    * get filename of history
    * @return a filename
    */
    char* getFilename(void);

    /**
    * set filename of history
    * @param filename of history
    * @return TRUE or FALSE
    */
    BOOL setFilename(char* _pstFilename);

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
    BOOL writeToFile(char* _pstFilename);

    /**
    * load history from a file
    * @param a filename if NULL load from default filename
    * default filename --> SCIHOME/<scilab version>history.scilab
    * @return TRUE or FALSE
    */
    BOOL loadFromFile(char* _pstFilename);

    /**
    * reset history manager
    */
    BOOL reset(void);

    /**
    * Get all lines in history
    * @param output size of the array of string
    * @return an array of strings
    */
    char** getAllLines(int* _iLines);

    /**
    * returns the first line in history
    * @return a string
    */
    char* getFirstLine(void);

    /**
    * returns the last line in history
    * @return a string
    */
    char* getLastLine(void);

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
    char* getNthLine(int _iLine);

    /**
    * delete the Nth Line in history
    * @param N
    * @return TRUE or FALSE
    */
    BOOL deleteNthLine(int _iLine);

    /**
    * Allow to save consecutive duplicate lines
    * @param doit : TRUE (to allow) or FALSE
    */
    void setSaveConsecutiveDuplicateLines(BOOL _bAllow);

    /**
    * get state about to save consecutive duplicate lines
    * @return TRUE or FALSE
    */
    BOOL getSaveConsecutiveDuplicateLines(void);

    /**
    * set after how many lines history is saved
    * @param num : number of lines
    */
    void setAfterHowManyLinesHistoryIsSaved(int _iNum);

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
    BOOL setToken(const char* _pstToken);

    /**
    * get token searched in history
    * @return token (a string)
    */
    char* getToken(void);

    /**
    * resettoken searched in history
    * @return TRUE or FALSE
    */
    BOOL resetToken(void);

    /**
    * Get the previous line in search
    * @return a line
    */
    char* getPreviousLine(void);

    /**
    * Get the next line in Scilab history
    * @return a line or NULL
    * after an appendLine iterator go to end
    */
    char* getNextLine(void);

    /**
    * get info about history file was truncated
    * @return TRUE or FALSE
    */
    BOOL isTruncated(void);

    /**
    *
    */
    BOOL setNumberOfLinesMax(int _iMaxLines);

    /**
    *
    */
    int getNumberOfLinesMax(void);

    /**
    * search if line is a beginning of a session
    * @return TRUE or FALSE
    */
    BOOL isBeginningSessionLine(char* _pstLine);

protected:

private:

    /**
    * Constructor
    */
    HistoryManager();

    /**
    * Destructor
    */
    ~HistoryManager();

    static HistoryManager* m_pHM;
    HistoryFile m_HF;
    HistorySearch m_HS;

    std::list<std::string> m_Commands;

    BOOL m_bAllowConsecutiveCommand;
    int m_iSaveLimit;
    int m_iSavedLines;

    /**
    * add as first line  beginning session info
    */
    void fixHistorySession(void);

    BOOL m_bTruncated;

};
/*------------------------------------------------------------------------*/
#endif /* __cplusplus */
/*------------------------------------------------------------------------*/
#endif /* __HISTORYMANAGER_CPP_H__ */
