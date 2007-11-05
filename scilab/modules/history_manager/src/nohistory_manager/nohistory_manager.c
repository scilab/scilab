/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "machine.h" 
#include "HistoryManager.h"
#include "InitializeHistoryManager.h"
#include "TerminateHistoryManager.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
/*-----------------------------------------------------------------------------------*/ 
int C2F(gw_history_manager)()
{
	Scierror(999,_("history_manager interface not installed.\n"));
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL historyIsEnabled(void)
{
	return FALSE;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL InitializeHistoryManager(void)
{
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL TerminateHistoryManager(void)
{
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL appendLineToScilabHistory(char *line)
{
	return FALSE;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL appendLinesToScilabHistory(char **lines,int numberoflines)
{
	return FALSE;
}
/*-----------------------------------------------------------------------------------*/ 
void displayScilabHistory(void)
{

}
/*-----------------------------------------------------------------------------------*/ 
BOOL writeScilabHistoryToFile(char *filename)
{
	return FALSE;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL loadScilabHistoryFromFile(char *filename)
{
	return FALSE;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL setFilenameScilabHistory(char *filename)
{
	return FALSE;
}
/*-----------------------------------------------------------------------------------*/ 
char *getFilenameScilabHistory(void)
{
	return NULL;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL setDefaultFilenameScilabHistory(void)
{
	return FALSE;
}
/*-----------------------------------------------------------------------------------*/ 
void resetScilabHistory(void)
{

}
/*-----------------------------------------------------------------------------------*/ 
char **getAllLinesOfScilabHistory(void)
{
	return FALSE;
}
/*-----------------------------------------------------------------------------------*/ 
int getSizeAllLinesOfScilabHistory(void)
{
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL setSearchedTokenInScilabHistory(char *token)
{
	return FALSE;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL resetSearchedTokenInScilabHistory(void)
{
	return FALSE;
}
/*-----------------------------------------------------------------------------------*/ 
char *getSearchedTokenInScilabHistory(void)
{
	return NULL;
}
/*-----------------------------------------------------------------------------------*/ 
char *getPreviousLineInScilabHistory(void)
{
	return NULL;
}
/*-----------------------------------------------------------------------------------*/ 
char *getNextLineInScilabHistory(void)
{
	return NULL;
}
/*-----------------------------------------------------------------------------------*/ 
int getNumberOfLinesInScilabHistory(void)
{
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
void setSaveConsecutiveDuplicateLinesInScilabHistory(BOOL doit)
{

}
/*-----------------------------------------------------------------------------------*/ 
BOOL getSaveConsecutiveDuplicateLinesInScilabHistory(void)
{
	return FALSE;	
}
/*-----------------------------------------------------------------------------------*/ 
void setAfterHowManyLinesScilabHistoryIsSaved(int num)
{

}
/*-----------------------------------------------------------------------------------*/ 
int getAfterHowManyLinesScilabHistoryIsSaved(void)
{
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
char *getNthLineInScilabHistory(int N)
{
	return NULL;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL deleteNthLineScilabHistory(int N)
{
	return FALSE;
}
/*-----------------------------------------------------------------------------------*/ 
int getSizeScilabHistory(void)
{
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
