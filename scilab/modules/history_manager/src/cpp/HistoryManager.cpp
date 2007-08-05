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
	BOOL bOK = FALSE;

	if (line)
	{
		int i = 0;
		char *cleanedline = NULL;
		/* remove space & carriage return at the end of line */
		cleanedline = (char*) MALLOC(sizeof(char)*(strlen(line)+1));
		strcpy(cleanedline,line);

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

		bOK = ScilabHistory.appendLine(cleanedline);

		if (cleanedline) {FREE(cleanedline);cleanedline = NULL;}
	}
	return bOK;
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
char **getAllLinesOfScilabHistory(void)
{
	int nbElements = 0;
	char **lines = ScilabHistory.getAllLines(&nbElements);
	return lines;
}
/*------------------------------------------------------------------------*/
int getSizeAllLinesOfScilabHistory(void)
{
	int nbElements = 0;
	char **lines = ScilabHistory.getAllLines(&nbElements);

	if (lines)
	{
		int i = 0;

		for (i=0;i < nbElements;i++) 
		{ 
			if (lines[i])
			{
				FREE(lines[i]);
				lines[i]=NULL; 
			}
		}
		FREE(lines);
		lines=NULL; 
	}

	return nbElements;
}
/*------------------------------------------------------------------------*/
char *getLastLineInScilabHistory(void)
{
	return ScilabHistory.getLastLine();
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
BOOL deleteNthLineScilabHistory(int N)
{
	return ScilabHistory.deleteNthLine(N);
}
/*------------------------------------------------------------------------*/
int getSizeScilabHistory(void)
{
	return ScilabHistory.getNumberOfLines();
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
		filename = (char*)MALLOC(sizeof(char)*(my_file.getFilename().length()+1));
		if (filename) strcpy(filename,my_file.getFilename().c_str());
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
		appendLine(commentbeginsession);
		if (commentbeginsession) {FREE(commentbeginsession);commentbeginsession=NULL;}

		bOK = TRUE;
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
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
	appendLine(commentbeginsession);
	if (commentbeginsession) {FREE(commentbeginsession);commentbeginsession=NULL;}
}
/*-----------------------------------------------------------------------------------*/ 
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
				char *pLine = (char*)MALLOC(sizeof(char)*(strlen(line.c_str())+1));
				if (pLine)
				{
					strcpy(pLine,line.c_str());
					lines[i] = pLine;
					i++;
				}
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
	if (!CommandsList.empty()) 
	{
		std::string str;
		list<CommandLine>::iterator it_commands = CommandsList.end();
		it_commands--;
		str = (*it_commands).get();
		if (!str.empty())
		{
			line = (char*)MALLOC(sizeof(char)*(strlen(str.c_str())+1));
			if (line)
			{
				strcpy(line,str.c_str());
			}
		}
	}
	return line;
}
/*-----------------------------------------------------------------------------------*/ 
int HistoryManager::getNumberOfLines(void)
{
	return (int)CommandsList.size();
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
		for(it_commands=CommandsList.begin(); it_commands != CommandsList.end(); ++it_commands) 
		{
			if (i == N)
			{
				string str;
				str = (*it_commands).get();
				if (!str.empty())
				{
					line = (char*)MALLOC(sizeof(char)*(strlen(str.c_str())+1));
					if (line)
					{
						strcpy(line,str.c_str());
					}
					return line;
				}
			}
			i++;
		}
	}
	return line;
}
/*-----------------------------------------------------------------------------------*/ 
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
					CommandsList.erase(it_commands);
					// After a remove , we update search
					my_search.setToken(NULL);
					return TRUE;
				}
			}
			i++;
		}
	}
	return bOK;
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
	char *returnedline = NULL;

	if (my_search.getSize() > 0)
	{
		std::string line = my_search.getPreviousLine();
		if (!line.empty())
		{
			returnedline = (char*)MALLOC(sizeof(char)*(line.size()+1));
			if (returnedline) strcpy(returnedline,line.c_str());
		}
	}
	return returnedline;
}
/*-----------------------------------------------------------------------------------*/ 
char *HistoryManager::getNextLine(void)
{
	char *returnedline = NULL;

	if (my_search.getSize() > 0)
	{
		std::string line = my_search.getNextLine();
		if (!line.empty())
		{
			returnedline = (char*)MALLOC(sizeof(char)*(line.length()+1));
			if (returnedline) strcpy(returnedline,line.c_str());
		}
	}
	return returnedline;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL HistoryManager::setToken(char *token)
{
	BOOL bOK = FALSE;
	std::string Token;
	if (token) Token.assign(token);
	my_search.setHistory(CommandsList);
	bOK = my_search.setToken(Token);
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
char * HistoryManager::getToken(void)
{
	char *returnedtoken = NULL;
	std::string token = my_search.getToken();

	if (!token.empty())
	{
		returnedtoken = (char*)MALLOC(sizeof(char)*(token.length()+1));
		if (returnedtoken) strcpy(returnedtoken,token.c_str());
	}
	return returnedtoken;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL HistoryManager::resetToken(void)
{
	return my_search.reset();
}
/*-----------------------------------------------------------------------------------*/ 
