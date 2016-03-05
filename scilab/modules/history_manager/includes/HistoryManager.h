/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007-2008 - INRIA - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Allan CORNET
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
#ifndef __HISTORYMANAGER_C_H__
#define __HISTORYMANAGER_C_H__
#include "BOOL.h"
#include "dynlib_history_manager.h"
/*------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------*/
/**
* get history manager state
* @return TRUE or FALSE
*/
HISTORY_MANAGER_IMPEXP BOOL historyIsEnabled(void);

/**
* append lines to History manager
* @param array of string
* @param size of the array of string
* @return TRUE or FALSE
*/
HISTORY_MANAGER_IMPEXP BOOL appendLinesToScilabHistory(char **lines_, int numberoflines);
// Note it is lines_ because line is a keyword for term stuff

/**
* add a line to History manager
* @param a line to add
* line isn't added if it is the same as previous (FALSE)
* @return TRUE or FALSE
*/
HISTORY_MANAGER_IMPEXP BOOL appendLineToScilabHistory(char *line);

/**
* Display history
*/
HISTORY_MANAGER_IMPEXP void displayScilabHistory(void);

/**
* save history in a file
* @param a filename if NULL saves in default filename
* @return TRUE or FALSE
*/
HISTORY_MANAGER_IMPEXP BOOL writeScilabHistoryToFile(char *filename);

/**
* load history from a file
* @param a filename if NULL load from default filename
* @return TRUE or FALSE
*/
HISTORY_MANAGER_IMPEXP BOOL loadScilabHistoryFromFile(char *filename);

/**
* set filename of history
* @param filename of history
* @return TRUE or FALSE
*/
HISTORY_MANAGER_IMPEXP BOOL setFilenameScilabHistory(char *filename);

/**
* get filename of history
* @return a filename
*/
HISTORY_MANAGER_IMPEXP char *getFilenameScilabHistory(void);

/**
* set default filename of history
* @return TRUE or FALSE
*/
HISTORY_MANAGER_IMPEXP BOOL setDefaultFilenameScilabHistory(void);

/**
* get info about history file was or not truncated (too big)
* @return TRUE (truncated) or FALSE
*/
HISTORY_MANAGER_IMPEXP BOOL isScilabHistoryTruncated(void);

/**
* reset history manager
*/
HISTORY_MANAGER_IMPEXP BOOL resetScilabHistory(void);

/**
* Get all lines in history
* @return an array of string
*/
HISTORY_MANAGER_IMPEXP char **getAllLinesOfScilabHistory(void);

/**
* Get numbers of lines in history
* size of array returned by getAllLinesOfScilabHistory
* @return int
*/
HISTORY_MANAGER_IMPEXP int getSizeAllLinesOfScilabHistory(void);

/**
* set new token to search in history
* @param token (a string)
* @return TRUE or FALSE
*/
HISTORY_MANAGER_IMPEXP BOOL setSearchedTokenInScilabHistory(char *token);

/**
* reset search in history
* @return TRUE or FALSE
*/
HISTORY_MANAGER_IMPEXP BOOL resetSearchedTokenInScilabHistory(void);

/**
* get token searched in history
* @return token (a string)
*/
HISTORY_MANAGER_IMPEXP char *getSearchedTokenInScilabHistory(void);

/**
* Get the previous line in Scilab history
* @return a line or NULL
* after an appendLine iterator go to end
*/
HISTORY_MANAGER_IMPEXP char *getPreviousLineInScilabHistory(void);

/**
* Get the next line in Scilab history
* @return a line or NULL
* after an appendLine iterator go to end
*/
HISTORY_MANAGER_IMPEXP char *getNextLineInScilabHistory(void);

/**
* Get numbers of lines in history
* @return number of lines
*/
HISTORY_MANAGER_IMPEXP int getNumberOfLinesInScilabHistory(void);

/**
* set consecutive duplicate lines are added
* @param doit (TRUE or FALSE)
*/
HISTORY_MANAGER_IMPEXP void setSaveConsecutiveDuplicateLinesInScilabHistory(BOOL doit);

/**
* indicate if consecutive duplicate lines are added
* @return TRUE or FALSE
*/
HISTORY_MANAGER_IMPEXP BOOL getSaveConsecutiveDuplicateLinesInScilabHistory(void);

/**
* Set after how many lines history is saved
* @param number between 0 and Max
* default value is 0 (disabled)
*/
HISTORY_MANAGER_IMPEXP void setAfterHowManyLinesScilabHistoryIsSaved(int num);

/**
* Get after how many lines history is saved
* @return number between 0 and Max
* default value is 0 (disabled)
*/
HISTORY_MANAGER_IMPEXP int getAfterHowManyLinesScilabHistoryIsSaved(void);

/**
* Get the Nth Line in history
* @param N
* @return the Nth Line
*/
HISTORY_MANAGER_IMPEXP char *getNthLineInScilabHistory(int N);

/**
* delete the Nth Line in history
* @param N
* @return TRUE or FALSE
*/
HISTORY_MANAGER_IMPEXP BOOL deleteNthLineScilabHistory(int N);

/**
* get number of lines of history
* @return a number >= 0
*/
HISTORY_MANAGER_IMPEXP int getSizeScilabHistory(void);

/**
*/
HISTORY_MANAGER_IMPEXP BOOL setSizeMaxScilabHistory(int nbLinesMax);

/**
*/
int getSizeMaxScilabHistory(void);


/*------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/*------------------------------------------------------------------------*/
#endif /* __HISTORYMANAGER_C_H__ */
/*------------------------------------------------------------------------*/
