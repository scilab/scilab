/*------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*------------------------------------------------------------------------*/
#include "HistoryManager.h"
#include "HistoryManager_c.h"
#include "MALLOC.h"
/*------------------------------------------------------------------------*/
extern "C"
{
	#include <stdio.h>
	#include <string.h>
	#include <time.h>
	#include <stdlib.h>
	#include "sciprint.h"
	#include "cluni0.h"
	#include "SCIHOME.h"
	#include "inffic.h"
};
/*------------------------------------------------------------------------*/
#define MAXBUF	1024
#define string_begin_session "// Begin Session : "
#define string_end_session   "// End Session   : "
#ifdef _MSC_VER
#define DEFAULT_HISTORY_FILE "history.scilab"
#define SEPARATOR_FILE "\\"
#else
#define DEFAULT_HISTORY_FILE ".history.scilab"
#define SEPARATOR_FILE "/"
#endif
/*------------------------------------------------------------------------*/
HistoryManager ScilabHistory;
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
	BOOL bOK = FALSE;
	char *SCIHOME = getSCIHOME();
	char *defaultfilename = NULL;

	if (SCIHOME)
	{
		int lengthbuildfilename = 0;
		if (defaultfilename) FREE(defaultfilename);
		lengthbuildfilename = (int)(strlen(SCIHOME)+strlen(SEPARATOR_FILE)+strlen(DEFAULT_HISTORY_FILE)+1);
		defaultfilename = (char*)MALLOC(sizeof(char)*(lengthbuildfilename));
		sprintf(defaultfilename,"%s%s%s",SCIHOME,SEPARATOR_FILE,DEFAULT_HISTORY_FILE);
		FREE(SCIHOME); SCIHOME = NULL;
		bOK = TRUE;
	}
	else
	{
		char  *history_name = get_sci_data_strings(HISTORY_ID);
		int out_n = 0;
		defaultfilename = (char*)MALLOC(MAXBUF*sizeof(char));
		if ( defaultfilename == NULL ) return NULL;
		C2F(cluni0)(history_name, defaultfilename, &out_n,(long)strlen(history_name),MAXBUF);
	}

	if (defaultfilename)
	{
		bOK = setFilenameScilabHistory(defaultfilename);
		FREE(defaultfilename);
		defaultfilename = NULL;
	}
	return bOK;
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
	ScilabHistory.moveToPreviousPositionIterator();
	return ScilabHistory.getLineCurrentPositionIterator();
}
/*------------------------------------------------------------------------*/
char *getNextLineInScilabHistory(void)
{
	ScilabHistory.moveToNextPositionIterator();
	return ScilabHistory.getLineCurrentPositionIterator();
}
/*------------------------------------------------------------------------*/
HistoryManager::HistoryManager()
{
	historyfilename = NULL;
	reset();
}
/*------------------------------------------------------------------------*/
HistoryManager::~HistoryManager()
{
	Commands.clear();
	if (historyfilename)
	{
		FREE(historyfilename);
		historyfilename = NULL;
	}
}
/*------------------------------------------------------------------------*/
BOOL HistoryManager::appendLine(char *line)
{
	BOOL bOK = FALSE;
	if (line)
	{
		CommandLine Line(line);
		Commands.push_back(Line);
		bOK = TRUE;
	}

	it_current_position = Commands.end();

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
	int nbline = 1;
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
	char *filename = NULL;
	if (historyfilename) 
	{
		filename = (char*)MALLOC(sizeof(char)*(strlen(historyfilename)+1));
	}
	return filename;
}
/*------------------------------------------------------------------------*/
void HistoryManager::setFilename(char *filename)
{
	if (filename)
	{	
		if (historyfilename) FREE(historyfilename);
		historyfilename = (char*)MALLOC(sizeof(char)*(strlen(filename)+1));
		if (historyfilename) strcpy(historyfilename,filename);
	}
}
/*------------------------------------------------------------------------*/
BOOL HistoryManager::writeToFile(char *filename)
{
	BOOL bOK = FALSE;
	if (Commands.empty()) return bOK;
	else
	{
		FILE *pFile = NULL;
		pFile = fopen (filename,"wt");
		if (pFile)
		{
			char *commentendsession = NULL;
			list<CommandLine>::iterator it_commands;
			for(it_commands=Commands.begin(); it_commands != Commands.end(); ++it_commands) 
			{
				char *line = (*it_commands).get();
				if (line)
				{
					fputs(line,pFile);
					fputs("\n",pFile);
					FREE(line);
					line = NULL;
				}
			}

			commentendsession = getCommentDateSession(FALSE);
			fputs(commentendsession,pFile);
			if (commentendsession) {FREE(commentendsession);commentendsession=NULL;}
			fclose(pFile);
			bOK = TRUE;
		}
	}
	return bOK;
}
/*------------------------------------------------------------------------*/
BOOL HistoryManager::loadFromFile(char *filename)
{
	BOOL bOK = FALSE;
	char *commentbeginsession = NULL;
	char  line[MAXBUF];
	FILE * pFile = NULL;

	pFile = fopen (filename,"rt");
	if (pFile)
	{
		while(fgets (line,sizeof(line),pFile) != NULL)
		{
			line[strlen(line)-1]='\0'; /* enleve le retour chariot */
			appendLine(line);
		}
		fclose(pFile);
		bOK = TRUE;
	}
	/* add date & time @ begin session */
	commentbeginsession = getCommentDateSession(TRUE);
	appendLine(commentbeginsession);
	if (commentbeginsession) {FREE(commentbeginsession);commentbeginsession=NULL;}

	return bOK;
}
/*------------------------------------------------------------------------*/
char *HistoryManager::getCommentDateSession(BOOL BeginSession)
{
	char *line = NULL;
	char *time_str = NULL;
	time_t timer;
	timer=time(NULL);

	time_str = ASCIItime(localtime(&timer));

	if (BeginSession) 
	{
		line = (char*)MALLOC(sizeof(char)*(strlen(string_begin_session)+strlen(time_str)+1));
		if (line) sprintf(line,"%s%s",string_begin_session,time_str);
	}
	else 
	{
		line = (char*)MALLOC(sizeof(char)*(strlen(string_end_session)+strlen(time_str)+1));
		if (line) sprintf(line,"%s%s",string_end_session,time_str);
	}
	return line;
}
/*-----------------------------------------------------------------------------------*/ 
char *HistoryManager::ASCIItime(const struct tm *timeptr)
{
	static char wday_name[7][4] = {
		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
	};
	static char mon_name[12][4] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};

	static char result[26];

	sprintf(result, "%.3s %.3s%3d %.2d:%.2d:%.2d %d",
		wday_name[timeptr->tm_wday],
		mon_name[timeptr->tm_mon],
		timeptr->tm_mday, timeptr->tm_hour,
		timeptr->tm_min, timeptr->tm_sec,
		1900 + timeptr->tm_year);

	return result;
}
/*-----------------------------------------------------------------------------------*/ 
void HistoryManager::reset(void)
{
	Commands.clear();
	it_current_position = Commands.end();
	if (historyfilename)
	{
		FREE(historyfilename);
		historyfilename = NULL;
	}
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
	list<CommandLine>::iterator it_commands = Commands.end();
	 it_current_position = Commands.end();
	return (*it_commands).get();
}
/*-----------------------------------------------------------------------------------*/ 
char *HistoryManager::getFirstLine(void)
{
	list<CommandLine>::iterator it_commands = Commands.begin();
	it_current_position = Commands.begin();
	return (*it_commands).get();
}
/*-----------------------------------------------------------------------------------*/ 
char *HistoryManager::getLineCurrentPositionIterator(void)
{
	return (*it_current_position).get();
}
/*-----------------------------------------------------------------------------------*/ 
void HistoryManager::moveToPreviousPositionIterator(void)
{
	it_current_position--;
}
/*-----------------------------------------------------------------------------------*/ 
void HistoryManager::moveToNextPositionIterator(void)
{
	it_current_position++;
}
/*-----------------------------------------------------------------------------------*/ 


