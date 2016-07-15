/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007-2008 - INRIA - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Vincent COUVERT
* Copyright (C) 2011 - DIGITEO - Allan CORNET
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
#include "HistoryManager.hxx"
/*------------------------------------------------------------------------*/
extern "C"
{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sciprint.h"
#include "sci_home.h"
#include "InitializeHistoryManager.h"
#include "TerminateHistoryManager.h"
#include "freeArrayOfString.h"
#include "os_string.h"
#include "CommandHistory_Wrap.h"
#include "localization.h"
#include "getCommentDateSession.h"
#include "sci_malloc.h"
#include "HistoryManager.h"
};
/*------------------------------------------------------------------------*/
#define MAXBUF	1024
/*------------------------------------------------------------------------*/
HistoryManager* HistoryManager::m_pHM = NULL;
/*------------------------------------------------------------------------*/
HistoryManager* HistoryManager::getInstance()
{
    if (m_pHM == NULL)
    {
        m_pHM = new HistoryManager();

        /* add date & time @ begin session */
        char *commentbeginsession = getCommentDateSession(FALSE);
        if (commentbeginsession)
        {
            appendLineToScilabHistory(commentbeginsession);
            FREE(commentbeginsession);
            commentbeginsession = NULL;
        }

        m_pHM->setToken("");

    }
    return m_pHM;
}
/*------------------------------------------------------------------------*/
void HistoryManager::killInstance(void)
{
    if (m_pHM)
    {
        delete m_pHM;
        m_pHM = NULL;
    }
}
/*------------------------------------------------------------------------*/
BOOL historyIsEnabled(void)
{
    return HistoryManager::historyIsEnabled();
}
/*------------------------------------------------------------------------*/
BOOL InitializeHistoryManager(void)
{
    if (HistoryManager::getInstance())
    {
        return TRUE;
    }
    return FALSE;
}
/*------------------------------------------------------------------------*/
BOOL TerminateHistoryManager(void)
{
    HistoryManager::getInstance()->killInstance();
    return TRUE;
}
/*------------------------------------------------------------------------*/
BOOL setSearchedTokenInScilabHistory(char* _pstToken)
{
    return HistoryManager::getInstance()->setToken(_pstToken);
}
/*------------------------------------------------------------------------*/
BOOL resetSearchedTokenInScilabHistory(void)
{
    return HistoryManager::getInstance()->resetToken();
}
/*------------------------------------------------------------------------*/
char* getSearchedTokenInScilabHistory(void)
{
    return HistoryManager::getInstance()->getToken();
}
/*------------------------------------------------------------------------*/
BOOL appendLineToScilabHistory(char* _pstLine)
{
    return HistoryManager::getInstance()->appendLine(_pstLine);
}
/*------------------------------------------------------------------------*/
BOOL appendLinesToScilabHistory(char** _pstLines, int _iLines)
{
    for (int i = 0 ; i < _iLines ; i++)
    {
        if (HistoryManager::getInstance()->appendLine(_pstLines[i]) == FALSE)
        {
            return FALSE;
        }
    }
    return TRUE;
}
/*------------------------------------------------------------------------*/
void displayScilabHistory(void)
{
    HistoryManager::getInstance()->displayHistory();
}
/*------------------------------------------------------------------------*/
BOOL writeScilabHistoryToFile(char* _pstFilename)
{
    return HistoryManager::getInstance()->writeToFile(_pstFilename);
}
/*------------------------------------------------------------------------*/
BOOL loadScilabHistoryFromFile(char* _pstFilename)
{
    return HistoryManager::getInstance()->loadFromFile(_pstFilename);
}
/*------------------------------------------------------------------------*/
BOOL isScilabHistoryTruncated(void)
{
    return HistoryManager::getInstance()->isTruncated();
}
/*------------------------------------------------------------------------*/
BOOL setFilenameScilabHistory(char* _pstFilename)
{
    return HistoryManager::getInstance()->setFilename(_pstFilename);
}
/*------------------------------------------------------------------------*/
char* getFilenameScilabHistory(void)
{
    return HistoryManager::getInstance()->getFilename();
}
/*------------------------------------------------------------------------*/
BOOL setDefaultFilenameScilabHistory(void)
{
    return HistoryManager::getInstance()->setDefaultFilename();
}
/*------------------------------------------------------------------------*/
BOOL resetScilabHistory(void)
{
    return HistoryManager::getInstance()->reset();
}
/*------------------------------------------------------------------------*/
char** getAllLinesOfScilabHistory(void)
{
    int iItems = 0;
    return HistoryManager::getInstance()->getAllLines(&iItems);
}
/*------------------------------------------------------------------------*/
int getSizeAllLinesOfScilabHistory(void)
{
    return HistoryManager::getInstance()->getNumberOfLines();
}
/*------------------------------------------------------------------------*/
char* getLastLineInScilabHistory(void)
{
    return HistoryManager::getInstance()->getLastLine();
}
/*------------------------------------------------------------------------*/
char* getPreviousLineInScilabHistory(void)
{
    return HistoryManager::getInstance()->getPreviousLine();
}
/*------------------------------------------------------------------------*/
char* getNextLineInScilabHistory(void)
{
    return HistoryManager::getInstance()->getNextLine();
}
/*------------------------------------------------------------------------*/
int getNumberOfLinesInScilabHistory(void)
{
    return HistoryManager::getInstance()->getNumberOfLines();
}
/*------------------------------------------------------------------------*/
void setSaveConsecutiveDuplicateLinesInScilabHistory(BOOL _bAllow)
{
    HistoryManager::getInstance()->setSaveConsecutiveDuplicateLines(_bAllow);
}
/*------------------------------------------------------------------------*/
BOOL getSaveConsecutiveDuplicateLinesInScilabHistory(void)
{
    return HistoryManager::getInstance()->getSaveConsecutiveDuplicateLines();
}
/*------------------------------------------------------------------------*/
void setAfterHowManyLinesScilabHistoryIsSaved(int _iNum)
{
    HistoryManager::getInstance()->setAfterHowManyLinesHistoryIsSaved(_iNum);
}
/*------------------------------------------------------------------------*/
int getAfterHowManyLinesScilabHistoryIsSaved(void)
{
    return HistoryManager::getInstance()->getAfterHowManyLinesHistoryIsSaved();
}
/*------------------------------------------------------------------------*/
char* getNthLineInScilabHistory(int _iLine)
{
    return HistoryManager::getInstance()->getNthLine(_iLine);
}
/*------------------------------------------------------------------------*/
BOOL deleteNthLineScilabHistory(int _iLine)
{
    return HistoryManager::getInstance()->deleteNthLine(_iLine);
}
/*------------------------------------------------------------------------*/
int getSizeScilabHistory(void)
{
    return (HistoryManager::getInstance()->getNumberOfLines() - 1);
}
/*------------------------------------------------------------------------*/
BOOL setSizeMaxScilabHistory(int _iMaxLines)
{
    return HistoryManager::getInstance()->setNumberOfLinesMax(_iMaxLines);
}
/*------------------------------------------------------------------------*/
int getSizeMaxScilabHistory(void)
{
    return HistoryManager::getInstance()->getNumberOfLinesMax();
}
/*------------------------------------------------------------------------*/
HistoryManager::HistoryManager()
{
    m_bTruncated                = FALSE;
    m_bAllowConsecutiveCommand  = FALSE;
    m_iSaveLimit                = 0;
    m_iSavedLines               = 0;
    m_Commands.clear();

    CommandHistoryInitialize();
}
/*------------------------------------------------------------------------*/
HistoryManager::~HistoryManager()
{
    m_Commands.clear();
}
/*------------------------------------------------------------------------*/
BOOL HistoryManager::historyIsEnabled(void)
{
    if (m_pHM)
    {
        return TRUE;
    }
    return FALSE;
}
/*------------------------------------------------------------------------*/
BOOL HistoryManager::appendLine(char* _pstLine)
{
    BOOL bOK = FALSE;
    if (_pstLine)
    {
        int i                   = 0;
        int len                 = 0;
        char* pstCleanedLine    = NULL;

        /* remove space & carriage return at the end of line */
        len = (int)strlen(_pstLine);
        pstCleanedLine = (char*) MALLOC(len + 1);
        memcpy(pstCleanedLine, _pstLine, len + 1);

        /* remove carriage return at the end of line */
        for (i = len ; i > 0 ; i--)
        {
            if (pstCleanedLine[i] == '\n')
            {
                pstCleanedLine[i] = '\0';
                len = i - 1;
                break;
            }
        }

        /* remove spaces at the end of line */
        i = len;
        while (i >= 0)
        {
            if (pstCleanedLine[i] == ' ')
            {
                pstCleanedLine[i] = '\0';
            }
            else
            {
                break;
            }
            i--;
        }

        if (strlen(pstCleanedLine) == 0)
        {
            FREE(pstCleanedLine);
            return TRUE;
        }

        // append
        if (m_bAllowConsecutiveCommand)
        {
            m_Commands.push_back(pstCleanedLine);
            m_iSavedLines++;
            bOK = TRUE;
            CommandHistoryAppendLine(pstCleanedLine);
        }
        else
        {
            char* pstPreviousLine = getLastLine();
            if (pstPreviousLine && strcmp(pstPreviousLine, pstCleanedLine) == 0)
            {
                bOK = TRUE;
            }
            else
            {
                m_Commands.push_back(pstCleanedLine);
                m_iSavedLines++;
                bOK = TRUE;

                CommandHistoryAppendLine(pstCleanedLine);
            }
            if (pstPreviousLine)
            {
                FREE(pstPreviousLine);
                pstPreviousLine = NULL;
            }
        }

        if (m_iSaveLimit != 0)
        {
            if (m_iSavedLines >= m_iSaveLimit)
            {
                m_HF.setHistory(m_Commands);
                m_HF.writeToFile();
                m_iSavedLines = 0;
            }
        }
        else
        {
            m_iSavedLines = 0;
        }

        FREE(pstCleanedLine);
        pstCleanedLine = NULL;
    }

    return bOK;
}
/*------------------------------------------------------------------------*/
BOOL HistoryManager::appendLines(char** _pstLines, int _iLines)
{
    for (int i = 0 ; i < _iLines ; i++)
    {
        if (appendLine(_pstLines[i]) == FALSE)
        {
            return FALSE;
        }
    }
    return TRUE;
}
/*------------------------------------------------------------------------*/
void HistoryManager::displayHistory(void)
{
    int nbline = 0;
    std::list<std::string>::const_iterator it;
    for (it = m_Commands.begin() ; it != m_Commands.end() ; it++)
    {
        sciprint(_("%d : %s\n"), nbline++, (*it).c_str());
    }
}
/*------------------------------------------------------------------------*/
char* HistoryManager::getFilename(void)
{
    if (m_HF.getFilename().empty() == false)
    {
        return os_strdup(m_HF.getFilename().c_str());
    }
    return NULL;
}
/*------------------------------------------------------------------------*/
BOOL HistoryManager::setFilename(char* _pstFilename)
{
    if (_pstFilename)
    {
        m_HF.setFilename(_pstFilename);
        return TRUE;
    }
    return FALSE;
}
/*------------------------------------------------------------------------*/
BOOL HistoryManager::setDefaultFilename(void)
{
    return m_HF.setDefaultFilename();
}
/*------------------------------------------------------------------------*/
BOOL HistoryManager::writeToFile(char* _pstFilename)
{
    if (_pstFilename)
    {
        m_HF.setHistory(m_Commands);
        return m_HF.writeToFile(_pstFilename);
    }
    return FALSE;
}
/*------------------------------------------------------------------------*/
BOOL HistoryManager::loadFromFile(char* _pstFilename)
{
    if (_pstFilename)
    {
        char* pstCommentBeginSession = NULL;
        if (m_HF.loadFromFile(_pstFilename) == HISTORY_TRUNCATED)
        {
            m_bTruncated = TRUE;
        }

        m_Commands.clear();
        m_Commands = m_HF.getHistory();

        if (m_Commands.size() > 0)
        {
            char* pstFirstLine = getFirstLine();
            if (pstFirstLine)
            {
                if (!isBeginningSessionLine(pstFirstLine))
                {
                    fixHistorySession();
                }
                FREE(pstFirstLine);
                pstFirstLine = NULL;
            }
        }

        /* add date & time @ begin session */
        pstCommentBeginSession = getCommentDateSession(FALSE);
        appendLine(pstCommentBeginSession);
        FREE(pstCommentBeginSession);
        pstCommentBeginSession = NULL;

        CommandHistoryLoadFromFile();

        return TRUE;
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL HistoryManager::reset(void)
{
    char* pstCommentBeginSession = NULL;

    m_Commands.clear();

    m_HF.reset();
    m_HF.setDefaultFilename();

    m_HS.reset();

    m_bAllowConsecutiveCommand  = FALSE;
    m_iSaveLimit                = 0;
    m_iSavedLines               = 0;

    CommandHistoryReset();

    /* Add date & time begin session */
    pstCommentBeginSession = getCommentDateSession(FALSE);
    if (pstCommentBeginSession)
    {
        appendLine(pstCommentBeginSession);
        FREE(pstCommentBeginSession);
        pstCommentBeginSession = NULL;
        return TRUE;
    }

    return FALSE;
}
/*--------------------------------------------------------------------------*/
char** HistoryManager::getAllLines(int* _piLines)
{
    char** pstLines = NULL;
    *_piLines       = 0;

    if (m_Commands.empty())
    {
        return pstLines;
    }
    else
    {
        std::list<std::string>::const_iterator it;

        pstLines = (char**)MALLOC((int)(m_Commands.size() + 1) * (sizeof(char*)));
        for (it = m_Commands.begin() ; it != m_Commands.end(); ++it)
        {
            pstLines[(*_piLines)++] = os_strdup((*it).c_str());
        }

        /* SWIG need array finish with NULL */
        pstLines[(*_piLines)] = NULL;
    }
    return pstLines;
}
/*--------------------------------------------------------------------------*/
char* HistoryManager::getFirstLine(void)
{
    if (m_Commands.empty() == false)
    {
        return os_strdup(m_Commands.front().c_str());
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/
char* HistoryManager::getLastLine(void)
{
    if (m_Commands.empty() == false)
    {
        return os_strdup(m_Commands.back().c_str());
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/
int HistoryManager::getNumberOfLines(void)
{
    return (int)m_Commands.size();
}
/*--------------------------------------------------------------------------*/
char* HistoryManager::getNthLine(int _iLine)
{
    if (_iLine < 0)
    {
        //reverse search Oo
        _iLine = getNumberOfLines() + _iLine;
    }

    if (_iLine >= 0 && _iLine <= getNumberOfLines())
    {
        int i = 0;
        std::list<std::string>::const_iterator it;
        for (it = m_Commands.begin() ; it != m_Commands.end() ; it++)
        {
            if (i == _iLine)
            {
                return os_strdup((*it).c_str());
            }
            i++;
        }
    }

    return NULL;
}
/*--------------------------------------------------------------------------*/
BOOL HistoryManager::deleteNthLine(int _iLine)
{
    if (_iLine >= 0 && _iLine <= getNumberOfLines())
    {
        int i = 0;
        std::list<std::string>::iterator it;
        for (it = m_Commands.begin() ; it != m_Commands.end(); it++)
        {
            if (i == _iLine)
            {
                m_Commands.erase(it);
                // After a remove , we update search
                m_HS.setHistory(m_Commands);
                m_HS.setToken("");

                CommandHistoryDeleteLine(_iLine);
                return TRUE;
            }
            i++;
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
void HistoryManager::setSaveConsecutiveDuplicateLines(BOOL _bAllow)
{
    m_bAllowConsecutiveCommand = _bAllow;
}
/*--------------------------------------------------------------------------*/
BOOL HistoryManager::getSaveConsecutiveDuplicateLines(void)
{
    return m_bAllowConsecutiveCommand;
}
/*--------------------------------------------------------------------------*/
void HistoryManager::setAfterHowManyLinesHistoryIsSaved(int _iSaveLimit)
{
    if (_iSaveLimit >= 0)
    {
        m_iSaveLimit    = _iSaveLimit;
        m_iSavedLines   = 0;
    }
}
/*--------------------------------------------------------------------------*/
int HistoryManager::getAfterHowManyLinesHistoryIsSaved(void)
{
    return m_iSaveLimit;
}
/*--------------------------------------------------------------------------*/
char* HistoryManager::getPreviousLine(void)
{
    if (m_HS.getSize() > 0)
    {
        std::string stLine = m_HS.getPreviousLine();
        if (stLine.empty() == false)
        {
            return os_strdup(stLine.c_str());
        }
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/
char* HistoryManager::getNextLine(void)
{
    if (m_HS.getSize() > 0)
    {
        std::string stLine = m_HS.getNextLine();
        return os_strdup(stLine.c_str());
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/
BOOL HistoryManager::setToken(const char* _pstToken)
{
    m_HS.setHistory(m_Commands);

    if (_pstToken)
    {
        return m_HS.setToken(_pstToken);
    }
    else
    {
        return m_HS.setToken("");
    }
}
/*--------------------------------------------------------------------------*/
char* HistoryManager::getToken(void)
{
    std::string stToken = m_HS.getToken();
    if (stToken.empty() == false)
    {
        return os_strdup(stToken.c_str());
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/
BOOL HistoryManager::resetToken(void)
{
    return m_HS.reset();
}
/*--------------------------------------------------------------------------*/
BOOL HistoryManager::isBeginningSessionLine(char* _pstLine)
{
    if (_pstLine)
    {
        if (strlen(_pstLine) > strlen(SESSION_PRAGMA_BEGIN) + strlen(SESSION_PRAGMA_END))
        {
            if ((strncmp(   _pstLine,
                            SESSION_PRAGMA_BEGIN,
                            strlen(SESSION_PRAGMA_BEGIN)) == 0) &&
                    (strncmp(    _pstLine + strlen(_pstLine) - strlen(SESSION_PRAGMA_END),
                                 SESSION_PRAGMA_END,
                                 strlen(SESSION_PRAGMA_END)) == 0))
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
    char* pstCommentBeginSession = getCommentDateSession(FALSE);
    if (pstCommentBeginSession)
    {
        m_Commands.push_front(pstCommentBeginSession);
        FREE(pstCommentBeginSession);
        pstCommentBeginSession = NULL;
    }
}
/*--------------------------------------------------------------------------*/
BOOL HistoryManager::isTruncated(void)
{
    return m_bTruncated;
}
/*--------------------------------------------------------------------------*/
BOOL HistoryManager::setNumberOfLinesMax(int _iMaxLines)
{
    return m_HF.setDefaultMaxNbLines(_iMaxLines);
}
/*--------------------------------------------------------------------------*/
int HistoryManager::getNumberOfLinesMax(void)
{
    return m_HF.getDefaultMaxNbLines();
}
/*--------------------------------------------------------------------------*/
