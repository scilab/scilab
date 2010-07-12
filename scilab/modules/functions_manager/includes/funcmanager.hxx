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
//#include "scilabfunction.hxx"


#ifdef _MSC_VER
	#if FUNC_MAN_EXPORTS
		#define EXTERN_FUNC_MAN __declspec (dllexport)
	#else
		#define EXTERN_FUNC_MAN __declspec (dllimport)
	#endif
#else
	#define EXTERN_FUNC_MAN 
#endif

using namespace std;

#define MODULE_DIR  L"/modules/"
#define MACRO_DIR   L"/macros/"
#define ETC_DIR     L"/etc/"
#define SCI_EXT     L"*.sci"
#define START_EXT   L".start"
#define END_EXT     L".end"

//Gateway function pointer
typedef bool (*GW_MOD)(void); 

class EXTERN_FUNC_MAN FuncManager
{
private : 
//	map <string, FuncInfo*>	m_FuncMap;
	map<wstring, GW_MOD>    m_ModuleMap;
	map<wstring, GW_MOD>    m_ActivModuleMap;
	list<wstring>           m_ModuleName;
	wstring                 m_szXmlFile;
    bool                    m_bNoStart;
public:
	FuncManager(void);
	~FuncManager(void);

    bool LoadModules(bool _bNoStart);

private :
	bool GetModules();
	bool AppendModules();
	bool VerifyModule(wchar_t* ModuleName);

	bool CreateModuleList(void);
	bool LoadFuncByModule(void);

    bool ExecuteStartFile(wstring _stModule);

};

#endif //__FUNCMANAGER_HH__

