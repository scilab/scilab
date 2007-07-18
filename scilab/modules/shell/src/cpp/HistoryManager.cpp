/*------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*------------------------------------------------------------------------*/
#include "HistoryManager.hxx"
#include "HistoryManager.h"
#include "getCommentDateSession.h"
#include "MALLOC.h"
/*------------------------------------------------------------------------*/
extern "C"
{
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include "sciprint.h"
	#include "cluni0.h"
	#include "SCIHOME.h"
	#include "inffic.h"
};
/*------------------------------------------------------------------------*/
#define MAXBUF	1024
/*------------------------------------------------------------------------*/
static HistoryManager ScilabHistory;
/*------------------------------------------------------------------------*/
BOOL hasSearchedInScilabHistory(void)
{
	BOOL bOK = FALSE;
	return bOK;
}
/*------------------------------------------------------------------------*/
BOOL setSearchedTokenInScilabHistory(char *token)
{
	return ScilabHistory.setToken(token);
}
/*------------------------------------------------------------------------*/
BOOL resetSearchedTokenInScilabHistory(void)
{
	return ScilabHistory.resetToken();
}
/*------------------------------------------------------------------------*/
char *getSearchedTokenInScilabHistory(void)
{
	return ScilabHistory.getToken();
}
/*------------------------------------------------------------------------*/
BOOL appendLineToScilabHistory(char *line)
{
	return ScilabHistory.appendLine(line);
}
/*------------------------------------------------------------------------*/
BOOL appendLinesToScilabHistory(char **lines,int numberoflines)
{
	return ScilabHistory.appendLines(lines,numberoflines);
}
/*------------------------------------------------------------------------*/
void displayScilabHistory(void)
{
	ScilabHistory.displayHistory();
}
/*------------------------------------------------------------------------*/
BOOL writeScilabHistoryToFile(char *filename)
{
	return ScilabHistory.writeToFile(filename);
}
/*------------------------------------------------------------------------*/
BOOL loadScilabHistoryFromFile(char *filename)
{
	return ScilabHistory.loadFromFile(filename);
}
/*------------------------------------------------------------------------*/
BOOL setFilenameScilabHistory(char *filename)
{
	BOOL bOK = FALSE;
	if (filename)
	{
		ScilabHistory.setFilename(filename);
		bOK = TRUE;
	}
	return bOK;
}
/*------------------------------------------------------------------------*/
char *getFilenameScilabHistory(void)
{
	return ScilabHistory.getFilename();
}
/*------------------------------------------------------------------------*/
BOOL setDefaultFilenameScilabHistory(void)
{
	return ScilabHistory.setDefaultFilename();
}
/*------------------------------------------------------------------------*/
void resetScilabHistory(void)
{
	ScilabHistory.reset();
}
/*------------------------------------------------------------------------*/
char **getAllLinesOfScilabHistory(int *numberoflines)
{
	char **lines = ScilabHistory.getAllLines(numberoflines);
	return lines;
}
/*------------------------------------------------------------------------*/
char *getLastLineInScilabHistory(void)
{
	return ScilabHistory.getLastLine();
}
/*------------------------------------------------------------------------*/
char *getFirstLineInScilabHistory(void)
{
	return ScilabHistory.getFirstLine();
}
/*------------------------------------------------------------------------*/
char *getPreviousLineInScilabHistory(void)
{
	return ScilabHistory.getPreviousLine();
}
/*------------------------------------------------------------------------*/
char *getNextLineInScilabHistory(void)
{
	return ScilabHistory.getNextLine();
}
/*------------------------------------------------------------------------*/
int getNumberOfLinesInScilabHistory(void)
{
	return ScilabHistory.getNumberOfLines();
}
/*------------------------------------------------------------------------*/
void setSaveConsecutiveDuplicateLinesInScilabHistory(BOOL doit)
{
	ScilabHistory.setSaveConsecutiveDuplicateLines(doit);
}
/*------------------------------------------------------------------------*/
BOOL getSaveConsecutiveDuplicateLinesInScilabHistory(void)
{
	return ScilabHistory.getSaveConsecutiveDuplicateLines();
}
/*------------------------------------------------------------------------*/
void setAfterHowManyLinesScilabHistoryIsSaved(int num)
{
	ScilabHistory.setAfterHowManyLinesHistoryIsSaved(num);
}
/*------------------------------------------------------------------------*/
int getAfterHowManyLinesScilabHistoryIsSaved(void)
{
	return ScilabHistory.getAfterHowManyLinesHistoryIsSaved();
}
/*------------------------------------------------------------------------*/
char *getNthLineInScilabHistory(int N)
{
	return ScilabHistory.getNthLine(N);
}
/*------------------------------------------------------------------------*/
HistoryManager::HistoryManager()
{
	Commands.clear();
	
	saveconsecutiveduplicatelines = FALSE;
	afterhowmanylineshistoryissaved = 0;
	numberoflinesbeforehistoryissaved = 0;
}
/*------------------------------------------------------------------------*/
HistoryManager::~HistoryManager()
{
	Commands.clear();
}
/*------------------------------------------------------------------------*/
BOOL HistoryManager::appendLine(char *line)
{
	BOOL bOK = FALSE;

	if (line)
	{
		if (!saveconsecutiveduplicatelines)
		{
			char *previousline = getLastLine();

			if ( (previousline) && (strcmp(previousline,line)== 0) )
			{
				bOK = FALSE;
			}
			else
			{
				CommandLine Line(line);
				Commands.push_back(Line);
				numberoflinesbeforehistoryissaved++;
				bOK = TRUE;
			}
			if (previousline) {FREE(previousline);previousline = NULL;}
		}
		else
		{
			CommandLine Line(line);
			Commands.push_back(Line);

			numberoflinesbeforehistoryissaved++;
			bOK = TRUE;
		}
	}
	else 
	{
		
	}

	if (afterhowmanylineshistoryissaved != 0)
	{
		if (afterhowmanylineshistoryissaved == numberoflinesbeforehistoryissaved)
		{
			my_file.setHistory(Commands);
			my_file.writeToFile();
			numberoflinesbeforehistoryissaved = 0;
		}
	}
	else
	{
		numberoflinesbeforehistoryissaved = 0;
	}
	

	return bOK;
}
/*------------------------------------------------------------------------*/
BOOL HistoryManager::appendLines(char **lines,int nbrlines)
{
	BOOL bOK = TRUE;
	int i = 0;

	for (i = 0;i < nbrlines; i++)
	{
		if ( (lines[i] == NULL) || (!appendLine(lines[i])) ) bOK = FALSE;
	}
	return bOK;
}
/*------------------------------------------------------------------------*/
void HistoryManager::displayHistory(void)
{
	int nbline = 0;
	list<CommandLine>::iterator it_commands;
	for(it_commands=Commands.begin(); it_commands != Commands.end(); ++it_commands) 
	{
		char *line = (*it_commands).get();
		if (line)
		{
			sciprint("%d : %s\n",nbline,line);
			nbline++;
			FREE(line);
			line = NULL;
		}
	}
}
/*------------------------------------------------------------------------*/
char *HistoryManager::getFilename(void)
{
	return my_file.getFilename();
}
/*------------------------------------------------------------------------*/
void HistoryManager::setFilename(char *filename)
{
	my_file.setFilename(filename);
}
/*------------------------------------------------------------------------*/
BOOL HistoryManager::setDefaultFilename(void)
{
	return my_file.setDefaultFilename();
}
/*------------------------------------------------------------------------*/
BOOL HistoryManager::writeToFile(char *filename)
{
	my_file.setHistory(Commands);
	return my_file.writeToFile(filename);
}
/*------------------------------------------------------------------------*/
BOOL HistoryManager::loadFromFile(char *filename)
{
	BOOL bOK = FALSE;
	char *commentbeginsession = NULL;

	my_file.loadFromFile(filename);
	Commands.clear();
	Commands = my_file.getHistory();

	/* add date & time @ begin session */
	commentbeginsession = getCommentDateSession(TRUE);
	appendLine(commentbeginsession);
	if (commentbeginsession) {FREE(commentbeginsession);commentbeginsession=NULL;}
	

	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
void HistoryManager::reset(void)
{
	char *commentbeginsession = NULL;

	Commands.clear();

	my_file.reset();
	my_file.setDefaultFilename();

	saveconsecutiveduplicatelines = FALSE;
	afterhowmanylineshistoryissaved = 0;
	numberoflinesbeforehistoryissaved = 0;

	/* Ajout date & heure debut session */
	commentbeginsession = getCommentDateSession(TRUE);
	appendLine(commentbeginsession);
	if (commentbeginsession) {FREE(commentbeginsession);commentbeginsession=NULL;}
}
/*-----------------------------------------------------------------------------------*/ 
char **HistoryManager::getAllLines(int *numberoflines)
{
	char **lines = NULL;
	*numberoflines = 0;

	if (Commands.empty()) return lines;
	else
	{
		list<CommandLine>::iterator it_commands;
		int i = 0;

		lines = (char**)MALLOC((int)Commands.size()*(sizeof(char*)));
		for(it_commands=Commands.begin(); it_commands != Commands.end(); ++it_commands) 
		{
			char *line = (*it_commands).get();
			if (line)
			{
				lines[i] = line;
				i++;
			}
		}
		*numberoflines = i;
	}

	return lines;
}
/*-----------------------------------------------------------------------------------*/ 
char *HistoryManager::getLastLine(void)
{
	char *line = NULL;
	if (!Commands.empty()) 
	{
		list<CommandLine>::iterator it_commands = Commands.end();
		it_commands--;
		line = (*it_commands).get();
	}
	return line;
}
/*-----------------------------------------------------------------------------------*/ 
char *HistoryManager::getFirstLine(void)
{
	char *line = NULL;
	if (!Commands.empty()) 
	{
		list<CommandLine>::iterator it_commands = Commands.begin();
		line = (*it_commands).get();
	}
	return line;
}
/*-----------------------------------------------------------------------------------*/ 
int HistoryManager::getNumberOfLines(void)
{
	return (int)Commands.size();
}
/*-----------------------------------------------------------------------------------*/ 
char *HistoryManager::getNthLine(int N)
{
	char *line = NULL;

	if (N < 0) N = getNumberOfLines() + N;

	if ( (N >= 0) && (N <= getNumberOfLines()) )
	{
		int i = 0;
		list<CommandLine>::iterator it_commands;
		for(it_commands=Commands.begin(); it_commands != Commands.end(); ++it_commands) 
		{
			if (i == N) 
			{
				line = (*it_commands).get();
				return line;
			}
			i++;
		}
	}
	return line;
}
/*-----------------------------------------------------------------------------------*/ 
void HistoryManager::setSaveConsecutiveDuplicateLines(BOOL doit)
{
	saveconsecutiveduplicatelines = doit;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL HistoryManager::getSaveConsecutiveDuplicateLines(void)
{
	return saveconsecutiveduplicatelines;
}
/*-----------------------------------------------------------------------------------*/ 
void HistoryManager::setAfterHowManyLinesHistoryIsSaved(int num)
{
	if (num >= 0) 
	{
		afterhowmanylineshistoryissaved = num;
		numberoflinesbeforehistoryissaved = 0;
	}
}
/*-----------------------------------------------------------------------------------*/ 
int HistoryManager::getAfterHowManyLinesHistoryIsSaved(void)
{
	return afterhowmanylineshistoryissaved;
}
/*-----------------------------------------------------------------------------------*/ 
char *HistoryManager::getPreviousLine(void)
{
	if (my_search.getSize() > 0)
	{
		return my_search.getPreviousLine();
	}
	return NULL;
}
/*-----------------------------------------------------------------------------------*/ 
char *HistoryManager::getNextLine(void)
{
	if (my_search.getSize() > 0)
	{
		return my_search.getNextLine();
	}
	return NULL;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL HistoryManager::setToken(char *token)
{
	my_search.setHistory(Commands);
	return my_search.setToken(token);
}
/*-----------------------------------------------------------------------------------*/ 
char * HistoryManager::getToken(void)
{
	return my_search.getToken();
}
/*-----------------------------------------------------------------------------------*/ 
BOOL HistoryManager::resetToken(void)
{
	return my_search.reset();
}
/*-----------------------------------------------------------------------------------*/ 
