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

std::list<std::string> ConfigVariable::m_ModuleList;

bool ConfigVariable::setModuleList(std::list<std::string>& _pModule_list)
{
    m_ModuleList.clear();

    std::list<std::string>::iterator it;
    for(it = _pModule_list.begin() ; it != _pModule_list.end() ; it++)
    {
        m_ModuleList.push_back(*it);
    }
    return true;
}

std::list<std::string> ConfigVariable::getModuleList()
{
    std::list<std::string> moduleList;
    return moduleList;
}


std::string ConfigVariable::m_SCIPath;

bool ConfigVariable::setSCIPath(std::string& _SCIPath)
{
    m_SCIPath = _SCIPath;
    return true;
}

std::string ConfigVariable::getSCIPath()
{
    return m_SCIPath;
}

std::string ConfigVariable::m_HOMEPath;

bool ConfigVariable::setHOMEPath(std::string& _HOMEPath)
{
    m_HOMEPath = _HOMEPath;
    return true;
}

std::string ConfigVariable::getHOMEPath()
{
    return m_HOMEPath;
}
