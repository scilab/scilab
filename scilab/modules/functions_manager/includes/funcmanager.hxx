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

#ifndef __FUNCMANAGER_HH__
#define __FUNCMANAGER_HH__

#include <map>
#include <list>
#include <iostream>
#include <string>

using namespace std;


//Gateway function pointer
typedef int (*GW_FUNC)(void); 

//class container
class FuncInfo
{
public :
	FuncInfo(string _szName, GW_FUNC* _pFunc, string _szModule)
	{
	}
	~FuncInfo(){};

	string		m_szName;
	GW_FUNC*	m_pFunc;
	string		m_szModule;
};

class FuncManager
{
public:
	FuncManager(void);
	~FuncManager(void);

	bool GetModules();
	bool AppendModules();
	bool VerifyModule(char *ModuleName);



	map <string, FuncInfo*> m_mapFuncList;
	list <string> m_ModuleList;
	string m_szXmlFile;
};

#endif //__FUNCMANAGER_HH__