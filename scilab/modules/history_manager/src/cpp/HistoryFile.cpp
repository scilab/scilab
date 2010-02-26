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
#include "HistoryFile.hxx"
#include "MALLOC.h"
#include "BOOL.h"
/*------------------------------------------------------------------------*/
extern "C"
{
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "sciprint.h"
#include "PATH_MAX.h"
#include "SCIHOME.h"
#include "inffic.h"
#include "getCommentDateSession.h"
#include "scilabDefaults.h"
#include "charEncoding.h"
};
/*------------------------------------------------------------------------*/
HistoryFile::HistoryFile()
{
	my_history_filename.erase();
}
/*------------------------------------------------------------------------*/
HistoryFile::~HistoryFile()
{
	this->reset();
}
/*------------------------------------------------------------------------*/
std::string HistoryFile::getFilename(void)
{
	if (this->my_history_filename.empty()) this->setDefaultFilename();
	return this->my_history_filename;
}
/*------------------------------------------------------------------------*/
void HistoryFile::setFilename(std::string filename)
{
	if (!filename.empty())
	{
		this->my_history_filename.erase();
		this->my_history_filename = filename;
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
	std::string defaultfilename;
	std::string defautlhistoryfile;

	defautlhistoryfile.assign(DEFAULT_HISTORY_FILE);

	if (SCIHOME)
	{
		std::string sep;
		std::string scihome;

		sep.assign(DIR_SEPARATOR);
		scihome.assign(SCIHOME);

		defaultfilename = scihome + sep + defautlhistoryfile;

		FREE(SCIHOME);
		SCIHOME = NULL;
		bOK = TRUE;
	}
	else
	{
		defaultfilename = defautlhistoryfile;
		bOK = FALSE;
		/* this isn't the standard path for history file */
		/* but we set a filename */
	}

	this->setFilename(defaultfilename);

	return bOK;
}
/*------------------------------------------------------------------------*/
BOOL HistoryFile::writeToFile(std::string filename)
{
	BOOL bOK = FALSE;

	if (this->Commands.empty()) return bOK;
	else
	{
		FILE *pFile = NULL;

		if (filename.empty())  return bOK;

		wcfopen(pFile , (char*)filename.c_str(), "wt");

		if (pFile)
		{
			char *commentendsession = NULL;
			list<CommandLine>::iterator it_commands;
			for(it_commands=this->Commands.begin(); it_commands != this->Commands.end(); ++it_commands)
			{
				std::string line = (*it_commands).get();
				if (!line.empty())
				{
					fputs(line.c_str(),pFile);
					fputs("\n",pFile);
				}
			}

			commentendsession = getCommentDateSession(FALSE);
			if (commentendsession)
			{
				fputs(commentendsession,pFile);
				fputs("\n",pFile);
				FREE(commentendsession);commentendsession=NULL;
			}
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
	if (!this->my_history_filename.empty()) bOK = this->writeToFile(my_history_filename);
	return bOK;
}
/*------------------------------------------------------------------------*/
BOOL HistoryFile::loadFromFile(std::string filename)
{
	#define SECURITY_BUFFER 1000
	BOOL bOK = FALSE;
	char  line[PATH_MAX+1];
	FILE * pFile = NULL;

	if (filename.empty()) return bOK;

	wcfopen(pFile , (char*)filename.c_str(), "rt");

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
	if (!this->my_history_filename.empty()) bOK = this->loadFromFile(my_history_filename);
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
		std::string line = (*it_commands).get();
		if (!line.empty())
		{
			CommandLine Line(line);
			this->Commands.push_back(Line);
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

	if (!my_history_filename.empty())
	{
		my_history_filename.erase();
		check2 = TRUE;
	}

	if (check1 && check2) bOK = TRUE;

	return bOK;
}
/*------------------------------------------------------------------------*/
