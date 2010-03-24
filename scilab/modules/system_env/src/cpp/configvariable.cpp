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

ConfigVariable* ConfigVariable::me;

ConfigVariable* ConfigVariable::getInstance()
{
	if (me == NULL)
	{
		me = new ConfigVariable();
	}
	return me;
}

std::string ConfigVariable::get(const std::string& _szVar)
{
	std::map<std::string, std::string>::const_iterator it;
	it = m_VarList.find(_szVar);
	if(it != m_VarList.end())
	{
		return it->second;
	}
	else
	{
		return "";
	}
}

bool ConfigVariable::set(std::string _szVar, std::string _szVal)
{
	bool bReturn = true;
	//std::map <std::string, std::string> ::const_iterator it;
	m_VarList.insert(std::pair<std::string,std::string>(_szVar,_szVal));
	//m_VarList[_szVar] = _szVal;
	return bReturn;
}
