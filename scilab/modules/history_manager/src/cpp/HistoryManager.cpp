/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007-2008 - INRIA - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Vincent COUVERT
* Copyright (C) 2011 - DIGITEO - Allan CORNET
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
#include "expandPathVariable.h"
#include "InitializeHistoryManager.h"
#include "TerminateHistoryManager.h"
#include "freeArrayOfString.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "CommandHistory_Wrap.h"
};

/*------------------------------------------------------------------------*/
#define MAXBUF	1024
/*------------------------------------------------------------------------*/
static HistoryManager *ScilabHistory = NULL;

/*------------------------------------------------------------------------*/
BOOL historyIsEnabled(void)
{
    if (ScilabHistory)
    {
        return TRUE;
    }
    return FALSE;
}

/*------------------------------------------------------------------------*/
BOOL InitializeHistoryManager(void)
{
    if (!ScilabHistory)
    {
        ScilabHistory = new HistoryManager();
        if (ScilabHistory)
        {
            return TRUE;
        }
    }
    return FALSE;
}

/*------------------------------------------------------------------------*/
BOOL TerminateHistoryManager(void)
{
    if (ScilabHistory)
    {
        delete ScilabHistory;

        ScilabHistory = NULL;
        return TRUE;
    }
    return FALSE;
}

/*------------------------------------------------------------------------*/
BOOL setSearchedTokenInScilabHistory(char *token)
{
    if (ScilabHistory)
    {
        return ScilabHistory->setToken(token);
    }
    return FALSE;
}

/*------------------------------------------------------------------------*/
BOOL resetSearchedTokenInScilabHistory(void)
{
    if (ScilabHistory)
    {
        return ScilabHistory->resetToken();
    }
    return FALSE;
}

/*------------------------------------------------------------------------*/
char *getSearchedTokenInScilabHistory(void)
{
    char *token = NULL;

    if (ScilabHistory)
    {
        token = ScilabHistory->getToken();
    }
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

        if (ScilabHistory && ScilabHistory->getNumberOfLines() == 0)
        {
            char * commentbeginsession = getCommentDateSession(FALSE);
            ScilabHistory->appendLine(commentbeginsession);
            FREE(commentbeginsession);
            CommandHistoryExpandAll();
        }

        /* remove space & carriage return at the end of line */
        cleanedline = strdup(line);

        /* remove carriage return at the end of line */
        for (i = (int)strlen(cleanedline); i > 0; i--)
        {
            if (cleanedline[i] == '\n')
            {
                cleanedline[i] = '\0';
                break;
            }
        }

        /* remove spaces at the end of line */
        i = (int)strlen(cleanedline) - 1;
        while (i >= 0)
        {
            if (cleanedline[i] == ' ')
            {
                cleanedline[i] = '\0';
            }
            else
            {
                break;
            }

            i--;
        }

        if (ScilabHistory)
        {
            bOK = ScilabHistory->appendLine(cleanedline);
        }

        if (cleanedline)
        {
            FREE(cleanedline);
            cleanedline = NULL;
        }
    }

    return bOK;
}

/*------------------------------------------------------------------------*/
BOOL appendLinesToScilabHistory(char **lines, int numberoflines)
{
    if (ScilabHistory)
    {
        return ScilabHistory->appendLines(lines, numberoflines);
    }
    return FALSE;
}

/*------------------------------------------------------------------------*/
void displayScilabHistory(void)
{
    if (ScilabHistory)
    {
        ScilabHistory->displayHistory();
    }
}

/*------------------------------------------------------------------------*/
BOOL writeScilabHistoryToFile(char *filename)
{
    if (ScilabHistory)
    {
        return ScilabHistory->writeToFile(filename);
    }
    return FALSE;
}

/*------------------------------------------------------------------------*/
BOOL loadScilabHistoryFromFile(char *filename)
{
    if (ScilabHistory)
    {
        return ScilabHistory->loadFromFile(filename);
    }
    return FALSE;
}

