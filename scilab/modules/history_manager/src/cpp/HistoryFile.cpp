/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007-2008 - INRIA - Allan CORNET
* Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/
/*------------------------------------------------------------------------*/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "HistoryFile.hxx"
/*------------------------------------------------------------------------*/
extern "C"
{
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "sci_malloc.h"
#include "BOOL.h"
#include "sciprint.h"
#include "PATH_MAX.h"
#include "sci_home.h"
#include "machine.h"
#include "getCommentDateSession.h"
#include "scilabDefaults.h"
#include "charEncoding.h"
#include "mopen.h"
#include "mgetl.h"
#include "mclose.h"
#include "freeArrayOfString.h"
#include "os_wfopen.h"
#include "expandPathVariable.h"
};
/*------------------------------------------------------------------------*/
#define DEFAULT_HISTORY_FILE_MAX_LINES 20000
/*------------------------------------------------------------------------*/
HistoryFile::HistoryFile()
{
    m_iMaxLines = DEFAULT_HISTORY_FILE_MAX_LINES;
    m_stFilename.erase();
}
/*------------------------------------------------------------------------*/
HistoryFile::~HistoryFile()
{
    reset();
}
/*------------------------------------------------------------------------*/
std::string HistoryFile::getFilename(void)
{
    if (m_stFilename.empty())
    {
        setDefaultFilename();
    }
    return m_stFilename;
}
/*------------------------------------------------------------------------*/
void HistoryFile::setFilename(std::string _stFilename)
{
    if (_stFilename.empty() == false)
    {
        // TODO: const_cast is very bad...
        char * expanded = expandPathVariable((char *)(_stFilename.c_str()));
        m_stFilename = std::string(expanded);
        FREE(expanded);
    }
    else
    {
        setDefaultFilename();
    }
}
/*------------------------------------------------------------------------*/
BOOL HistoryFile::setDefaultFilename(void)
{
    BOOL bOK = FALSE;
    char* SCIHOME = getSCIHOME();
    std::string stDefaultFilename;

    stDefaultFilename = std::string(SCIHOME);
    stDefaultFilename += std::string(DIR_SEPARATOR);
    stDefaultFilename += std::string(DEFAULT_HISTORY_FILE);

    setFilename(stDefaultFilename);
    FREE(SCIHOME);
    return TRUE;
}
/*------------------------------------------------------------------------*/
BOOL HistoryFile::writeToFile(std::string _stFilename)
{
    if (m_Commands.empty())
    {
        return FALSE;
    }
    else
    {
        std::ofstream fOut;

        if (_stFilename.empty())
        {
            return FALSE;
        }

#ifdef _MSC_VER
        wchar_t* filename = to_wide_string(_stFilename.c_str());
        fOut.open(filename, std::ios::trunc);
        FREE(filename);
#else
        fOut.open(_stFilename.c_str(), std::ios::trunc);
#endif
        if (fOut.is_open() == false)
        {
            return FALSE;
        }

        std::list<std::string>::const_iterator it;
        for (it = m_Commands.begin(); it != m_Commands.end(); it++)
        {
            fOut << (*it).c_str() << std::endl;
        }
        fOut.close();
    }
    return TRUE;
}
/*------------------------------------------------------------------------*/
BOOL HistoryFile::writeToFile(void)
{
    if (m_stFilename.empty() == false)
    {
        return writeToFile(m_stFilename);
    }
    return FALSE;
}
/*------------------------------------------------------------------------*/

errorLoadHistoryCode HistoryFile::loadFromFile(std::string _stFilename)
{
    errorLoadHistoryCode returnedError = ERROR_HISTORY_NOT_LOADED;
    std::ifstream fIn;
    std::vector<std::string> vstLines;

    fIn.open(_stFilename.c_str());
    if (fIn.is_open() == false)
    {
        return returnedError;
    }

    //read entire file and store it in vstLines.
    while (fIn.eof() == false)
    {
        std::string stLine;
        std::getline(fIn, stLine);

        if (stLine.empty())
        {
            continue;
        }
        vstLines.push_back(stLine);
    }
    fIn.close();

    //fill history list
    int iStart = 0;
    int iEnd = (int)vstLines.size();
    returnedError = NO_ERROR_HISTORY_LOADED;

    if (vstLines.size() > getDefaultMaxNbLines())
    {
        iStart = (int)vstLines.size() - getDefaultMaxNbLines();
        returnedError = HISTORY_TRUNCATED;
    }

    for (int i = iStart ; i < iEnd ; i++)
    {
        m_Commands.push_back(vstLines[i]);
    }

    return returnedError;

    /*
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
                        Commands.push_back(Line);
                    }
                    freeArrayOfString(lines, nblines);
                    lines = NULL;
                }
            }
        }
        return returnedError;
    */
}
/*------------------------------------------------------------------------*/
errorLoadHistoryCode HistoryFile::loadFromFile(void)
{
    errorLoadHistoryCode returnedError = ERROR_HISTORY_NOT_LOADED;
    if (m_stFilename.empty() == false)
    {
        returnedError = loadFromFile(m_stFilename);
    }
    return returnedError;
}
/*------------------------------------------------------------------------*/
std::list<std::string> HistoryFile::getHistory(void)
{
    return m_Commands;
}
/*------------------------------------------------------------------------*/
BOOL HistoryFile::setHistory(std::list<std::string> _lstCommands)
{
    BOOL bOK = FALSE;
    std::list<std::string>::const_iterator it;

    if (m_Commands.empty() == false)
    {
        m_Commands.clear();
    }

    for (it = _lstCommands.begin(); it != _lstCommands.end(); it++)
    {
        m_Commands.push_back(*it);
    }
    return bOK;
}
/*------------------------------------------------------------------------*/
BOOL HistoryFile::reset(void)
{
    m_Commands.clear();
    m_stFilename.erase();
    return TRUE;
}
/*------------------------------------------------------------------------*/
int HistoryFile::getDefaultMaxNbLines(void)
{
    return m_iMaxLines;
}
/*------------------------------------------------------------------------*/
BOOL HistoryFile::setDefaultMaxNbLines(int _iMaxLines)
{
    BOOL bOK = FALSE;
    if (_iMaxLines > 0)
    {
        m_iMaxLines = _iMaxLines;
        bOK = TRUE;
    }
    return bOK;
}
/*------------------------------------------------------------------------*/
