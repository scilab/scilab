/*------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*------------------------------------------------------------------------*/
#ifndef __HISTORYMANAGER_C_H__
#define __HISTORYMANAGER_C_H__
#include "machine.h" /* BOOL */
/*------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------*/
	/**
	* add a line to History manager
	* @param a line to add
	* line isn't added if it is the same as previous (FALSE)
	* @return TRUE or FALSE
	*/
	BOOL appendLineToScilabHistory(char *line);

	/**
	* append lines to History manager
	* @param array of string
	* @param size of the array of string
	* @return TRUE or FALSE
	*/
	BOOL appendLinesToScilabHistory(char **lines,int numberoflines);

	/**
	* Display history
	*/
	void displayScilabHistory(void);

	/**
	* save history in a file
	* @param a filename if NULL saves in default filename
	* default filename --> SCIHOME/history.scilab
	* @return TRUE or FALSE
	*/
	BOOL writeScilabHistoryToFile(char *filename);

	/**
	* load history from a file
	* @param a filename if NULL load from default filename
	* default filename --> SCIHOME/history.scilab
	* @return TRUE or FALSE
	*/
	BOOL loadScilabHistoryFromFile(char *filename);

	/**
	* set filename of history
	* @param filename of history
	* @return TRUE or FALSE
	*/
	BOOL setFilenameScilabHistory(char *filename);

	/**
	* get filename of history
	* @return a filename
	*/
	char *getFilenameScilabHistory(void);

	/**
	* set default filename of history
	* @return TRUE or FALSE
	*/
	BOOL setDefaultFilenameScilabHistory(void);

	/**
	* reset history manager
	*/
	void resetScilabHistory(void);

	/**
	* Get all lines in history
	* @param output size of the array of string
	* @return a array of string
	*/
	char **getAllLinesOfScilabHistory(int *numberoflines);

	/**
	* Get the most recent line in history
	* @return a line or NULL
	*/
	char *getLastLineInScilabHistory(void);

	/**
	* Get the first line in history
	* @return a line or NULL
	*/
	char *getFirstLineInScilabHistory(void);

	/**
	* Get the previous line in Scilab history
	* @return a line or NULL
	* after a appendLine iterator go to end
	*/
	char *getPreviousLineInScilabHistory(void);

	/**
	* Get the next line in Scilab history
	* @return a line or NULL
	* after a appendLine iterator go to end
	*/
	char *getNextLineInScilabHistory(void);

/*------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/*------------------------------------------------------------------------*/
#endif /* __HISTORYMANAGER_C_H__ */
/*------------------------------------------------------------------------*/
