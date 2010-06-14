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
std::list<std::string> ConfigVariable::m_ModuleList;

void ConfigVariable::setModuleList(std::list<std::string>& _pModule_list)
{
    m_ModuleList.clear();

    std::list<std::string>::iterator it;
    for(it = _pModule_list.begin() ; it != _pModule_list.end() ; it++)
    {
        m_ModuleList.push_back(*it);
    }
}

std::list<std::string> ConfigVariable::getModuleList()
{
    std::list<std::string> moduleList;
    return moduleList;
}
/*
** \}
*/

/*
** SCI
** \{
*/
std::string ConfigVariable::m_SCIPath;

void ConfigVariable::setSCIPath(std::string& _SCIPath)
{
    m_SCIPath = _SCIPath;
}

std::string ConfigVariable::getSCIPath()
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

std::string ConfigVariable::m_SCIHOME;

void ConfigVariable::setSCIHOME(std::string& _SCIHOME)
{
    m_SCIHOME = _SCIHOME;
}

std::string ConfigVariable::getSCIHOME()
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

std::string ConfigVariable::m_TMPDIR;

void ConfigVariable::setTMPDIR(std::string& _TMPDIR)
{
    m_TMPDIR = _TMPDIR;
}

std::string ConfigVariable::getTMPDIR()
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
