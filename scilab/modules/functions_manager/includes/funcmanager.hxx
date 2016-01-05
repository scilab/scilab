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

//disable warnings about exports STL items
#pragma warning (disable : 4251)

#ifdef _MSC_VER
#if FUNC_MAN_EXPORTS
#define EXTERN_FUNC_MAN __declspec (dllexport)
#else
#define EXTERN_FUNC_MAN __declspec (dllimport)
#endif
#else
#define EXTERN_FUNC_MAN
#endif

#define MODULE_DIR  "/modules/"
#define MACRO_DIR   "/macros/"
#define ETC_DIR     "/etc/"
#define SCI_EXT     "*.sci"
#define START_EXT   ".start"
#define QUIT_EXT    ".quit"

//Gateway function pointer
typedef int (*GW_MOD)(void);
typedef int (*FINALIZE_MODULE)(void);

class EXTERN_FUNC_MAN FuncManager
{
private :
    //	map <string, FuncInfo*>	m_FuncMap;
    typedef std::map<std::string, std::pair<GW_MOD, GW_MOD> >  ModuleMap;
    ModuleMap  m_ModuleMap;
    std::map<std::string, GW_MOD> m_ActivModuleMap;
    std::list<std::string> m_ModuleName;
    std::string m_szXmlFile;
    bool m_bNoStart;
public:
    static FuncManager* getInstance();
    static void destroyInstance();

    bool LoadModules();
    bool UnloadModules();
    bool EndModules();

private :
    FuncManager(void);
    ~FuncManager(void);

    bool GetModules();
    bool AppendModules();
    bool VerifyModule(const char* ModuleName);

    bool CreateModuleList(void);

    bool ExecuteStartFile(const std::string& _stModule);
    bool ExecuteQuitFile(const std::string& _stModule);
    bool ExecuteFile(const std::string& _stFile);
    static FuncManager* me;
};

#endif //__FUNCMANAGER_HH__

