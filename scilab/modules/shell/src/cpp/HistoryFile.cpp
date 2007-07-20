/*------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*------------------------------------------------------------------------*/
#include "HistoryFile.hxx"
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
#include "getCommentDateSession.h"
};
/*------------------------------------------------------------------------*/
#define MAXBUF	1024
#ifdef _MSC_VER
#define DEFAULT_HISTORY_FILE "history.scilab"
#define SEPARATOR_FILE "\\"
#else
#define DEFAULT_HISTORY_FILE ".history.scilab"
#define SEPARATOR_FILE "/"
#endif
/*------------------------------------------------------------------------*/
HistoryFile::HistoryFile()
{
	my_history_filename = NULL;
}
/*------------------------------------------------------------------------*/
HistoryFile::~HistoryFile()
{
	this->reset();
}
/*------------------------------------------------------------------------*/
char *HistoryFile::getFilename(void)
{
	char *filename = NULL;

	if (this->my_history_filename == NULL) this->setDefaultFilename();

	if (this->my_history_filename) 
	{
		filename = (char*)MALLOC(sizeof(char)*(strlen(this->my_history_filename)+1));
		strcpy(filename,this->my_history_filename);
	}
	return filename;
}
/*------------------------------------------------------------------------*/
void HistoryFile::setFilename(char *filename)
{
	if (filename)
	{
		if (this->my_history_filename) 
		{
			FREE(this->my_history_filename);
			this->my_history_filename = NULL;
		}
		this->my_history_filename = (char*)MALLOC(sizeof(char)*(strlen(filename)+1));
		if (this->my_history_filename) strcpy(this->my_history_filename,filename);
	}
	else
	{
		this->setDefaultFilename();
	}
}
/*------------------------------------------------------------------------*/
BOOL HistoryFile::setDefaultFilename(void)
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
		this->setFilename(defaultfilename);
		FREE(defaultfilename);
		defaultfilename = NULL;
	}
	return bOK;
}
/*------------------------------------------------------------------------*/
BOOL HistoryFile::writeToFile(char *filename)
{
	BOOL bOK = FALSE;

	if (this->Commands.empty()) return bOK;
	else
	{
		FILE *pFile = NULL;
		pFile = fopen (filename,"wt");

		if (pFile)
		{
			char *commentendsession = NULL;
			list<CommandLine>::iterator it_commands;
			for(it_commands=this->Commands.begin(); it_commands != this->Commands.end(); ++it_commands) 
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
			fputs("\n",pFile);
			if (commentendsession) {FREE(commentendsession);commentendsession=NULL;}
			fclose(pFile);
			bOK = TRUE;
		}
	}
	return bOK;
}
/*------------------------------------------------------------------------*/
BOOL HistoryFile::writeToFile(void)
{
	BOOL bOK = FALSE;
	if (this->my_history_filename) bOK = this->writeToFile(my_history_filename);
	return bOK;
}
/*------------------------------------------------------------------------*/
BOOL HistoryFile::loadFromFile(char *filename)
{
	#define SECURITY_BUFFER 1000
	BOOL bOK = FALSE;
	char  line[MAXBUF];
	FILE * pFile = NULL;

	pFile = fopen (filename,"rt");
	if (pFile)
	{
		while(fgets (line,sizeof(line),pFile) != NULL)
		{
			line[strlen(line)-1]='\0'; /* remove carriage return */
			CommandLine Line(line);
			this->Commands.push_back(Line);
		}
		fclose(pFile);
		bOK = TRUE;
	}
	return bOK;
}
/*------------------------------------------------------------------------*/
BOOL HistoryFile::loadFromFile(void)
{
	BOOL bOK = FALSE;
	if (this->my_history_filename) bOK = this->loadFromFile(my_history_filename);
	return bOK;
}
/*------------------------------------------------------------------------*/
list<CommandLine> HistoryFile::getHistory(void)
{
	list <CommandLine> lines(Commands);
	return lines;
}
/*------------------------------------------------------------------------*/
BOOL HistoryFile::setHistory(list<CommandLine> commands)
{
	BOOL bOK = FALSE;
	list<CommandLine>::iterator it_commands;

	if (!this->Commands.empty()) this->Commands.clear();

	for(it_commands=commands.begin(); it_commands != commands.end(); ++it_commands) 
	{
		char *line = (*it_commands).get();
		if (line)
		{
			CommandLine Line(line);
			this->Commands.push_back(Line);
			FREE(line);
			line = NULL;
		}
	}
	return bOK;
}
/*------------------------------------------------------------------------*/
BOOL HistoryFile::reset(void)
{
	BOOL bOK = FALSE;
	BOOL check1 = FALSE,check2 = FALSE;

	if (!this->Commands.empty()) 
	{
		this->Commands.clear();
		check1 = TRUE;
	}

	if (my_history_filename) 
	{
		FREE(this->my_history_filename);
		this->my_history_filename = NULL;
		check2 = TRUE;
	}

	if (check1 && check2) bOK = TRUE;

	return bOK;
}
/*------------------------------------------------------------------------*/
