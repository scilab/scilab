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
#include <alltypes.hxx>
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

#define MODULE_DIR  "/modules/"
#define MACRO_DIR   "/macros/"
#define ETC_DIR     "/etc/"
#define SCI_EXT     "*.sci"
#define START_EXT   ".start"
#define END_EXT     ".end"

//Gateway function pointer
typedef bool (*GW_MOD)(void); 

class EXTERN_FUNC_MAN FuncManager
{
private : 
//	map <string, FuncInfo*>	m_FuncMap;
	map <string, GW_MOD>    m_ModuleMap;
	map <string, GW_MOD>    m_ActivModuleMap;
	list <string>           m_ModuleName;
	string                  m_szXmlFile;
    bool                    m_bNoStart;
public:
	FuncManager(void);
	~FuncManager(void);

    bool LoadModules(bool _bNoStart);

private :
	bool GetModules();
	bool AppendModules();
	bool VerifyModule(char *ModuleName);

	bool CreateModuleList(void);
	bool LoadFuncByModule(void);

	bool LoadMacroFile(string _stModule);
    bool ExecuteStartFile(string _stModule);

};

#endif //__FUNCMANAGER_HH__

