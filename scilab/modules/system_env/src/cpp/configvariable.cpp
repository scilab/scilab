/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "configvariable.hxx"

/*
** Module List
** \{
*/
list<wstring> ConfigVariable::m_ModuleList;

void ConfigVariable::setModuleList(list<wstring>& _pModule_list)
{
    m_ModuleList.clear();

    list<wstring>::iterator it;
    for(it = _pModule_list.begin() ; it != _pModule_list.end() ; it++)
    {
        m_ModuleList.push_back(*it);
    }
}

list<wstring> ConfigVariable::getModuleList()
{
    list<wstring> moduleList;
    list<wstring>::iterator it;
    for(it = m_ModuleList.begin() ; it != m_ModuleList.end() ; it++)
    {
        moduleList.push_back(*it);
    }
    return moduleList;
}
/*
** \}
*/

/*
** SCI
** \{
*/
wstring ConfigVariable::m_SCIPath;

void ConfigVariable::setSCIPath(wstring& _SCIPath)
{
    m_SCIPath = _SCIPath;
}

wstring ConfigVariable::getSCIPath()
{
    return m_SCIPath;
}

/*
** \}
*/

/*
** SCIHOME
** \{
*/

wstring ConfigVariable::m_SCIHOME;

void ConfigVariable::setSCIHOME(wstring& _SCIHOME)
{
    m_SCIHOME = _SCIHOME;
}

wstring ConfigVariable::getSCIHOME()
{
    return m_SCIHOME;
}
/*
** \}
*/

/*
** TMPDIR
** \{
*/

wstring ConfigVariable::m_TMPDIR;

void ConfigVariable::setTMPDIR(wstring& _TMPDIR)
{
    m_TMPDIR = _TMPDIR;
}

wstring ConfigVariable::getTMPDIR()
{
    return m_TMPDIR;
}
/*
** \}
*/

/*
** Force Quit
** \{
*/
bool ConfigVariable::m_bForceQuit = false;

void ConfigVariable::setForceQuit(bool _bForceQuit)
{
    m_bForceQuit = _bForceQuit;
}

bool ConfigVariable::getForceQuit(void)
{
    return m_bForceQuit;
}
/*
** \}
*/

/*
** Exit Status
** \{
*/
int ConfigVariable::m_iExitStatus = 0;

void ConfigVariable::setExitStatus(int _iExitStatus)
{
    m_iExitStatus = _iExitStatus;
}

int ConfigVariable::getExitStatus(void)
{
    return m_iExitStatus;
}
/*
** \}
*/

/*
** Digit precision, ex format function
** \{
*/

int ConfigVariable::m_iFormat = 0;

void ConfigVariable::setFormat(int _iFormat)
{
    m_iFormat = _iFormat;
}

int ConfigVariable::getFormat(void)
{
    return m_iFormat;
}

/*
** \}
*/

/*
** Screen console width
** \{
*/

int ConfigVariable::m_iConsoleWidth = 0;

void ConfigVariable::setConsoleWidth(int _iConsoleWidth)
{
    m_iConsoleWidth = _iConsoleWidth;
}

int ConfigVariable::getConsoleWidth(void)
{
    return m_iConsoleWidth;
}
/*
** \}
*/

/*
** Scilab mode
** \{
*/

int ConfigVariable::m_iScilabMode = 0;

void ConfigVariable::setScilabMode(int _iScilabMode)
{
    m_iScilabMode = _iScilabMode;
}

int ConfigVariable::getScilabMode(void)
{
    return m_iScilabMode;
}
/*
** \}
*/

/*
** Warning Mode
** \{
*/
bool ConfigVariable::m_bWarningMode = true;

void ConfigVariable::setWarningMode(bool _bWarningMode)
{
    m_bWarningMode = _bWarningMode;
}

bool ConfigVariable::getWarningMode(void)
{
    return m_bWarningMode;
}
/*
** \}
*/

/*
** HOME
** \{
*/

wstring ConfigVariable::m_HOME;

void ConfigVariable::setHOME(wstring& _HOME)
{
    m_HOME = _HOME;
}

wstring ConfigVariable::getHOME()
{
    return m_HOME;
}
/*
** \}
*/

/*
** Clear last error information
** \{
*/
bool ConfigVariable::m_bLastErrorCall = false;

void ConfigVariable::setLastErrorCall(void)
{
    m_bLastErrorCall = true;
}

void ConfigVariable::clearLastError(void)
{
    if(m_bLastErrorCall == false)
    {
        m_wstError          = L"";
        m_iError            = 0;
        m_iErrorLine        = 0;
        m_wstErrorFunction  = L"";
    }
    m_bLastErrorCall = false;
}
/*
** \}
*/

/*
** Last Error Message
** \{
*/

wstring ConfigVariable::m_wstError;

void ConfigVariable::setLastErrorMessage(wstring _wstError)
{
    m_wstError = _wstError;
}

wstring ConfigVariable::getLastErrorMessage()
{
    return m_wstError;
}
/*
** \}
*/

/*
** Last Error ID
** \{
*/
int ConfigVariable::m_iError = 0;

void ConfigVariable::setLastErrorNumber(int _iError)
{
    m_iError = _iError;
}

int ConfigVariable::getLastErrorNumber(void)
{
    return m_iError;
}
/*
** \}
*/

/*
** Last Error Line
** \{
*/
int ConfigVariable::m_iErrorLine = 0;

void ConfigVariable::setLastErrorLine(int _iErrorLine)
{
    m_iErrorLine = _iErrorLine;
}

int ConfigVariable::getLastErrorLine(void)
{
    return m_iErrorLine;
}
/*
** \}
*/

/*
** Last Error Function
** \{
*/

wstring ConfigVariable::m_wstErrorFunction;

void ConfigVariable::setLastErrorFunction(wstring _wstErrorFunction)
{
    m_wstErrorFunction = _wstErrorFunction;
}

wstring ConfigVariable::getLastErrorFunction()
{
    return m_wstErrorFunction;
}

/*
** \}
*/

/*
** Prompt Mode
** \{
*/

int ConfigVariable::m_iPromptMode = 0;

void ConfigVariable::setPromptMode(int _iPromptMode)
{
    m_iPromptMode = _iPromptMode;
}

int ConfigVariable::getPromptMode(void)
{
    return m_iPromptMode;
}

/*
** \}
*/
