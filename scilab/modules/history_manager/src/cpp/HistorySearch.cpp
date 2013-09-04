/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
extern "C"
{
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "os_strdup.h"
#include "MALLOC.h"
}
#include "HistorySearch.hxx"
/*------------------------------------------------------------------------*/
HistorySearch::HistorySearch()
{
    m_stToken = "dummy";
    m_vstLines.clear();
    m_iPosition = 0;
}
/*------------------------------------------------------------------------*/
HistorySearch::~HistorySearch()
{
    reset();
}
/*------------------------------------------------------------------------*/
BOOL HistorySearch::setHistory(std::list<std::string> _lstCommands)
{
    BOOL bOK = FALSE;
    std::list<std::string>::const_iterator it;

    if (m_Commands.empty() == false)
    {
        m_Commands.clear();
    }

    for (it = _lstCommands.begin() ; it != _lstCommands.end(); it++)
    {
        m_Commands.push_back((*it));
    }
    return bOK;
}
/*------------------------------------------------------------------------*/
BOOL HistorySearch::setToken(std::string _stToken)
{
    if (_stToken != m_stToken || m_stToken.empty() || _stToken.empty())
    {
        m_stToken = _stToken;
        return search();
    }
    return FALSE;
}
/*------------------------------------------------------------------------*/
std::string HistorySearch::getToken(void)
{
    return m_stToken;
}
/*------------------------------------------------------------------------*/
BOOL HistorySearch::search(void)
{
    BOOL bOK = FALSE;
    if (m_stToken.empty() == false)
    {
        std::list<std::string>::const_iterator it;

        m_vstLines.clear();

        for (it = m_Commands.begin() ; it != m_Commands.end(); it++)
        {
            std::string stLine = (*it);
            if (stLine.compare(0, m_stToken.size(), m_stToken) == 0 )
            {
                m_vstLines.push_back(stLine);
            }
        }
    }
    else
    {
        std::list<std::string>::const_iterator it;

        m_vstLines.clear();

        for (it = m_Commands.begin() ; it != m_Commands.end(); it++)
        {
            m_vstLines.push_back(*it);
        }
    }
    //current_position = i - 1;
    m_iPosition = (int)m_vstLines.size();
    return bOK;
}
/*------------------------------------------------------------------------*/
int HistorySearch::getSize(void)
{
    return (int)m_vstLines.size();
}
/*------------------------------------------------------------------------*/
BOOL HistorySearch::reset(void)
{
    m_stToken.erase();
    m_vstLines.clear();
    m_Commands.clear();
    m_iPosition = 0;

    return TRUE;
}
/*------------------------------------------------------------------------*/
std::string HistorySearch::getPreviousLine(void)
{
    std::string stLine;
    if (m_iPosition > 0)
    {
        m_iPosition--;
        stLine = m_vstLines[m_iPosition];
    }
    else
    {
        // m_iPosition <= 0
        stLine = m_vstLines[0];
    }
    return stLine;
}
/*------------------------------------------------------------------------*/
std::string HistorySearch::getNextLine(void)
{
    if (m_iPosition + 1 < m_vstLines.size())
    {
        //normal case
        return m_vstLines[++m_iPosition];
    }
    else if (m_iPosition + 1 == m_vstLines.size())
    {
        m_iPosition++;
    }
    return getToken();
}
/*------------------------------------------------------------------------*/

