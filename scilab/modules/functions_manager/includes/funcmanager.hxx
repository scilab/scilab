/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
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

#ifndef __FUNCMANAGER_HH__
#define __FUNCMANAGER_HH__

#include <map>
#include <list>
#include <iostream>
#include <string>

//disable warnings about exports STL items
#ifdef _MSC_VER
#pragma warning (disable : 4251)
#endif

#ifdef _MSC_VER
#if FUNC_MAN_EXPORTS
#define EXTERN_FUNC_MAN __declspec (dllexport)
#else
#define EXTERN_FUNC_MAN __declspec (dllimport)
#endif
#else
#define EXTERN_FUNC_MAN
#endif

#define MODULE_DIR  L"/modules/"
#define MACRO_DIR   L"/macros/"
#define ETC_DIR     L"/etc/"
#define SCI_EXT     L"*.sci"
#define START_EXT   L".start"
#define QUIT_EXT    L".quit"

//Gateway function pointer
typedef int (*GW_MOD)(void);
typedef int (*FINALIZE_MODULE)(void);

class EXTERN_FUNC_MAN FuncManager
{
private :
    //	map <string, FuncInfo*>	m_FuncMap;
    typedef std::map<std::wstring, std::pair<GW_MOD, GW_MOD> >  ModuleMap;
    ModuleMap  m_ModuleMap;
    std::map<std::wstring, GW_MOD> m_ActivModuleMap;
    std::list<std::wstring> m_ModuleName;
    std::wstring m_szXmlFile;
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
    bool VerifyModule(wchar_t* ModuleName);

    bool CreateModuleList(void);

    bool ExecuteStartFile(const std::wstring& _stModule);
    bool ExecuteQuitFile(const std::wstring& _stModule);
    bool ExecuteFile(const std::wstring& _stFile);
    static FuncManager* me;
};

#endif //__FUNCMANAGER_HH__

