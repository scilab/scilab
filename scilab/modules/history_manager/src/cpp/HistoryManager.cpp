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
	#include "SCIHOME.h"
	#include "inffic.h"
	#include "InitializeHistoryManager.h"
	#include "TerminateHistoryManager.h"
	#include "freeArrayOfString.h"
	#ifdef _MSC_VER
	#include "strdup_windows.h"
	#endif
};
/*------------------------------------------------------------------------*/
#define MAXBUF	1024
/*------------------------------------------------------------------------*/
static HistoryManager *ScilabHistory = NULL;
/*------------------------------------------------------------------------*/
BOOL historyIsEnabled(void)
{
	BOOL bOK = FALSE;
	if (ScilabHistory) bOK = TRUE;
	return bOK;
}
/*------------------------------------------------------------------------*/
BOOL InitializeHistoryManager(void)
{
	BOOL bOK = FALSE;
	if (!ScilabHistory)
	{
		ScilabHistory = new HistoryManager();
		if (ScilabHistory) bOK = TRUE;
	}
	return bOK;
}
/*------------------------------------------------------------------------*/
BOOL TerminateHistoryManager(void)
{
	BOOL bOK = FALSE;
	if (ScilabHistory)
	{
		delete ScilabHistory;
		ScilabHistory = NULL;
		bOK = TRUE;
	}
	return bOK;
}
/*------------------------------------------------------------------------*/
BOOL setSearchedTokenInScilabHistory(char *token)
{
	BOOL bOK = FALSE;
	if (ScilabHistory) bOK = ScilabHistory->setToken(token);
	return bOK;
}
/*------------------------------------------------------------------------*/
BOOL resetSearchedTokenInScilabHistory(void)
{
	BOOL bOK = FALSE;
	if (ScilabHistory) bOK = ScilabHistory->resetToken();
	return bOK;
}
/*------------------------------------------------------------------------*/
char *getSearchedTokenInScilabHistory(void)
{
	char *token = NULL;
	if (ScilabHistory) token = ScilabHistory->getToken();
	return token;
}
/*------------------------------------------------------------------------*/
BOOL appendLineToScilabHistory(char *line)
{
	BOOL bOK = FALSE;

	if (line)
	{
		int i = 0;
		char *cleanedline = NULL;
		/* remove space & carriage return at the end of line */
		cleanedline = strdup(line);

		/* remove carriage return at the end of line */
		for (i = (int) strlen(cleanedline); i > 0 ; i--)
		{
			if (cleanedline[i]=='\n')
			{
				cleanedline[i] = '\0';
				break;
			}
		}

		/* remove spaces at the end of line */
		i = (int)strlen(cleanedline) - 1;
		while (i>=0)
		{
			if ( cleanedline[i] == ' ')
			{
				cleanedline[i] = '\0';
			}
			else break;

			i--;
		}

		if (ScilabHistory) bOK = ScilabHistory->appendLine(cleanedline);


		if (cleanedline) {FREE(cleanedline);cleanedline = NULL;}
	}
	return bOK;
}
/*------------------------------------------------------------------------*/
BOOL appendLinesToScilabHistory(char **lines,int numberoflines)
{
	BOOL bOK = FALSE;
	if (ScilabHistory) bOK = ScilabHistory->appendLines(lines,numberoflines);
	return bOK;
}
/*------------------------------------------------------------------------*/
void displayScilabHistory(void)
{
	if (ScilabHistory) ScilabHistory->displayHistory();
}
/*------------------------------------------------------------------------*/
BOOL writeScilabHistoryToFile(char *filename)
{
	BOOL bOK = FALSE;
	if (ScilabHistory) bOK = ScilabHistory->writeToFile(filename);
	return bOK;
}
/*------------------------------------------------------------------------*/
BOOL loadScilabHistoryFromFile(char *filename)
{
	BOOL bOK = FALSE;
	if (ScilabHistory) bOK = ScilabHistory->loadFromFile(filename);
	return bOK;
}
/*------------------------------------------------------------------------*/
BOOL setFilenameScilabHistory(char *filename)
{
	BOOL bOK = FALSE;
	if (filename)
	{
		if (ScilabHistory)
		{
			ScilabHistory->setFilename(filename);
			bOK = TRUE;
		}
	}
	return bOK;
}
/*------------------------------------------------------------------------*/
char *getFilenameScilabHistory(void)
{
	char *filename = NULL;
	if (ScilabHistory) filename = ScilabHistory->getFilename();
	return filename;
}
/*------------------------------------------------------------------------*/
BOOL setDefaultFilenameScilabHistory(void)
{
	BOOL bOK = FALSE;
	if (ScilabHistory) bOK = ScilabHistory->setDefaultFilename();
	return bOK;
}
/*------------------------------------------------------------------------*/
void resetScilabHistory(void)
{
	if (ScilabHistory) ScilabHistory->reset();
}
/*------------------------------------------------------------------------*/
char **getAllLinesOfScilabHistory(void)
{
	int nbElements = 0;
	char **lines = NULL;
	if (ScilabHistory) lines = ScilabHistory->getAllLines(&nbElements);
	return lines;
}
/*------------------------------------------------------------------------*/
int getSizeAllLinesOfScilabHistory(void)
{
	int nbElements = 0;
	char **lines = NULL;

	if (ScilabHistory) lines = ScilabHistory->getAllLines(&nbElements);

	freeArrayOfString(lines, nbElements);

	return nbElements;
}
/*------------------------------------------------------------------------*/
char *getLastLineInScilabHistory(void)
{
	char *line = NULL;
	if (ScilabHistory) line = ScilabHistory->getLastLine();
	return line;
}
/*------------------------------------------------------------------------*/
char *getPreviousLineInScilabHistory(void)
{
	char *line = NULL;
	if (ScilabHistory) line = ScilabHistory->getPreviousLine();
	return line;
}
/*------------------------------------------------------------------------*/
char *getNextLineInScilabHistory(void)
{
	char *line = NULL;
	if (ScilabHistory) line = ScilabHistory->getNextLine();
	return line;
}
/*------------------------------------------------------------------------*/
int getNumberOfLinesInScilabHistory(void)
{
	int val = 0;
	if (ScilabHistory) val = ScilabHistory->getNumberOfLines();
	return val;
}
/*------------------------------------------------------------------------*/
void setSaveConsecutiveDuplicateLinesInScilabHistory(BOOL doit)
{
	if (ScilabHistory) ScilabHistory->setSaveConsecutiveDuplicateLines(doit);
}
/*------------------------------------------------------------------------*/
BOOL getSaveConsecutiveDuplicateLinesInScilabHistory(void)
{
	BOOL bOK = FALSE;
	if (ScilabHistory) bOK = ScilabHistory->getSaveConsecutiveDuplicateLines();
	return bOK;
}
/*------------------------------------------------------------------------*/
void setAfterHowManyLinesScilabHistoryIsSaved(int num)
{
	if (ScilabHistory) ScilabHistory->setAfterHowManyLinesHistoryIsSaved(num);
}
/*------------------------------------------------------------------------*/
int getAfterHowManyLinesScilabHistoryIsSaved(void)
{
	int val = 0;
	if (ScilabHistory) val = ScilabHistory->getAfterHowManyLinesHistoryIsSaved();
	return val;
}
/*------------------------------------------------------------------------*/
char *getNthLineInScilabHistory(int N)
{
	char *line = NULL;
	if (ScilabHistory) line = ScilabHistory->getNthLine(N);
	return line;
}
/*------------------------------------------------------------------------*/
BOOL deleteNthLineScilabHistory(int N)
{
	BOOL bOK = FALSE;
	if (ScilabHistory) bOK = ScilabHistory->deleteNthLine(N);
	return bOK;
}
/*------------------------------------------------------------------------*/
int getSizeScilabHistory(void)
{
	int val = 0;
	if (ScilabHistory) val = ScilabHistory->getNumberOfLines() - 1;
	return val;
}
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*/
HistoryManager::HistoryManager()
{
	CommandsList.clear();
	saveconsecutiveduplicatelines = FALSE;
	afterhowmanylineshistoryissaved = 0;
	numberoflinesbeforehistoryissaved = 0;
}
/*------------------------------------------------------------------------*/
HistoryManager::~HistoryManager()
{
	CommandsList.clear();
}
/*------------------------------------------------------------------------*/
BOOL HistoryManager::appendLine(char *cline)
{
	BOOL bOK = FALSE;

	if (cline)
	{
		if (!saveconsecutiveduplicatelines)
		{
			char *previousline = getLastLine();

			if ( (previousline) && (strcmp(previousline,cline)== 0) )
			{
				bOK = FALSE;
			}
			else
			{
				CommandLine Line(cline);
				CommandsList.push_back(Line);
				numberoflinesbeforehistoryissaved++;
				bOK = TRUE;
			}
			if (previousline) {FREE(previousline);previousline = NULL;}
		}
		else
		{
			CommandLine Line(cline);
			CommandsList.push_back(Line);

			numberoflinesbeforehistoryissaved++;
			bOK = TRUE;
		}
	}

	if (afterhowmanylineshistoryissaved != 0)
	{
		if (afterhowmanylineshistoryissaved == numberoflinesbeforehistoryissaved)
		{
			my_file.setHistory(CommandsList);
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
	for(it_commands=CommandsList.begin(); it_commands != CommandsList.end(); ++it_commands)
	{
		std::string line = (*it_commands).get();
		if (!line.empty())
		{
			sciprint("%d : %s\n",nbline,line.c_str());
			nbline++;
		}
	}
}
/*------------------------------------------------------------------------*/
char *HistoryManager::getFilename(void)
{
	char *filename = NULL;

	if (! my_file.getFilename().empty())
	{
		filename = strdup(my_file.getFilename().c_str());
	}
	return filename;
}
/*------------------------------------------------------------------------*/
void HistoryManager::setFilename(char *filename)
{
	if (filename)
	{
		std::string name;
		name.assign(filename);
		my_file.setFilename(name);
	}
}
/*------------------------------------------------------------------------*/
BOOL HistoryManager::setDefaultFilename(void)
{
	return my_file.setDefaultFilename();
}
/*------------------------------------------------------------------------*/
BOOL HistoryManager::writeToFile(char *filename)
{
	if (filename)
	{
		std::string name;
		name.assign(filename);

		my_file.setHistory(CommandsList);
		return my_file.writeToFile(name);
	}
	return FALSE;
}
/*------------------------------------------------------------------------*/
BOOL HistoryManager::loadFromFile(char *filename)
{
	BOOL bOK = FALSE;

	if (filename)
	{
		char *commentbeginsession = NULL;
		std::string name;
		name.assign(filename);

		my_file.loadFromFile(name);

		CommandsList.clear();
		CommandsList = my_file.getHistory();

		/* add date & time @ begin session */
		commentbeginsession = getCommentDateSession(TRUE);
		if (commentbeginsession)
		{
			appendLine(commentbeginsession);
			FREE(commentbeginsession);commentbeginsession=NULL;
		}
		bOK = TRUE;
	}
	return bOK;
}
/*--------------------------------------------------------------------------*/
void HistoryManager::reset(void)
{
	char *commentbeginsession = NULL;

	CommandsList.clear();

	my_file.reset();
	my_file.setDefaultFilename();

	my_search.reset();

	saveconsecutiveduplicatelines = FALSE;
	afterhowmanylineshistoryissaved = 0;
	numberoflinesbeforehistoryissaved = 0;

	/* Add date & time begin session */
	commentbeginsession = getCommentDateSession(TRUE);
	if (commentbeginsession)
	{
		appendLine(commentbeginsession);
		FREE(commentbeginsession);commentbeginsession=NULL;
	}
}
/*--------------------------------------------------------------------------*/
char **HistoryManager::getAllLines(int *numberoflines)
{
	char **lines = NULL;
	*numberoflines = 0;

	if (CommandsList.empty()) return lines;
	else
	{
		list<CommandLine>::iterator it_commands;
		int i = 0;

		lines = (char**)MALLOC((int)CommandsList.size()*(sizeof(char*)));
		for(it_commands=CommandsList.begin(); it_commands != CommandsList.end(); ++it_commands)
		{
			string line = (*it_commands).get();
			if (!line.empty())
			{
				lines[i] = strdup(line.c_str());
				i++;
			}
		}
		*numberoflines = i;
	}
	return lines;
}
/*--------------------------------------------------------------------------*/
char *HistoryManager::getLastLine(void)
{
	char *line = NULL;
	if (!CommandsList.empty())
	{
		std::string str;
		list<CommandLine>::iterator it_commands = CommandsList.end();
		it_commands--;
		str = (*it_commands).get();
		if (!str.empty())
		{
			line = strdup(str.c_str());
		}
	}
	return line;
}
/*--------------------------------------------------------------------------*/
int HistoryManager::getNumberOfLines(void)
{
	return (int)CommandsList.size();
}
/*--------------------------------------------------------------------------*/
char *HistoryManager::getNthLine(int N)
{
	char *line = NULL;

	if (N < 0) N = getNumberOfLines() + N;

	if ( (N >= 0) && (N <= getNumberOfLines()) )
	{
		int i = 0;
		list<CommandLine>::iterator it_commands;
		for(it_commands=CommandsList.begin(); it_commands != CommandsList.end(); ++it_commands)
		{
			if (i == N)
			{
				string str;
				str = (*it_commands).get();
				if (!str.empty())
				{
					return strdup(str.c_str());
				}
			}
			i++;
		}
	}
	return line;
}
/*--------------------------------------------------------------------------*/
BOOL HistoryManager::deleteNthLine(int N)
{
	BOOL bOK = FALSE;
	if ( (N >= 0) && (N <= getNumberOfLines()) )
	{
		int i = 0;
		list<CommandLine>::iterator it_commands;
		for(it_commands=CommandsList.begin(); it_commands != CommandsList.end(); ++it_commands)
		{
			if (i == N)
			{
				if ( it_commands != CommandsList.end() )
				{
					std::string str;
					str.erase();
					CommandsList.erase(it_commands);
					// After a remove , we update search
					my_search.setHistory(CommandsList);
					my_search.setToken(str);
					return TRUE;
				}
			}
			i++;
		}
	}
	return bOK;
}
/*--------------------------------------------------------------------------*/
void HistoryManager::setSaveConsecutiveDuplicateLines(BOOL doit)
{
	saveconsecutiveduplicatelines = doit;
}
/*--------------------------------------------------------------------------*/
BOOL HistoryManager::getSaveConsecutiveDuplicateLines(void)
{
	return saveconsecutiveduplicatelines;
}
/*--------------------------------------------------------------------------*/
void HistoryManager::setAfterHowManyLinesHistoryIsSaved(int num)
{
	if (num >= 0)
	{
		afterhowmanylineshistoryissaved = num;
		numberoflinesbeforehistoryissaved = 0;
	}
}
/*--------------------------------------------------------------------------*/
int HistoryManager::getAfterHowManyLinesHistoryIsSaved(void)
{
	return afterhowmanylineshistoryissaved;
}
/*--------------------------------------------------------------------------*/
char *HistoryManager::getPreviousLine(void)
{
	char *returnedline = NULL;

	if (my_search.getSize() > 0)
	{
		std::string line = my_search.getPreviousLine();
		if (!line.empty())
		{
			returnedline = strdup(line.c_str());
		}
	}
	return returnedline;
}
/*--------------------------------------------------------------------------*/
char *HistoryManager::getNextLine(void)
{
	char *returnedline = NULL;

	if (my_search.getSize() > 0)
	{
		std::string line = my_search.getNextLine();
		returnedline = strdup(line.c_str());
	}
	return returnedline;
}
/*--------------------------------------------------------------------------*/
BOOL HistoryManager::setToken(char *token)
{
	BOOL bOK = FALSE;
	std::string Token;
	if (token) Token.assign(token);
	my_search.setHistory(CommandsList);
	bOK = my_search.setToken(Token);
	return bOK;
}
/*--------------------------------------------------------------------------*/
char * HistoryManager::getToken(void)
{
	char *returnedtoken = NULL;
	std::string token = my_search.getToken();

	if (!token.empty())
	{
		returnedtoken = strdup(token.c_str());
	}
	return returnedtoken;
}
/*--------------------------------------------------------------------------*/
BOOL HistoryManager::resetToken(void)
{
	return my_search.reset();
}
/*--------------------------------------------------------------------------*/
