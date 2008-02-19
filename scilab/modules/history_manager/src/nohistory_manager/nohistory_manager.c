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

/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "HistoryManager.h"
#include "InitializeHistoryManager.h"
#include "TerminateHistoryManager.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int gw_history_manager(void)
{
	Scierror(999,_("Scilab history_manager module not installed.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
BOOL historyIsEnabled(void)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL InitializeHistoryManager(void)
{
	return TRUE;
}
/*--------------------------------------------------------------------------*/
BOOL TerminateHistoryManager(void)
{
	return TRUE;
}
/*--------------------------------------------------------------------------*/
BOOL appendLineToScilabHistory(char *line)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL appendLinesToScilabHistory(char **lines,int numberoflines)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
void displayScilabHistory(void)
{

}
/*--------------------------------------------------------------------------*/
BOOL writeScilabHistoryToFile(char *filename)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL loadScilabHistoryFromFile(char *filename)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL setFilenameScilabHistory(char *filename)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
char *getFilenameScilabHistory(void)
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
BOOL setDefaultFilenameScilabHistory(void)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
void resetScilabHistory(void)
{

}
/*--------------------------------------------------------------------------*/
char **getAllLinesOfScilabHistory(void)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
int getSizeAllLinesOfScilabHistory(void)
{
	return 0;
}
/*--------------------------------------------------------------------------*/
BOOL setSearchedTokenInScilabHistory(char *token)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL resetSearchedTokenInScilabHistory(void)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
char *getSearchedTokenInScilabHistory(void)
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
char *getPreviousLineInScilabHistory(void)
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
char *getNextLineInScilabHistory(void)
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
int getNumberOfLinesInScilabHistory(void)
{
	return 0;
}
/*--------------------------------------------------------------------------*/
void setSaveConsecutiveDuplicateLinesInScilabHistory(BOOL doit)
{

}
/*--------------------------------------------------------------------------*/
BOOL getSaveConsecutiveDuplicateLinesInScilabHistory(void)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
void setAfterHowManyLinesScilabHistoryIsSaved(int num)
{

}
/*--------------------------------------------------------------------------*/
int getAfterHowManyLinesScilabHistoryIsSaved(void)
{
	return 0;
}
/*--------------------------------------------------------------------------*/
char *getNthLineInScilabHistory(int N)
{
	return NULL;
}
/*--------------------------------------------------------------------------*/
BOOL deleteNthLineScilabHistory(int N)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
int getSizeScilabHistory(void)
{
	return 0;
}
/*--------------------------------------------------------------------------*/