/*------------------------------------------------------------------------*/
BOOL isScilabHistoryTruncated(void)
{
    if (ScilabHistory)
    {
        return ScilabHistory->isTruncated();
    }
    return FALSE;
}
/*------------------------------------------------------------------------*/
BOOL setFilenameScilabHistory(char *filename)
{
    char * expandedPath = NULL;

    if (filename)
    {
        if (ScilabHistory)
        {
            expandedPath = expandPathVariable(filename);
            if (expandedPath)
            {
                ScilabHistory->setFilename(expandedPath);
                FREE(expandedPath);
            }
            else
            {
                ScilabHistory->setFilename(filename);
            }

            return TRUE;
        }
    }
    return FALSE;
}

/*------------------------------------------------------------------------*/
char *getFilenameScilabHistory(void)
{
    if (ScilabHistory)
    {
        return ScilabHistory->getFilename();
    }
    return NULL;
}

/*------------------------------------------------------------------------*/
BOOL setDefaultFilenameScilabHistory(void)
{
    if (ScilabHistory)
    {
        return ScilabHistory->setDefaultFilename();
    }
    return FALSE;
}

/*------------------------------------------------------------------------*/
void resetScilabHistory(void)
{
    if (ScilabHistory)
    {
        ScilabHistory->reset();
    }
}

/*------------------------------------------------------------------------*/
char **getAllLinesOfScilabHistory(void)
{
    int nbElements = 0;
    char **lines = NULL;

    if (ScilabHistory)
    {
        lines = ScilabHistory->getAllLines(&nbElements);
        /* SWIG need array finish with NULL */
        if (lines)
        {
            lines = (char **)REALLOC(lines, sizeof(char *) * (nbElements + 1));
            lines[nbElements] = NULL;
        }
    }
    return lines;
}

/*------------------------------------------------------------------------*/
int getSizeAllLinesOfScilabHistory(void)
{
    int nbElements = 0;
    char **lines = NULL;

    if (ScilabHistory)
    {
        lines = ScilabHistory->getAllLines(&nbElements);
    }

    freeArrayOfString(lines, nbElements);

    return nbElements;
}

/*------------------------------------------------------------------------*/
char *getLastLineInScilabHistory(void)
{
    if (ScilabHistory)
    {
        return ScilabHistory->getLastLine();
    }
    return NULL;
}

/*------------------------------------------------------------------------*/
char *getPreviousLineInScilabHistory(void)
{
    if (ScilabHistory)
    {
        return ScilabHistory->getPreviousLine();
    }
    return NULL;
}

/*------------------------------------------------------------------------*/
char *getNextLineInScilabHistory(void)
{
    if (ScilabHistory)
    {
        return ScilabHistory->getNextLine();
    }
    return NULL;
}

/*------------------------------------------------------------------------*/
int getNumberOfLinesInScilabHistory(void)
{
    if (ScilabHistory)
    {
        return ScilabHistory->getNumberOfLines();
    }
    return 0;
}

/*------------------------------------------------------------------------*/
void setSaveConsecutiveDuplicateLinesInScilabHistory(BOOL doit)
{
    if (ScilabHistory)
    {
        ScilabHistory->setSaveConsecutiveDuplicateLines(doit);
    }
}

/*------------------------------------------------------------------------*/
BOOL getSaveConsecutiveDuplicateLinesInScilabHistory(void)
{
    if (ScilabHistory)
    {
        return ScilabHistory->getSaveConsecutiveDuplicateLines();
    }
    return FALSE;
}

/*------------------------------------------------------------------------*/
void setAfterHowManyLinesScilabHistoryIsSaved(int num)
{
    if (ScilabHistory)
    {
        ScilabHistory->setAfterHowManyLinesHistoryIsSaved(num);
    }
}

/*------------------------------------------------------------------------*/
int getAfterHowManyLinesScilabHistoryIsSaved(void)
{
    if (ScilabHistory)
    {
        return ScilabHistory->getAfterHowManyLinesHistoryIsSaved();
    }
    return 0;
}

/*------------------------------------------------------------------------*/
char *getNthLineInScilabHistory(int N)
{
    if (ScilabHistory)
    {
        return ScilabHistory->getNthLine(N);
    }
    return NULL;
}

