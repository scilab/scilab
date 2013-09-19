/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007-2008 - INRIA - Allan CORNET
* Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
#include "machine.h"
#include "getCommentDateSession.h"
#include "scilabDefaults.h"
#include "charEncoding.h"
#include "mopen.h"
#include "mgetl.h"
#include "mclose.h"
#include "freeArrayOfString.h"
#include "getScilabPreference.h"
#include "expandPathVariable.h"
};
/*------------------------------------------------------------------------*/
#define DEFAULT_HISTORY_FILE_MAX_LINES 20000
/*------------------------------------------------------------------------*/
HistoryFile::HistoryFile()
{
    MaxLinesToRead = DEFAULT_HISTORY_FILE_MAX_LINES;
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
    if (this->my_history_filename.empty())
    {
        this->setDefaultFilename();
    }
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
    const ScilabPreferences* prefs = getScilabPreferences();
    if (prefs != NULL && prefs->historyFile != NULL)
    {
        const char* prefHistoryFile = prefs->historyFile;
        this->setFilename(expandPathVariable((char*)prefHistoryFile));
        return TRUE;
    }
    else
    {
        std::string filename(DEFAULT_HISTORY_FILE);
        char *SCIHOME = getSCIHOME();
        if (SCIHOME)
        {
            std::string scihome(SCIHOME);
            std::string sep(DIR_SEPARATOR);
            this->setFilename(scihome + sep + filename);
            return TRUE;
        }
        else
        {
            this->setFilename(filename);
            return FALSE;
        }
    }
}

/*------------------------------------------------------------------------*/
BOOL HistoryFile::writeToFile(std::string filename)
{
    BOOL bOK = FALSE;

    if (this->Commands.empty())
    {
        return bOK;
    }
    else
    {
        FILE *pFile = NULL;

        if (filename.empty())
        {
            return bOK;
        }

        wcfopen(pFile , (char*)filename.c_str(), "wt");

        if (pFile)
        {
            list<CommandLine>::iterator it_commands;
            for (it_commands = this->Commands.begin(); it_commands != this->Commands.end(); ++it_commands)
            {
                std::string line = (*it_commands).get();
                if (!line.empty())
                {
                    fputs(line.c_str(), pFile);
                    fputs("\n", pFile);
                }
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
    if (!this->my_history_filename.empty())
    {
        bOK = this->writeToFile(my_history_filename);
    }
    return bOK;
}
/*------------------------------------------------------------------------*/
errorLoadHistoryCode HistoryFile::loadFromFile(std::string filename)
{
    errorLoadHistoryCode returnedError = ERROR_HISTORY_NOT_LOADED;
    int fd = 0;
    int f_swap = 0;
    double res = 0.0;
    int errMOPEN = MOPEN_INVALID_STATUS;
    double dErrClose = 0.;


    C2F(mopen)(&fd, (char*)filename.c_str(), "rt", &f_swap, &res, &errMOPEN);
    if (errMOPEN == MOPEN_NO_ERROR)
    {
        int errMGETL = MGETL_ERROR;
        int nblines = 0;
        char **lines = mgetl(fd, -1, &nblines, &errMGETL);

        C2F(mclose)(&fd, &dErrClose);
        if (errMGETL == MGETL_NO_ERROR)
        {
            if (lines)
            {
                int iStart = 0;
                int iEnd = 0;
                if (nblines > getDefaultMaxNbLines())
                {
                    iStart = nblines - getDefaultMaxNbLines();
                    returnedError = HISTORY_TRUNCATED;
                }
                else
                {
                    iStart = 0;
                    returnedError = NO_ERROR_HISTORY_LOADED;
                }
                iEnd = nblines;

                for (int i = iStart; i < iEnd; i++)
                {
                    CommandLine Line(lines[i]);
                    this->Commands.push_back(Line);
                }
                freeArrayOfString(lines, nblines);
                lines = NULL;
            }
        }
    }
    return returnedError;
}
/*------------------------------------------------------------------------*/
errorLoadHistoryCode HistoryFile::loadFromFile(void)
{
    errorLoadHistoryCode returnedError = ERROR_HISTORY_NOT_LOADED;
    if (!this->my_history_filename.empty())
    {
        returnedError = this->loadFromFile(my_history_filename);
    }
    return returnedError;
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

    if (!this->Commands.empty())
    {
        this->Commands.clear();
    }

    for (it_commands = commands.begin(); it_commands != commands.end(); ++it_commands)
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
    BOOL check1 = FALSE, check2 = FALSE;

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

    if (check1 && check2)
    {
        bOK = TRUE;
    }

    return bOK;
}
/*------------------------------------------------------------------------*/
int HistoryFile::getDefaultMaxNbLines(void)
{
    return MaxLinesToRead;
}
/*------------------------------------------------------------------------*/
BOOL HistoryFile::setDefaultMaxNbLines(int nbLinesMax)
{
    BOOL bOK = FALSE;
    if (nbLinesMax > 0)
    {
        MaxLinesToRead = nbLinesMax;
        bOK = TRUE;
    }
    return bOK;
}
/*------------------------------------------------------------------------*/
