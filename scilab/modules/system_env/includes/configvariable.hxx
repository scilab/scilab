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
#include <string>
#include "dynlib_system_env.h"

#include "file.hxx"

using namespace std;
class EXTERN_SYSTEM_ENV ConfigVariable
{
    //module list
private :
    static list<wstring> m_ModuleList;

public :
    static void setModuleList(list<wstring>& _module_list);
    static std::list<wstring> getModuleList();


    //SCI
private :
    static wstring m_SCIPath;

public :
    static void setSCIPath(wstring& _SCIPath);
    static wstring getSCIPath();

    //HOME
private :
    static wstring m_SCIHOME;

public :
    static void setSCIHOME(wstring& _m_SCIHOME);
    static wstring getSCIHOME();

    //TMPDIR
private :
    static wstring m_TMPDIR;

public :
    static void setTMPDIR(wstring& _TMPDIR);
    static wstring getTMPDIR();

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
};

#endif /* __CONFIGVARIABLE_HXX__ */