/*------------------------------------------------------------------------*/
BOOL deleteNthLineScilabHistory(int N)
{
    if (ScilabHistory)
    {
        return ScilabHistory->deleteNthLine(N);
    }
    return FALSE;
}

/*------------------------------------------------------------------------*/
int getSizeScilabHistory(void)
{
    if (ScilabHistory)
    {
        return ScilabHistory->getNumberOfLines() - 1;
    }
    return 0;
}

/*------------------------------------------------------------------------*/
BOOL setSizeMaxScilabHistory(int nbLinesMax)
{
    if (ScilabHistory)
    {
        return ScilabHistory->setNumberOfLinesMax(nbLinesMax);
    }
    return FALSE;
}
/*------------------------------------------------------------------------*/
int getSizeMaxScilabHistory(void)
{
    if (ScilabHistory)
    {
        return ScilabHistory->getNumberOfLinesMax();
    }
    return 0;
}
/*------------------------------------------------------------------------*/
HistoryManager::HistoryManager()
{
    bTruncated = FALSE;
    CommandsList.clear();
    saveconsecutiveduplicatelines = FALSE;
    afterhowmanylineshistoryissaved = 0;
    numberoflinesbeforehistoryissaved = 0;

    CommandHistoryInitialize();
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

            if ((previousline) && (strcmp(previousline, cline) == 0))
            {
                bOK = FALSE;
            }
            else
            {
                CommandLine Line(cline);

                CommandsList.push_back(Line);
                numberoflinesbeforehistoryissaved++;
                bOK = TRUE;

                CommandHistoryAppendLine(cline);
            }
            if (previousline)
            {
                FREE(previousline);
                previousline = NULL;
            }
        }
        else
        {
            CommandLine Line(cline);

            CommandsList.push_back(Line);

            numberoflinesbeforehistoryissaved++;
            bOK = TRUE;

            CommandHistoryAppendLine(cline);
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
BOOL HistoryManager::appendLines(char **lines, int nbrlines)
{
    BOOL bOK = TRUE;
    int i = 0;

    for (i = 0; i < nbrlines; i++)
    {
        if ((lines[i] == NULL) || (!appendLine(lines[i])))
        {
            bOK = FALSE;
        }
    }
    return bOK;
}

/*------------------------------------------------------------------------*/
void HistoryManager::displayHistory(void)
{
    int nbline = 0;

    list < CommandLine >::iterator it_commands;
    for (it_commands = CommandsList.begin(); it_commands != CommandsList.end(); ++it_commands)
    {
        std::string line = (*it_commands).get();
        if (!line.empty())
        {
            sciprint("%d : %s\n", nbline, line.c_str());
            nbline++;
        }
    }
}

/*------------------------------------------------------------------------*/
char *HistoryManager::getFilename(void)
{
    if (!my_file.getFilename().empty())
    {
        return strdup(my_file.getFilename().c_str());
    }
    return NULL;
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

    if (filename)
    {
        char *commentbeginsession = NULL;

        std::string name;
        name.assign(filename);

        if (my_file.loadFromFile(name) == HISTORY_TRUNCATED)
        {
            bTruncated = TRUE;
        }

        CommandsList.clear();
        CommandsList = my_file.getHistory();

        if (CommandsList.size() > 0)
        {
            char *firstLine = getFirstLine();

            if (firstLine)
            {
                if (!isBeginningSessionLine(firstLine))
                {
                    fixHistorySession();
                }
                FREE(firstLine);
                firstLine = NULL;
            }
        }

        /* add date & time @ begin session */
        commentbeginsession = getCommentDateSession(FALSE);
        appendLine(commentbeginsession);
        FREE(commentbeginsession);
        commentbeginsession = NULL;

        CommandHistoryLoadFromFile();

        return TRUE;
    }
    return FALSE;
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

    CommandHistoryReset();

    /* Add date & time begin session */
    commentbeginsession = getCommentDateSession(FALSE);
    if (commentbeginsession)
    {
        appendLine(commentbeginsession);
        FREE(commentbeginsession);
        commentbeginsession = NULL;
    }

}

/*--------------------------------------------------------------------------*/
char **HistoryManager::getAllLines(int *numberoflines)
{
    char **lines = NULL;

    *numberoflines = 0;

    if (CommandsList.empty())
    {
        return lines;
    }
    else
    {
        list < CommandLine >::iterator it_commands;
        int i = 0;

        lines = (char **)MALLOC((int)CommandsList.size() * (sizeof(char *)));
        for (it_commands = CommandsList.begin(); it_commands != CommandsList.end(); ++it_commands)
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
char *HistoryManager::getFirstLine(void)
{
    char *line = NULL;

    if (!CommandsList.empty())
    {
        std::string str;
        list < CommandLine >::iterator it_commands = CommandsList.begin();
        str = (*it_commands).get();
        if (!str.empty())
        {
            line = strdup(str.c_str());
        }
    }
    return line;
}

/*--------------------------------------------------------------------------*/
char *HistoryManager::getLastLine(void)
{
    char *line = NULL;

    if (!CommandsList.empty())
    {
        std::string str;
        list < CommandLine >::iterator it_commands = CommandsList.end();
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

    if (N < 0)
    {
        N = getNumberOfLines() + N;
    }

    if ((N >= 0) && (N <= getNumberOfLines()))
    {
        int i = 0;

        list < CommandLine >::iterator it_commands;
        for (it_commands = CommandsList.begin(); it_commands != CommandsList.end(); ++it_commands)
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
    if ((N >= 0) && (N <= getNumberOfLines()))
    {
        int i = 0;

        list<CommandLine>::iterator it_start, it_end;
        for (it_start = CommandsList.begin(); it_start != CommandsList.end(); ++it_start)
        {
            if (i == N)
            {
                it_end = it_start;
                it_end++;
                if (isBeginningSessionLine(*it_start))
                {
                    for (; it_end != CommandsList.end(); ++it_end)
                    {
                        if (isBeginningSessionLine(*it_end))
                        {
                            break;
                        }
                    }
                }

                CommandsList.erase(it_start, it_end);

                my_search.setHistory(CommandsList);
                my_search.setToken(std::string());

                CommandHistoryDeleteLine(N);
                return TRUE;
            }
            i++;
        }
    }
    return FALSE;
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
    std::string Token;
    if (token)
    {
        Token.assign(token);
    }
    my_search.setHistory(CommandsList);
    return my_search.setToken(Token);
}

/*--------------------------------------------------------------------------*/
char *HistoryManager::getToken(void)
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

BOOL HistoryManager::isBeginningSessionLine(CommandLine& line)
{
    std::string str = line.get();
    return isBeginningSessionLine(str.c_str());
}

/*--------------------------------------------------------------------------*/
BOOL HistoryManager::isBeginningSessionLine(const char *line)
{
    if (line)
    {
        size_t len_begin = strlen(SESSION_PRAGMA_BEGIN);
        size_t len_end = strlen(SESSION_PRAGMA_END);
        size_t len_line = strlen(line);

        if (len_line > len_begin + len_end)
        {
            if ((strncmp(line, SESSION_PRAGMA_BEGIN, len_begin) == 0) &&
                    (strncmp(line + len_line - len_end, SESSION_PRAGMA_END, len_end) == 0))
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}



/*--------------------------------------------------------------------------*/
void HistoryManager::fixHistorySession(void)
{
    /* add date & time @ begin session */
    char *commentbeginsession = getCommentDateSession(FALSE);

    if (commentbeginsession)
    {
        CommandLine Line(commentbeginsession);

        CommandsList.push_front(Line);
        FREE(commentbeginsession);
        commentbeginsession = NULL;
    }
}

/*--------------------------------------------------------------------------*/
BOOL HistoryManager::isTruncated(void)
{
    return bTruncated;
}
/*--------------------------------------------------------------------------*/
BOOL HistoryManager::setNumberOfLinesMax(int nbLinesMax)
{
    return my_file.setDefaultMaxNbLines(nbLinesMax);
}
/*--------------------------------------------------------------------------*/
int HistoryManager::getNumberOfLinesMax(void)
{
    return my_file.getDefaultMaxNbLines();
}
/*--------------------------------------------------------------------------*/
