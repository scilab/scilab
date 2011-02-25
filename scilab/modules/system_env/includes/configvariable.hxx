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

#ifndef __CONFIGVARIABLE_HXX__
#define __CONFIGVARIABLE_HXX__

#include <list>
#include <map>
#include <string>
#include "threadId.hxx"
#include "cell.hxx"

extern "C"
{
#include "dynlib_system_env.h"
#include "Thread_Wrapper.h"
}

class EXTERN_SYSTEM_ENV ConfigVariable
{
    //module list
private :
    static std::list<std::wstring> m_ModuleList;

public :
    static void setModuleList(std::list<std::wstring>& _module_list);
    static std::list<std::wstring> getModuleList();


    //SCI
private :
    static std::wstring m_SCIPath;

public :
    static void setSCIPath(std::wstring& _SCIPath);
    static std::wstring getSCIPath();

    //SCIHOME
private :
    static std::wstring m_SCIHOME;

public :
    static void setSCIHOME(std::wstring& _m_SCIHOME);
    static std::wstring getSCIHOME();

    //TMPDIR
private :
    static std::wstring m_TMPDIR;

public :
    static void setTMPDIR(std::wstring& _TMPDIR);
    static std::wstring getTMPDIR();

    // Force Quit
private :
    static bool m_bForceQuit;

public :
    static void setForceQuit(bool _bForceQuit);
    static bool getForceQuit(void);

    // Exit Status
private :
    static int m_iExitStatus;

public :
    static void setExitStatus(int _iExitStatus);
    static int getExitStatus(void);

    // Digit precision, ex format function
private :
    static int m_iFormat;

public :
    static void setFormat(int _iFormat);
    static int getFormat(void);


    // Screen console width
private :
    static int m_iConsoleWidth;

public :
    static void setConsoleWidth(int _iConsoleWidth);
    static int getConsoleWidth(void);

    // Scilab mode
private :
    static int m_iScilabMode;

public :
    static void setScilabMode(int _iScilabMode);
    static int getScilabMode(void);

    //Warning mode
private :
    static bool m_bWarningMode;

public :
    static void setWarningMode(bool _bWarningMode);
    static bool getWarningMode(void);

    //HOME
private :
    static std::wstring m_HOME;

public :
    static void setHOME(std::wstring& _m_HOME);
    static std::wstring getHOME();

    //Clear last error information
public :
    static void setLastErrorClear(void);

    //Clear last error information
private :
    static bool m_bLastErrorCall;
public :
    static void setLastErrorCall(void);
    static void clearLastError(void);

    //Last Error Message
private :
    static std::wstring m_wstError;

public :
    static void setLastErrorMessage(std::wstring _wstError);
    static std::wstring getLastErrorMessage();

    //Last Error ID
private :
    static int m_iError;

public :
    static void setLastErrorNumber(int _iError);
    static int getLastErrorNumber();

    //Last Error Line
private :
    static int m_iErrorLine;

public :
    static void setLastErrorLine(int _iLine);
    static int getLastErrorLine();

    //Last Error Function
private :
    static std::wstring m_wstErrorFunction;

public :
    static void setLastErrorFunction(std::wstring _wstFunction);
    static std::wstring getLastErrorFunction();

    //Prompt Mode
public :
    /*
    normal = 0,
    silent = -1,
    prompt = 2,
    exec = 1,
    exec3 = 3,
    step = 4,
    step7 = 7
    */
private :
    static int m_iPromptMode;

public :
    static void setPromptMode(int _iPromptMode);
    static int getPromptMode(void);

    //Thread List

private :
    static std::map<__threadKey, types::ThreadId*>    m_threadList;
public :

    static types::ThreadId* getThread(__threadKey _key);
    static types::Cell* getAllThreads(void);
    static void setThread(__threadKey _key, types::ThreadId* _thread);
    static void deleteThread(__threadKey _key);


};

#endif /* __CONFIGVARIABLE_HXX__ */
